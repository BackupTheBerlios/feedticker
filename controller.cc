/**
 * @file controller.cc
 *
 * Copyright (C) 2009 T. Kordelle (feedthk@users.berlios.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <gconf/gconf-client.h>

#include <errno.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include <libxml/parser.h>
#include <libxml/xmlreader.h>

#include <iostream>
#include <boost/bind.hpp>

#include "controller.hpp"
#include "message.hpp"
#include "parser/parser.hpp"
#include "layout/defaultlayout.hpp"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define RSS_RELOAD_TIMEOUT   (15 * 60 * 1000)

static const char GCONF_TICKERLIST[] = GCONF_SCHEMA"/ticker_list";

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef boost::shared_ptr<CURL>  CurlHandlePtr;
typedef boost::shared_ptr<FILE>  FilePtr;
typedef boost::shared_ptr<char>  CharPtr;

/**
 * Constructor
 *
 * name: RSS::Controller::Controller
 * @return  RSS::Controller*
 */
RSS::Controller::Controller()
               : gtkBuilder(NULL),
                 globalConfig((*this))
{
    /* load the interface */
    gtkBuilder = gtk_builder_new ();
    if (gtkBuilder == NULL)
        SHOW_ERROR_MESSAGE ("unable to create GtkBuilder.");
}

/**
 * Destructor
 *
 * name: RSS::Controller::~Controller
 * @return void
 */
RSS::Controller::~Controller()
{
    layoutMap.clear();
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
bool RSS::Controller::parseRssFeed(const char         *filename,
                                   AbstractLayoutPtr  &layoutPtr)
{
    xmlNodePtr    nodePtr;

    RSS::xmlParserCtxtPtr ctxt(xmlNewParserCtxt(), xmlFreeParserCtxt);
    if (ctxt.get() == NULL)
    {
        SHOW_ERROR_MESSAGE ("Failed to allocate parser context");
        return false;
    }

    RSS::xmlDocPtr  doc (xmlSAXParseFile (ctxt->sax, filename, true), xmlFreeDoc);
    if (doc == NULL)
    {
        SHOW_ERROR_MESSAGE ("Error: unable to parse file \"%s\"\n", filename);
        return false;
    }

    nodePtr = xmlDocGetRootElement (doc.get());

    RSS::AbstractParserPtr parserPtr = RSS::getParser(doc, nodePtr);
    parserPtr->parseFeed(nodePtr, layoutPtr);

    return true;
}

/**
 * callback from boost::shared_ptr  CharPtr
 *
 * name: RSS::Controller::cleanupTmpFile
 * @param  char*  fileName
 * @return void
 */
void RSS::Controller::cleanupTmpFile(char *fileName)
{
    g_unlink (fileName);
    g_free (fileName);
}

/**
 * callback from boost::shared_ptr  FilePtr
 *
 * name: RSS::Controller::closeFile
 * @param FILE
 * @return void
 */
void RSS::Controller::closeFile(FILE *f)
{
    fflush(f);
    fclose(f);
}

/**
 *
 * name: RSS::Controller::loadFeed
 * @param
 * @return TRUE on success | FALES if feed cannot be loaded
 */
bool RSS::Controller::loadFeed(const std::string  &url,
                               const std::string  &favicon,
                               AbstractLayoutPtr  &layoutPtr)
{
    CharPtr    fileNamePtr;

    /* local scope needed to write and close temporary file before parsing */
    {
        char      *file_name = NULL;
        GError    *error = NULL;
        FilePtr    tmpFilePtr;
        gint       tmp_file_fd;
        CURLcode   curlError;

        tmp_file_fd = g_file_open_tmp ("rss_tickerXXXXXX.xml", &file_name, &error);
        if (tmp_file_fd == -1)
        {
            SHOW_GERROR_MESSAGE ("MKTEMP: cannot create temporary file.", error);
            return false;
        }
        fileNamePtr = CharPtr(file_name, boost::bind (&RSS::Controller::cleanupTmpFile, this, _1));
        tmpFilePtr  = FilePtr(fdopen(tmp_file_fd, "w"), boost::bind (&RSS::Controller::closeFile, this, _1));

        CurlHandlePtr curlHandle = CurlHandlePtr(curl_easy_init(), curl_easy_cleanup);
        if (((curlError = curl_easy_setopt(curlHandle.get(), CURLOPT_URL, url.c_str())) != CURLE_OK) ||
            ((curlError = curl_easy_setopt(curlHandle.get(), CURLOPT_NOPROGRESS, 1L)) != CURLE_OK) ||
            ((curlError = curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEDATA, tmpFilePtr.get())) != CURLE_OK) ||
            ((curlError = curl_easy_setopt(curlHandle.get(), CURLOPT_FOLLOWLOCATION, 1L)) != CURLE_OK)
            )
        {
            SHOW_ERROR_MESSAGE ("curl_easy_setopt failed.", curl_easy_strerror(curlError));
            return false;
        }

        if (!globalConfig.getProxy().empty())
        {
            if ((curlError = curl_easy_setopt(curlHandle.get(), CURLOPT_PROXY, globalConfig.getProxy().c_str())) != CURLE_OK)
            {
                SHOW_ERROR_MESSAGE ("curl_easy_setopt (proxy) failed.", curl_easy_strerror(curlError));
                return false;
            }
        }

        if ((curlError = curl_easy_perform (curlHandle.get())) != CURLE_OK)
        {
            SHOW_ERROR_MESSAGE ("curl_easy_perform failed.", curl_easy_strerror(curlError));
            return false;
        }
    }

    try
    {
        layoutPtr->setFavicon (favicon);
        parseRssFeed (fileNamePtr.get(), layoutPtr);
    }
    catch (std::exception &ex)
    {
        SHOW_ERROR_MESSAGE ("Exception:", ex.what());
    }

    return true;
}

