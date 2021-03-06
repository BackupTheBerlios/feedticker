/**
 * @file layout.hpp
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

#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include <gtk/gtk.h>

#include <list>
#include <string>

#include <boost/shared_ptr.hpp>

#include "config.hpp"
#include "popupmenu.hpp"
#include "feed/feedentry.hpp"

/**
 *
 */
namespace RSS
{
    /**
     *
     * name: AbstractLayout
     */
    class AbstractLayout
    {
        protected:
            GtkLayout    *layoutWidget;
            PopupMenu    *popupmenu;

        public:
            AbstractLayout()
            : layoutWidget(NULL),
              popupmenu(NULL)
            {  }

            virtual ~AbstractLayout()
            {  }

            virtual void setFeedInfo(const char*  head,
                                     const char*  description,
                                     const char*  link) = 0;

            virtual void addEntry(const char*  headline,
                                  const char*  description,
                                  const char*  link) = 0;

            virtual void setFavicon (const std::string &fileNameIcon) = 0;

            virtual void showTicker() = 0;

            virtual void clearFeed() = 0;

            inline GtkLayout*  getGtkLayout() const {
                return layoutWidget;
            }

            inline PopupMenu& getPopupMenu() const {
                return (*popupmenu);
            }
    };
    typedef boost::shared_ptr<AbstractLayout>   AbstractLayoutPtr;

    /**
     *
     */
    class TickerLayout : public RSS::AbstractLayout
    {
        private:
            ConfigurationPtr    config;
            std::string         faviconName;
            const Controller   &controller;
            GtkWindow          *topLevelWindow;
            Feed                feed;

            void   createFeedWindow();
            void   setupKeyboardShortcuts();
            void   resortFeedLayout();

        protected:
            guint               timeoutId;
            guint               timeoutReloadId;

        public:
            TickerLayout(ConfigurationPtr    cfg,
                         const Controller   &controll);
            virtual ~TickerLayout();

            virtual void setFeedInfo(const char*  head,
                                     const char*  description,
                                     const char*  link);

            virtual void addEntry(const char*  headline,
                                  const char*  description,
                                  const char*  link);

            virtual void setFavicon (const std::string &fileNameIcon) {
                faviconName = fileNameIcon;
            }

            virtual void showTicker();
            virtual void clearFeed();

            void timeout();

            GtkWindow*  getGtkWindow() const {
                return topLevelWindow;
            }

            void moveWindowDelayed();

            /* G_CALLBACK */
            static gboolean timeout (gpointer data);
            static gboolean idleCallback (gpointer data);
    };

    typedef boost::shared_ptr<TickerLayout>  TickerLayoutPtr;
}

#endif /*  __LAYOUT_HPP__ */
