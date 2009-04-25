/**
 * @file feedentry.cc
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

#include "layout.hpp"

#define FEED_OFFSET     10
#define MOVE_OFFSET      1

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::FeedEntry::FeedEntry(const AbstractLayout  &abstractLayout)
              : layout(abstractLayout),
                is_on_layout(false),
                vbox(NULL),
                headline(NULL),
                description(NULL),
                xpos(0)
{
    vbox = gtk_vbox_new (FALSE, 0);
    /* increment refcount of the widget to move. to avoid destroy
     * when it is removed from layout
     */
    g_object_ref(vbox);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::FeedEntry::~FeedEntry()
{
    /* unref the widget. now is time to destroy */
    g_object_unref(vbox);

    removeFromLayout();

    if (GTK_IS_WIDGET(description))
        gtk_widget_destroy(description);

    if (GTK_IS_WIDGET(headline))
        gtk_widget_destroy(headline);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::FeedEntry::moveLeft()
{
    xpos -= MOVE_OFFSET;
    gtk_layout_move (layout.getGtkLayout(), vbox, xpos, 0);
}

/**
 *
 * name: RSS::FeedEntry::putOnLayout
 * @param  newXpos  position to put on
 * @return
 */
void RSS::FeedEntry::putOnLayout(int newXpos)
{
    xpos = newXpos;
    gtk_layout_put (layout.getGtkLayout(), vbox, xpos, 0);
    layout.getPopupMenu().connectWidget(headline);
    gtk_widget_show_all (vbox);
    is_on_layout = true;
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
bool RSS::FeedEntry::checkAndPutOnLayout(const RSS::FeedEntryPtr &previousEntry)
{
    if (previousEntry->isVisible())
    {
        GtkRequisition   requisition;
        int              newXpos;

        gtk_widget_size_request(previousEntry->vbox, &requisition);
        newXpos = previousEntry->xpos + requisition.width + FEED_OFFSET;
        if (newXpos <= gdk_screen_width())
            putOnLayout(newXpos);
    }
    return is_on_layout;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
bool RSS::FeedEntry::removeFromLayout()
{
    if ((is_on_layout) && (GTK_IS_WIDGET(vbox)))
    {
        layout.getPopupMenu().disconnectWidget(headline);
        gtk_container_remove (GTK_CONTAINER(layout.getGtkLayout()), vbox);
        is_on_layout = false;
        xpos         = 0;
    }

    return is_on_layout;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::FeedEntry::connectPopupmenu(PopupMenu *popupmenu)
{
    popupmenu->connectWidget(headline);
}
