/**
 * @file config.cc
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

#include <iostream>
#include <sstream>
#include <string>

#include <boost/bind.hpp>

#include "config.hpp"
#include "controller.hpp"
#include "message.hpp"
#include "callback.hpp"
#include "feedsetup.ui"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static const char         GCONF_TICKER[]  = GCONF_SCHEMA "/ticker/";
static const std::string  EMPTY_STRING("");

/**
 * Constructor
 *
 * name: RSS::Configuration::Configuration
 * @param RSS::Configuration::Controller
 * @return Configuration*
 */
RSS::Configuration::Configuration(Controller *controll)
                  : Settings(),
                    controller (controll),
                    settingWindow (NULL),
                    newTickerConfig (false)
{
}

/*
 * Destructor
 *
 * name: RSS::Configuration::~Configuration
 * @return void
 */
RSS::Configuration::~Configuration()
{
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::createTreeView()
{
    GtkTreeStore       *treestore;
    GtkTreeViewColumn  *column       = gtk_tree_view_column_new();
    GtkCellRenderer    *cellRenderer = gtk_cell_renderer_text_new();

    treeView = GTK_TREE_VIEW (gtk_builder_get_object (controller->getGtkBuilder(), "treeviewFeed"));

    gtk_tree_view_column_set_title (column, "Feed-URL");
    gtk_tree_view_insert_column (treeView, column, COL_URL);
    gtk_tree_view_column_pack_start (column, cellRenderer, TRUE);
    /* connect 'text' property of the cell renderer to model column that contains the last name */
    gtk_tree_view_column_add_attribute (column, cellRenderer, "text", COL_URL);
    gtk_tree_view_column_set_sort_column_id (column, COL_URL);

    column = gtk_tree_view_column_new ();
    gtk_tree_view_column_set_title (column, "Favicon");
    gtk_tree_view_insert_column (treeView, column, COL_FAVICON);
    gtk_tree_view_column_pack_end (column, cellRenderer, TRUE);
    /* connect 'text' property of the cell renderer to model column that contains the last name */
    gtk_tree_view_column_add_attribute (column, cellRenderer, "text", COL_FAVICON);
    gtk_tree_view_column_set_sort_column_id (column, COL_FAVICON);

    /* add treeModel */
    treestore = gtk_tree_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model (treeView, GTK_TREE_MODEL(treestore));
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::removeTicker()
{
    std::ostringstream    gconfDir;
    GConfClient          *client = gconf_client_get_default();

    gconfDir << GCONF_TICKER << tickerName;
    gconf_client_remove_dir (client, gconfDir.str().c_str(), NULL);
    g_object_unref (client);

    controller->removeTicker (this);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::connectPopupmenu()
{
    GObject    *obj = gtk_builder_get_object (controller->getGtkBuilder(), "mi_Edit");
    popupEditTicker = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "activate", boost::bind(&RSS::Configuration::showTickerConfigDialog, this)));

    obj               = gtk_builder_get_object (controller->getGtkBuilder(), "mi_remove_ticker");
    popupRemoveTicker = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "activate", boost::bind(&RSS::Configuration::removeTicker, this)));
}

/**
 *
 * name: RSS::Configuration::setPreferencesToDialog
 * @param
 * @return
 */
