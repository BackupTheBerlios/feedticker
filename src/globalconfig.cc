/**
 * @file globalsettings.cc
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

#include <gconf/gconf-client.h>

#include <boost/bind.hpp>

#include "controller.hpp"
#include "globalconfig.hpp"
#include "config.hpp"
#include "message.hpp"

#include "globalconfig.ui"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define GCONF_PROXY                     GCONF_SCHEMA "/proxy"
#define GCONF_PROXY_URL                 GCONF_PROXY "/url"
#define GCONF_PROXY_NEEDAUTHORIZATION   GCONF_PROXY "/needAuthorization"
#define GCONF_PROXY_USERNAME            GCONF_PROXY "/userName"
#define GCONF_PROXY_PASSWORD            GCONF_PROXY "/password"

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::GlobalConfig::GlobalConfig(const Controller  &control)
                 : controller(control),
                   gtkWindow(NULL)
{
    readConfigFromGConf();
}

/**
 *
 * name: RSS::GlobalConfig::~GlobalConfig
 * @param
 * @return
 */
RSS::GlobalConfig::~GlobalConfig()
{
    if (GTK_IS_WINDOW (gtkWindow))
        gtk_widget_hide_all (GTK_WIDGET (gtkWindow));
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::GlobalConfig::showDialog()
{
    if (gtkWindow == NULL)
    {
        GObject  *obj;
        GError   *error    = NULL;
        gint      ret_code = gtk_builder_add_from_string (controller.getGtkBuilder(), globalconfig_ui, -1, &error);
        if (ret_code == 0)
        {
            SHOW_GERROR_MESSAGE ("Unable to create global config dialog.", error);
            return;
        }

        gtkWindow = GTK_WINDOW (gtk_builder_get_object (controller.getGtkBuilder(), "globalConfigWindow"));

        obj       = gtk_builder_get_object (controller.getGtkBuilder(), "btnGlobalOk");
        btnCbSave = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::GlobalConfig::buttonSaveActivate, this)));

        obj         = gtk_builder_get_object (controller.getGtkBuilder(), "btnGlobalCancel");
        btnCbCancel = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::GlobalConfig::buttonCancelActivate, this)));
        
        obj              = gtk_builder_get_object (controller.getGtkBuilder(), "cbAuthorization");
        btnCbAuthToggled = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "toggled", boost::bind(&RSS::GlobalConfig::buttonAuthorizationToggled, this)));
    }

    setPreferencesToDialog();

    gtk_widget_show_all (GTK_WIDGET (gtkWindow));
}

/**
 *
 * name: RSS::GlobalConfig::setPreferencesToDialog
 * @param
 * @return
 */
void RSS::GlobalConfig::setPreferencesToDialog()
{
    GObject  *obj = gtk_builder_get_object (controller.getGtkBuilder(), "cbAuthorization");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj), proxy.needAuthorization);
    buttonAuthorizationToggled();
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::GlobalConfig::readConfigFromGConf()
{
    GConfClient   *client = gconf_client_get_default();
    gchar         *gcharValue;
    gboolean       needAuthorization;

    gcharValue = gconf_client_get_string (client, GCONF_PROXY_URL, NULL);
    if (gcharValue != NULL)
    {
        proxy.url = gcharValue;
        g_free (gcharValue);
    }
    
    needAuthorization = gconf_client_get_bool (client, GCONF_PROXY_NEEDAUTHORIZATION, NULL);
    if (needAuthorization)
    {
        proxy.needAuthorization = needAuthorization;
        
        gcharValue = gconf_client_get_string (client, GCONF_PROXY_USERNAME, NULL);
        if (gcharValue != NULL)
        {
            proxy.userName = gcharValue;
            g_free (gcharValue);
        }
        gcharValue = gconf_client_get_string (client, GCONF_PROXY_PASSWORD, NULL);
        if (gcharValue != NULL)
        {
            proxy.password = gcharValue;
            g_free (gcharValue);
        }
    }

    g_object_unref (client);
}

/**
 *
 * name: RSS::GlobalConfig::writeConfigToGConf
 * @param
 * @return
 */
void RSS::GlobalConfig::writeConfigToGConf()
{
    GError      *error = NULL;
    GConfClient *client = gconf_client_get_default();

    if (!proxy.url.empty())
    {
        if (!gconf_client_set_string (client, GCONF_PROXY_URL, proxy.url.c_str(), &error))
            SHOW_GERROR_MESSAGE ("Error while storing proxy:url settings.", error);
        
        if (proxy.needAuthorization)
        {
            if (!gconf_client_set_bool (client, GCONF_PROXY_NEEDAUTHORIZATION, proxy.needAuthorization, &error))
                SHOW_GERROR_MESSAGE ("Error while storing proxy settings.", error);
            if (!gconf_client_set_string (client, GCONF_PROXY_USERNAME, proxy.userName.c_str(), &error))
                SHOW_GERROR_MESSAGE ("Error while storing proxy:username settings.", error);
            if (!gconf_client_set_string (client, GCONF_PROXY_PASSWORD, proxy.password.c_str(), &error))
                SHOW_GERROR_MESSAGE ("Error while storing proxy:password settings.", error);
        }
        else
        {
            gconf_client_unset (client, GCONF_PROXY_NEEDAUTHORIZATION, NULL);
            gconf_client_unset (client, GCONF_PROXY_USERNAME, NULL);
            gconf_client_unset (client, GCONF_PROXY_PASSWORD, NULL);
        }
    }
    else
    {
        gconf_client_unset (client, GCONF_PROXY, NULL);
    }

    g_object_unref (client);
}

/**
 *
 * name: RSS::GlobalConfig::buttonAuthorizationToggled
 * @return void
 */
void RSS::GlobalConfig::buttonAuthorizationToggled()
{
    GObject  *obj = gtk_builder_get_object (controller.getGtkBuilder(), "cbAuthorization");
    proxy.needAuthorization = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (obj));
    
    GObject *objUser   = gtk_builder_get_object (controller.getGtkBuilder(), "enProxyUser");
    GObject *objPwd    = gtk_builder_get_object (controller.getGtkBuilder(), "enProxyPassword");
    GObject *objLbUser = gtk_builder_get_object (controller.getGtkBuilder(), "lbProxyUser");
    GObject *objLbPwd  = gtk_builder_get_object (controller.getGtkBuilder(), "lbProxyPassword");
    
    gtk_widget_set_sensitive (GTK_WIDGET (objUser), proxy.needAuthorization);
    gtk_widget_set_sensitive (GTK_WIDGET (objPwd),  proxy.needAuthorization);
    gtk_widget_set_sensitive (GTK_WIDGET (objLbUser), proxy.needAuthorization);
    gtk_widget_set_sensitive (GTK_WIDGET (objLbPwd),  proxy.needAuthorization);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::GlobalConfig::buttonCancelActivate()
{
    gtk_widget_hide_all (GTK_WIDGET (gtkWindow));
}

/**
 *
 * name: RSS::GlobalConfig::buttonSaveActivate
 * @return void
 */
void RSS::GlobalConfig::buttonSaveActivate()
{
    writeConfigToGConf();

    buttonCancelActivate();
}

