/**
 * @file layout.cc
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
#include <gdk/gdkkeysyms.h>

#include <iterator>
#include "layout.hpp"
#include "util/browser.hpp"
#include "message.hpp"
#include "controller.hpp"

/**
 *
 * name: RSS::TickerLayout::TickerLayout
 * @param layout
 * @return RSS::TickerLayout*
 */
RSS::TickerLayout::TickerLayout(ConfigurationPtr    cfg,
                                const Controller   &controll)
                 : AbstractLayout(),
                   config(cfg),
                   controller(controll),
                   topLevelWindow(NULL),
                   timeoutId(0U),
                   timeoutReloadId(0U)
{
    createFeedWindow();
}

/**
 * Destructor
 *
 * name: RSS::TickerLayout::~TickerLayout()
 * @return void
 */
RSS::TickerLayout::~TickerLayout()
{
    clearFeed();

    if (popupmenu != NULL)
        delete popupmenu;

    gtk_container_remove(GTK_CONTAINER (topLevelWindow), GTK_WIDGET (layoutWidget));
    g_object_unref (topLevelWindow);
}

/**
 *
 * name: RSS::TickerLayout::createFeedWindow
 * @return void
 */
void RSS::TickerLayout::createFeedWindow()
{
    topLevelWindow = GTK_WINDOW(gtk_window_new (GTK_WINDOW_TOPLEVEL));

    if (config->isDock())
        gtk_window_set_type_hint (topLevelWindow, GDK_WINDOW_TYPE_HINT_DOCK);

    if (config->isSticky())
        gtk_window_stick (topLevelWindow);

    if (config->isSkip())
        gtk_window_set_skip_taskbar_hint (topLevelWindow, config->isSkip());

    if (config->isAbove())
        gtk_window_set_keep_above (topLevelWindow, config->isAbove());

    gtk_window_set_title (topLevelWindow, config->getTickerName().c_str());
    gtk_window_set_role (topLevelWindow, config->getTickerName().c_str());

    gtk_window_set_default_size (topLevelWindow, gdk_screen_width(), 10);
    gtk_window_set_decorated (topLevelWindow, FALSE);

    gtk_widget_set_double_buffered (GTK_WIDGET(topLevelWindow), TRUE);

    g_signal_connect(topLevelWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    popupmenu = new PopupMenu(controller, topLevelWindow);

    config->connectPopupmenu();

    setupKeyboardShortcuts();

    layoutWidget = GTK_LAYOUT(gtk_layout_new (NULL, NULL));

    gtk_container_add (GTK_CONTAINER (topLevelWindow), GTK_WIDGET (layoutWidget));
}

/**
 *
 * name: RSS::TickerLayout::setupKeyboardShortcuts
 * @return void
 */
void RSS::TickerLayout::setupKeyboardShortcuts()
{
    GtkAccelGroup  *accel_group;
    GClosure       *gclosure;

    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group (GTK_WINDOW (topLevelWindow), accel_group);

    /* Exit on <Ctrl>q */
    gclosure = g_cclosure_new_swap (G_CALLBACK(gtk_main_quit), topLevelWindow, NULL);
    gtk_accel_group_connect (accel_group, GDK_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE, gclosure);
}

/**
 *
 * name: RSS::TickerLayout::resortFeedLayout
 * @return void
 */
void RSS::TickerLayout::resortFeedLayout()
{
    if (feed.feedList.empty())
        return;

    GtkRequisition   requisition;
    FeedEntryPtr     firstFeed;
    FeedEntryPtr     lastFeed;

    /* check if the first entry of the list has left the screen */
    firstFeed = feed.feedList.front();
    gtk_widget_size_request(firstFeed->vbox, &requisition);

    /* Feed has left
     * resort feed from first to last entry
     */
    if ((firstFeed->xpos + requisition.width) < 0)
    {
        /* removed feed from list */
        feed.feedList.erase(feed.feedList.begin());
        /* remove the widget from the layout (screen) */
        firstFeed->removeFromLayout();
        /* get the last entry from list */
        lastFeed = feed.feedList.back();
        /* place the feed on the new position */
        firstFeed->checkAndPutOnLayout(lastFeed);
        /* append the feed again to the list as last */
        feed.feedList.push_back(firstFeed);
    }
}

/**
 *
 * name: RSS::TickerLayout::setFeedInfo
 * @param title
 * @param description
 * @param link
 * @return void
 */
void RSS::TickerLayout::setFeedInfo(const char*  title,
                                    const char*  description,
                                    const char*  link)
{
    feed.feedHead.title       = title;
    feed.feedHead.description = description;
    feed.feedHead.link        = link;
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
static void url_clicked(GtkButton *button, RSS::FeedEntry *fe)
{
    RSS::Browser::startBrowser(fe->link);
}

/**
 *
 * name: RSS::TickerLayout::addEntry
 * @param  headline
 * @param  description
 * @return void
 */
void RSS::TickerLayout::addEntry(const char* headline,
                                 const char* description,
                                 const char* link)
{
    FeedEntryPtr     feedEntry;
    FeedEntryPtr     lastFeedEntry;
    GtkRequisition   requisition;

    feedEntry = FeedEntryPtr(new FeedEntry((*this)));

    if (link == NULL)
    {
        feedEntry->headline = gtk_label_new (headline);
        gtk_box_pack_start (GTK_BOX(feedEntry->vbox), feedEntry->headline, FALSE, FALSE, 0);
    }
    else
    {
        feedEntry->link     = link;
        feedEntry->headline = gtk_button_new_with_label (headline);
        if (!faviconName.empty())
        {
            GtkWidget  *favicon = gtk_image_new_from_file (faviconName.c_str ());
            gtk_button_set_image (GTK_BUTTON(feedEntry->headline), favicon);
        }
        gtk_button_set_relief(GTK_BUTTON(feedEntry->headline), GTK_RELIEF_NONE);
        gtk_button_set_focus_on_click(GTK_BUTTON(feedEntry->headline), FALSE);
        g_signal_connect(G_OBJECT(feedEntry->headline), "clicked", G_CALLBACK(url_clicked), (void*)feedEntry.get());
        gtk_widget_show_all(feedEntry->headline);
        gtk_box_pack_start (GTK_BOX(feedEntry->vbox), feedEntry->headline, FALSE, FALSE, 0);
    }

    if (description != NULL)
        gtk_widget_set_tooltip_text (feedEntry->headline, description);

    int newXpos;
    if (feed.feedList.empty())
        feedEntry->putOnLayout(gdk_screen_width () >> 2); // div 2
    else
        feedEntry->checkAndPutOnLayout(feed.feedList.back());

    feed.feedList.push_back(feedEntry);
}

/**
 *
 * name: RSS::TickerLayout::timeout
 * @param
 * @return
 */
void RSS::TickerLayout::timeout()
{
    /* check if feed must be sort at last */
    resortFeedLayout();

    /* move all visible feed left */
    FeedEntryPtr  lastFeedEntry;
    for (FeedEntryList::const_iterator it = feed.feedList.begin(); it != feed.feedList.end(); it++)
    {
        const FeedEntryPtr &feedEntry = (*it);

        if (feedEntry->isVisible())
        {
            lastFeedEntry = feedEntry;
            feedEntry->moveLeft();
        }
        else
        {
            feedEntry->checkAndPutOnLayout(lastFeedEntry);
            break; // leave loop. now one more do display on screen
        }
    }
}

/**
 *
 * name: RSS:TickerLayout::clearFeed
 * @param
 * @return
 */
void RSS::TickerLayout::clearFeed()
{
    if (timeoutId > 0U)
    {
        if (!g_source_remove (timeoutId))
            SHOW_ERROR_MESSAGE("Can't remove g_timout_source.");

        timeoutId = 0U;
    }

    feed.feedHead.clear();
    feed.feedList.clear();
}

/**
 *
 * name: g_timeout
 * @param
 * @return
 */
gboolean RSS::TickerLayout::timeout(gpointer  data)
{
    static_cast<RSS::TickerLayout*>(data)->timeout();

    return TRUE;
}

/**
 *
 * name: RSS::TickerLayout::moveWindowDelayed
 * @param
 * @return
 */
void RSS::TickerLayout::moveWindowDelayed()
{
    /* Window managers are free to ignore this; most window managers
     * ignore requests for initial window positions (instead using a
     * user-defined placement algorithm) and honor requests after
     * the window has already been shown.
     */
    gtk_window_move (topLevelWindow, config->getXPos(), config->getYPos());
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
gboolean RSS::TickerLayout::idleCallback(gpointer data)
{
    static_cast<RSS::TickerLayout*>(data)->moveWindowDelayed();
    return FALSE;
}

/**
 *
 * name: RSS::TickerLayout::showTicker
 * @param
 * @return
 */
void RSS::TickerLayout::showTicker()
{
    gtk_widget_show_all (GTK_WIDGET(topLevelWindow));

    if (!feed.feedList.empty())
    {
        GtkRequisition   requisition;

        gtk_widget_size_request(feed.feedList.front()->vbox, &requisition);
        gtk_window_resize(topLevelWindow, config->getXSize(), config->getYSize(requisition.height));
    }

    /* first time window_move must be delayed. all event from show_all
     * must passed before window_move will be accepted
     */
    g_idle_add (RSS::TickerLayout::idleCallback, this);

    /* after show_all
     * make the window a littlebit shiny */
    gdk_window_set_opacity (GTK_WIDGET(topLevelWindow)->window, config->getOpacity());

    timeoutId = g_timeout_add (50, static_cast<GSourceFunc>(RSS::TickerLayout::timeout), this);
}