/**
 *
 * name: RSS::Controller::setupLayoutFeed()
 * @return void
 */
void RSS::Controller::setupLayoutFeed()
{
    for(LayoutMap::iterator itLayout = layoutMap.begin(); itLayout != layoutMap.end(); itLayout++)
    {
        AbstractLayoutPtr  layoutPtr = itLayout->first;
        layoutPtr->clearFeed();

        StringStringMap  urlMap = itLayout->second->getFeedMap ();
        for (StringStringMap::const_iterator it = urlMap.begin(); it != urlMap.end(); it++)
        {
            loadFeed(it->first, it->second, layoutPtr);
        }
        layoutPtr->showTicker();

        /* Popupmenu is after first visible ticker available */
        if (btnNewTicker.get() == NULL)
        {
            GObject  *obj = gtk_builder_get_object (getGtkBuilder(), "mi_new_ticker");
            btnNewTicker = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "activate", boost::bind(&RSS::Controller::addNewTicker, this)));

            obj             = gtk_builder_get_object (getGtkBuilder(), "mi_global_config");
            btnGlobalConfig = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "activate", boost::bind(&RSS::Controller::showGlobalConfig, this)));
        }
    }
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Controller::timeout()
{
    setupLayoutFeed();
}

/**
 *
 * name: g_timeout
 * @param
 * @return
 */
gboolean RSS::Controller::timeout(gpointer  data)
{
    static_cast<RSS::Controller*>(data)->timeout();

    return TRUE;
}

/**
 *
 * name: RSS::Controller::readTickerFromGconf
 * @return RSS::StringList
 */
