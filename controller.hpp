/**
 * @file controller.hpp
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

#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

#include <gtk/gtk.h>

#include <string>
#include <map>

#include "layout.hpp"
#include "config.hpp"
#include "globalconfig.hpp"

namespace RSS
{
    typedef std::map<AbstractLayoutPtr, ConfigurationPtr>  LayoutMap;
    typedef std::list<std::string>                         StringList;

    class Controller
    {
        private:
            GtkBuilder          *gtkBuilder;
            LayoutMap            layoutMap;
            GlobalConfig         globalConfig;
            ButtonCallbackPtr    btnNewTicker;
            ButtonCallbackPtr    btnGlobalConfig;

            bool parseRssFeed(const char *filename, AbstractLayoutPtr  &layoutPtr);
            bool loadFeed(const std::string  &url, const std::string &favicon, AbstractLayoutPtr  &layoutPtr);
            void cleanupTmpFile(char *fileName);
            void closeFile(FILE *f);
            void setupLayoutFeed();
            void saveTickerToGconf();

            RSS::StringList readTickerFromGconf();

            void addNewTicker();
            void showGlobalConfig();

        public:
            Controller();
            ~Controller();

            void run();
            void timeout();

            void removeTicker (Configuration *cfg);

            /* callback for the config-dialog to reload the new settings */
            void reloadFeed(Configuration  *cfg);

            inline GtkBuilder* getGtkBuilder() const {
                return gtkBuilder;
            }

            inline guint getTickerCount() const {
                return layoutMap.size();
            }
            
            inline const GlobalConfig& getGlobalConfig() const {
                return globalConfig;
            }

            /* */
            static void init();

            /* GTK callback */
            static gboolean timeout(gpointer data);
    };
}

#endif /* __CONTROLLER_HPP__ */
