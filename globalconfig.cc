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
#define GCONF_PROXY   GCONF_SCHEMA "/proxy"

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::GlobalConfig::GlobalConfig(Controller  &control)
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

        obj       = gtk_builder_get_object (controller.getGtkBuilder(), "btnSave");
        btnCbSave = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::GlobalConfig::buttonSaveActivate, this)));

        obj         = gtk_builder_get_object (controller.getGtkBuilder(), "btnSave");
        btnCbCancel = ButtonCallbackPtr(new RSS::ButtonCallback(obj, "clicked", boost::bind(&RSS::GlobalConfig::buttonCancelActivate, this)));
    }

    gtk_widget_show_all (GTK_WIDGET (gtkWindow));
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
    gchar         *gProxy;

    gProxy = gconf_client_get_string (client, GCONF_PROXY, NULL);
    if (gProxy != NULL)
    {
        proxy = gProxy;
        g_free (gProxy);
    }

    g_object_unref (client);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::GlobalConfig::writeConfigToGConf()
{
    GError      *error = NULL;
    GConfClient *client = gconf_client_get_default();

    if (!proxy.empty())
    {
        if (!gconf_client_set_string (client, GCONF_PROXY, proxy.c_str(), &error))
            SHOW_GERROR_MESSAGE ("Error while storing proxy settings.", error);
    }
    else
    {
        gconf_client_unset (client, GCONF_PROXY, NULL);
    }

    g_object_unref (client);
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
 * name: unbekannt
 * @param
 * @return
 */
void RSS::GlobalConfig::buttonSaveActivate()
{
    buttonCancelActivate();
}