RSS::StringList RSS::Controller::readTickerFromGconf()
{
    GConfClient *client;
    GSList      *tList;
    GError      *error = NULL;
    StringList   tickerList;

    client = gconf_client_get_default();
    tList  = gconf_client_get_list (client, GCONF_TICKERLIST, GCONF_VALUE_STRING, &error);

    while (tList != NULL)
    {
        tickerList.push_back (std::string (static_cast<char*>(tList->data)));
        g_free (tList->data);
        tList = g_slist_next(tList);
    }

    if (tList != NULL)
        g_slist_free (tList);

    if (error != NULL)
        g_error_free (error);

    g_object_unref (client);

    return tickerList;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Controller::saveTickerToGconf ()
{
    GConfClient *client = gconf_client_get_default();
    GSList      *tList  = NULL;
    GError      *error  = NULL;

    for (LayoutMap::iterator itLayout = layoutMap.begin(); itLayout != layoutMap.end(); itLayout++)
    {
        itLayout->second->saveTickerConfig ();

        const std::string &tickerName = itLayout->second->getTickerName ();

        tList = g_slist_append (tList, const_cast<char*>(tickerName.c_str ()));
    }

    if (tList != NULL)
    {
        if (!gconf_client_set_list (client, GCONF_TICKERLIST, GCONF_VALUE_STRING, tList, &error))
            SHOW_GERROR_MESSAGE ("Error while saving configuration.", error);
    }
    else
    {
        if (!gconf_client_unset (client, GCONF_TICKERLIST, &error))
            SHOW_GERROR_MESSAGE ("Error while saving configuration.", error);
    }
    g_object_unref (client);
}


/*
 * callback for the config-dialog to reload the new settings
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Controller::reloadFeed(Configuration  *cfg)
{
    if (!cfg->empty())
    {
        if (cfg->isNewTicker())
        {
            cfg->setNewTicker (false);

            ConfigurationPtr  config = ConfigurationPtr (cfg);
            layoutMap[TickerLayoutPtr(new TickerLayout(config, (*this)))] = config;
        }
        saveTickerToGconf ();
        setupLayoutFeed ();
    }
    else
        delete cfg;

    /* if there is no feed and no other ticker running, there's no reason to stay */
    if (getTickerCount() == 0)
        gtk_main_quit();
}

/**
 *
 * name: RSS::Controller::removeTicker
 * @param
 * @return
 */
void RSS::Controller::removeTicker (Configuration *cfg)
{
    for(LayoutMap::iterator it = layoutMap.begin(); it != layoutMap.end(); it++)
    {
        Configuration *toRemove = it->second.get();
        if (toRemove == cfg)
        {
            layoutMap.erase (it);

            saveTickerToGconf();
            setupLayoutFeed();
            break;
        }
    }
}

/**
 *
 * name: RSS::Controller::addNewTicker
 * @param
 * @return
 */
void RSS::Controller::addNewTicker()
{
    Configuration  *cfg = new Configuration(this);
    cfg->setNewTicker (true);
    cfg->showTickerConfigDialog();
}

/**
 *
 * name: RSS::Controller::showGlobalConfig
 * @param
 * @return
 */
void RSS::Controller::showGlobalConfig()
{
    globalConfig.showDialog();
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Controller::run()
{
    StringList  tickerList;

    tickerList = readTickerFromGconf();

    if (tickerList.empty())
    {
        ConfigurationPtr  config(new Configuration(this));
        layoutMap[DefaultLayoutPtr(new DefaultLayout(config, (*this)))] = config;
    }
    else
    {
        StringList::const_iterator  it = tickerList.begin();
        for (; it != tickerList.end(); it++)
        {
            ConfigurationPtr  config(new Configuration(this));
            config->loadTickerConfig((*it));

            layoutMap[TickerLayoutPtr(new TickerLayout(config, (*this)))] = config;
        }
    }

    setupLayoutFeed();

    g_timeout_add (RSS_RELOAD_TIMEOUT, (GSourceFunc)RSS::Controller::timeout, this);
}

/**
 *
 * name: RSS::Controller::init
 * @return void
 */
void RSS::Controller::init()
{
    g_type_init();

    curl_global_init(CURL_GLOBAL_ALL);

    xmlInitParser();
}
