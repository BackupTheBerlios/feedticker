/**
 * @file popupmenu.cc
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

#include <boost/bind.hpp>

#include "popupmenu.hpp"
#include "message.hpp"
#include "callback.hpp"
#include "popupmenu.ui"

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::PopupMenu::PopupMenu(GtkBuilder *gtkBuilder,
                          GtkWindow  *windowToBind)
              : popupmenu(NULL)
{
    GError   *error = NULL;

    gint ret_code = gtk_builder_add_from_string (gtkBuilder, popupmenu_ui, -1, &error);
    if (ret_code == 0)
    {
        SHOW_GERROR_MESSAGE ("Unable to create Popupmenu", error);
        return;
    }

    gtk_builder_connect_signals (gtkBuilder, NULL);
    popupmenu = GTK_MENU (gtk_builder_get_object (gtkBuilder, "popupMenu"));
    gtk_menu_attach_to_widget (popupmenu, GTK_WIDGET(windowToBind), NULL);

    bcbPtr = ButtonCallbackPtr(new RSS::ButtonCallback(windowToBind, "button_press_event", boost::bind(&RSS::PopupMenu::buttonPressEvent, this, _1), 1));
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::PopupMenu::~PopupMenu()
{
    if (GTK_IS_MENU(popupmenu))
        gtk_menu_detach(popupmenu);
}

/*
 *
 * name: RSS::PopupMenu::connectWidget
 * @param
 * @return
 */
void RSS::PopupMenu::connectWidget(GtkWidget *widget)
{
    if (GTK_IS_MENU(popupmenu) && GTK_IS_WIDGET(widget))
    {
        signalMap[widget] = ButtonCallbackPtr(new RSS::ButtonCallback(widget, "button_press_event", boost::bind(&RSS::PopupMenu::buttonPressEvent, this, _1), 1));
    }
}

/*
 *
 * name: RSS::PopupMenu::disconnectWidget
 * @param
 * @return
 */
void RSS::PopupMenu::disconnectWidget(GtkWidget *widget)
{
    if (GTK_IS_MENU(popupmenu) && GTK_IS_WIDGET(widget))
    {
        RSS::SignalMap::iterator it = signalMap.find(widget);
        if (it != signalMap.end())
            signalMap.erase(it);
    }
}

/*
 *
 * name: RSS::PopupMenu::buttonPressEvent
 * @param
 * @return
 */
bool RSS::PopupMenu::buttonPressEvent(GdkEvent *event)
{
    GdkEventButton *event_button;

    g_return_val_if_fail (event != NULL, FALSE);

    if (event->type == GDK_BUTTON_PRESS)
    {
        event_button = (GdkEventButton *) event;
        if (event_button->button == 3)
        {
            gtk_menu_popup (popupmenu, NULL, NULL, NULL, NULL, event_button->button, event_button->time);
            return true;
        }
    }

    return false;
}