void RSS::Configuration::setPreferencesToDialog()
{
    GtkWidget      *entry;

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "entryTickername"));
    gtk_entry_set_text (GTK_ENTRY (entry), tickerName.c_str());
    if (!tickerName.empty())
        gtk_entry_set_editable (GTK_ENTRY (entry), FALSE);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "spinbuttonOpacity"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (entry), opacity * 100);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbSticky"));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (entry), bSticky);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbSkipTaskbar"));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (entry), bSkip);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbAbove"));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (entry), bAbove);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbDock"));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (entry), bDock);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbXpos"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (entry), xpos);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbYpos"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (entry), ypos);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbWidth"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (entry), xsize);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbHeight"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (entry), ysize);

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbReloadInterval"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (entry), reloadInterval);

    GtkTreeIter     toplevel;
    GtkTreeStore   *treestore = GTK_TREE_STORE (gtk_tree_view_get_model (treeView));
    gtk_tree_store_clear(treestore);
    for (StringStringMap::const_iterator cit = urlMap.begin(); cit != urlMap.end(); cit++)
    {
        gtk_tree_store_append (treestore, &toplevel, NULL);
        gtk_tree_store_set (treestore, &toplevel, COL_URL, cit->first.c_str(), COL_FAVICON, cit->second.c_str(), -1);
    }
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::showTickerConfigDialog()
{
    if (settingWindow == NULL)
    {
        GObject  *obj;
        GError   *error = NULL;

        gint  ret_code = gtk_builder_add_from_string (controller->getGtkBuilder(), feedsetup_ui, -1, &error);
        if (ret_code == 0)
        {
            SHOW_GERROR_MESSAGE ("Unable to crate Feeddialog.", error);
            return;
        }

        createTreeView();

        settingWindow  = GTK_WINDOW (gtk_builder_get_object (controller->getGtkBuilder(), "tickerSettings"));

        obj        = gtk_builder_get_object (controller->getGtkBuilder(), "btnUseIt");
        btnCbUseIt = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::Configuration::buttonSaveActivate, this)));

        obj         = gtk_builder_get_object (controller->getGtkBuilder(), "btnDontUseIt");
        btnCbCancel = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::Configuration::buttonCancelActivate, this)));

        obj          = gtk_builder_get_object (controller->getGtkBuilder(), "btnAddFeed");
        btnCbAddFeed = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::Configuration::buttonAddFeedActivate, this)));

        obj             = gtk_builder_get_object (controller->getGtkBuilder(), "btnRemoveFeed");
        btnCbRemoveFeed = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::Configuration::buttonRemoveFeedActivate, this)));

        btnFileChooser   = GTK_FILE_CHOOSER (gtk_builder_get_object (controller->getGtkBuilder(), "filechooserbutton"));
        btnCbFileChooser = ButtonCallbackPtr(new RSS::ButtonCallback(btnFileChooser, "file-set", boost::bind(&RSS::Configuration::fileChooserFileSet, this)));
    }

    setPreferencesToDialog();

    gtk_widget_show_all (GTK_WIDGET(settingWindow));
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const std::string  RSS::Configuration::getConfigFeedKey(guint  id)
{
    std::ostringstream   gconfKey;

    gconfKey << GCONF_TICKER << tickerName << "/feeds/" << id;

    return gconfKey.str();
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const std::string  RSS::Configuration::getConfigOpacityKey()
{
    std::ostringstream   gconfKey;

    gconfKey << GCONF_TICKER << tickerName << "/opacity";

    return gconfKey.str();
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const std::string  RSS::Configuration::getConfigKey(const char* keyName)
{
    std::ostringstream   gconfKey;

    gconfKey << GCONF_TICKER << tickerName << "/" << keyName;

    return gconfKey.str();
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::loadTickerConfig(const std::string &tName)
{
    std::ostringstream    gconfDir;
    GConfClient          *client = gconf_client_get_default();

    tickerName = tName;

    gconfDir << GCONF_TICKER << tickerName;
    if (gconf_client_dir_exists (client, gconfDir.str().c_str(), NULL))
    {
        opacity        = gconf_client_get_float (client, getConfigOpacityKey().c_str(), NULL);
        bSticky        = gconf_client_get_bool (client, getConfigKey("sticky_window").c_str(), NULL);
        bSkip          = gconf_client_get_bool (client, getConfigKey("skip_window").c_str(), NULL);
        bAbove         = gconf_client_get_bool (client, getConfigKey("above_window").c_str(), NULL);
        bDock          = gconf_client_get_bool (client, getConfigKey("dock_window").c_str(), NULL);
        xpos           = gconf_client_get_int (client, getConfigKey("xpos").c_str(), NULL);
        ypos           = gconf_client_get_int (client, getConfigKey("ypos").c_str(), NULL);
        xsize          = gconf_client_get_int (client, getConfigKey("xsize").c_str(), NULL);
        ysize          = gconf_client_get_int (client, getConfigKey("ysize").c_str(), NULL);
        reloadInterval = gconf_client_get_int (client, getConfigKey("reloadInterval").c_str(), NULL);

        // would 10000 feeds enough
        for (guint i = 0; i < 10000; i++)
        {
            gchar  *feedURL     = NULL;
            gchar  *favIconName = NULL;

            if (!gconf_client_get_pair (client, getConfigFeedKey(i).c_str(), GCONF_VALUE_STRING, GCONF_VALUE_STRING, &feedURL, &favIconName, NULL))
                break;

            if (feedURL != NULL)
            {
                if (favIconName != NULL)
                {
                    urlMap[std::string(feedURL)] = std::string(favIconName);
                    g_free (favIconName);
                }
                else
                    urlMap[std::string(feedURL)] = EMPTY_STRING;

                g_free (feedURL);
            }
            else
                break;
        }
    }

    g_object_unref (client);
}

/**
 *
 * name: RSS::Configuration::saveTickerConfig
 * @param
 * @return
 */
void RSS::Configuration::saveTickerConfig()
{
    GConfClient                      *client = gconf_client_get_default();
    GError                           *error  = NULL;
    StringStringMap::const_iterator   cit    = urlMap.begin();

    if (!gconf_client_set_float (client, getConfigOpacityKey().c_str(), opacity, &error))
    {
        SHOW_GERROR_MESSAGE ("Error while saving configuration (opacity).", error);
        error = NULL;
    }

    gconf_client_set_bool (client, getConfigKey("sticky_window").c_str(), bSticky, NULL);
    gconf_client_set_bool (client, getConfigKey("skip_window").c_str(), bSkip, NULL);
    gconf_client_set_bool (client, getConfigKey("above_window").c_str(), bAbove, NULL);
    gconf_client_set_bool (client, getConfigKey("dock_window").c_str(), bDock, NULL);
    gconf_client_set_int (client, getConfigKey("xpos").c_str(), xpos, NULL);
    gconf_client_set_int (client, getConfigKey("ypos").c_str(), ypos, NULL);
    gconf_client_set_int (client, getConfigKey("xsize").c_str(), xsize, NULL);
    gconf_client_set_int (client, getConfigKey("ysize").c_str(), ysize, NULL);
    gconf_client_set_int (client, getConfigKey("reloadInterval").c_str(), reloadInterval, NULL);

    for (guint i = 0; cit != urlMap.end(); i++, cit++)
    {
        const char* feedURL = cit->first.c_str();
        const char* favIcon = cit->second.c_str();

        if (!gconf_client_set_pair (client, getConfigFeedKey(i).c_str(), GCONF_VALUE_STRING, GCONF_VALUE_STRING, &feedURL, &favIcon, &error))
            SHOW_GERROR_MESSAGE ("Error while saving configuration.", error);
    }
}

/**
 *
 * name: RSS::Configuration::buttonSaveActivate
 * @param
 * @return
 */
void RSS::Configuration::buttonSaveActivate()
{
    GtkTreeIter     iter;
    GtkWidget      *entry;
    GtkTreeModel   *treeModel = gtk_tree_view_get_model (treeView);

    entry      = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "entryTickername"));
    tickerName = gtk_entry_get_text (GTK_ENTRY (entry));
    if (tickerName.empty())
    {
        gtk_widget_grab_focus (entry);
        return;
    }

    entry   = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "spinbuttonOpacity"));
    opacity = gtk_spin_button_get_value (GTK_SPIN_BUTTON (entry)) / 100;

    entry   = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbSticky"));
    bSticky = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (entry));

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbSkipTaskbar"));
    bSkip = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (entry));

    entry  = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbAbove"));
    bAbove = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (entry));

    entry  = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "cbDock"));
    bDock  = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (entry));

    entry   = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "spinbuttonOpacity"));
    opacity = gtk_spin_button_get_value (GTK_SPIN_BUTTON (entry)) / 100;

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbXpos"));
    xpos  = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (entry));

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbYpos"));
    ypos  = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (entry));

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbWidth"));
    xsize = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (entry));

    entry = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbHeight"));
    ysize = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (entry));

    entry          = GTK_WIDGET (gtk_builder_get_object (controller->getGtkBuilder(), "sbReloadInterval"));
    reloadInterval = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (entry));

    urlMap.clear();

    if (gtk_tree_model_get_iter_first (treeModel, &iter))
    {
        do
        {
            gchar *feedURL;
            gchar *faviconURL;

            gtk_tree_model_get (treeModel, &iter, COL_URL, &feedURL, COL_FAVICON, &faviconURL, -1);

            urlMap[std::string(feedURL)] = std::string(faviconURL);

            g_free (feedURL);
            g_free (faviconURL);
        }
        while (gtk_tree_model_iter_next (treeModel, &iter));
    }

    buttonCancelActivate ();
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::buttonCancelActivate()
{
    gtk_widget_hide_all (GTK_WIDGET (settingWindow));

    controller->reloadFeed(this);
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::fileChooserFileSet()
{
    GtkEntry *edFaviconURL = GTK_ENTRY (gtk_builder_get_object (controller->getGtkBuilder(), "edFavIconURL"));
    gchar    *fileName     = gtk_file_chooser_get_filename (btnFileChooser);
    if (fileName != NULL)
    {
        gtk_entry_set_text (edFaviconURL, fileName);
        g_free (fileName);
    }
    else
        gtk_entry_set_text (edFaviconURL, "");
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
bool RSS::Configuration::retriveNewFeedFromDialog()
{
    GtkEntry  *edFeedURL    = GTK_ENTRY (gtk_builder_get_object (controller->getGtkBuilder(), "edFeedURL"));
    GtkEntry  *edFaviconURL = GTK_ENTRY (gtk_builder_get_object (controller->getGtkBuilder(), "edFavIconURL"));

    const gchar  *feedURL    = gtk_entry_get_text(edFeedURL);
    const gchar  *faviconURL = gtk_entry_get_text(edFaviconURL);

    if ((feedURL == NULL) || (*feedURL == '\0'))
    {
        gtk_widget_grab_focus (GTK_WIDGET (edFeedURL));
        return false;
    }

    GtkTreeIter     toplevel;
    GtkTreeStore   *treestore = GTK_TREE_STORE (gtk_tree_view_get_model (treeView));
    gtk_tree_store_append (treestore, &toplevel, NULL);
    gtk_tree_store_set (treestore, &toplevel, COL_URL, feedURL, COL_FAVICON, faviconURL, -1);

    /* clear entryfields */
    gtk_entry_set_text (edFeedURL, "");
    gtk_entry_set_text (edFaviconURL, "");

    return true;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::Configuration::buttonAddFeedActivate()
{
    bool       canClose;
    GtkDialog *dlgNewFeed = GTK_DIALOG (gtk_builder_get_object (controller->getGtkBuilder(), "dlgNewFeed"));

    do
    {
        gint   result = gtk_dialog_run (dlgNewFeed);

        switch (result)
        {
            case GTK_RESPONSE_OK:
                canClose = retriveNewFeedFromDialog ();
                break;
            default:
                canClose = true;
                break;
        }

        gtk_widget_hide (GTK_WIDGET(dlgNewFeed));

    } while (!canClose);
}

/**
 *
 * name: RSS::Configuration::buttonRemoveFeedActivate
 * @return void
 */
void RSS::Configuration::buttonRemoveFeedActivate()
{
    GtkTreeIter        iter;
    GtkTreeModel      *treeModel;
    GtkTreeSelection  *selection = gtk_tree_view_get_selection (treeView);

    if (gtk_tree_selection_get_selected (selection, &treeModel, &iter))
        gtk_tree_store_remove (GTK_TREE_STORE(treeModel), &iter);
}
