/**
 * @file config.hpp
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

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <boost/shared_ptr.hpp>

#include "callback.hpp"
#include "settings.hpp"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define GCONF_SCHEMA  "/apps/rssticker"

namespace RSS
{
    class Controller;

    class Configuration : public RSS::Settings
    {
        private:
            enum
            {
                COL_URL = 0U,
                COL_FAVICON,
                NUM_COLS
            };

            ButtonCallbackPtr     btnCbUseIt;
            ButtonCallbackPtr     btnCbCancel;
            ButtonCallbackPtr     btnCbAddFeed;
            ButtonCallbackPtr     btnCbRemoveFeed;
            ButtonCallbackPtr     btnCbFileChooser;
            ButtonCallbackPtr     popupEditTicker;
            ButtonCallbackPtr     popupRemoveTicker;

            Controller        *controller;
            GtkWindow         *settingWindow;
            GtkFileChooser    *btnFileChooser;
            GtkTreeView       *treeView;
            bool               newTickerConfig;

            void createTreeView();
            void setPreferencesToDialog();
            bool retriveNewFeedFromDialog();
            const std::string getConfigFeedKey(guint id);
            const std::string getConfigOpacityKey();
            const std::string getConfigKey(const char* keyName);

            void buttonSaveActivate();
            void buttonCancelActivate();
            void buttonAddFeedActivate();
            void buttonRemoveFeedActivate();
            void fileChooserFileSet();
            void removeTicker();

        public:
            Configuration(Controller *controll);
            ~Configuration();

            void showTickerConfigDialog();
            void loadTickerConfig(const std::string &tName);
            void saveTickerConfig();
            void connectPopupmenu();

            inline const bool isNewTicker() const {
                return newTickerConfig;
            }

            inline void setNewTicker(const bool flag) {
                newTickerConfig = flag;
            }

            inline bool empty() const {
                return urlMap.empty();
            }
    };
    typedef boost::shared_ptr<Configuration>  ConfigurationPtr;
}

#endif /* __CONFIG_HPP__ */
