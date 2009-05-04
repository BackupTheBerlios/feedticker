/**
 * @file globalconfig.hpp
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

#ifndef __GLOBALCONFIG_HPP__
#define __GLOBALCONFIG_HPP__

#include <gtk/gtk.h>

#include <string>

namespace RSS
{
    typedef struct __PROXY_SETTINGS
    {
        std::string      url;
        bool             needAuthorization;
        std::string      userName;
        std::string      password;
        
        __PROXY_SETTINGS() 
        : needAuthorization(false) {
        }
    } ProxySettings;
    
    class GlobalConfig
    {
        private:
            const RSS::Controller    &controller;
            
            ProxySettings         proxy;
            GtkWindow            *gtkWindow;
            ButtonCallbackPtr     btnCbSave;
            ButtonCallbackPtr     btnCbCancel;
            ButtonCallbackPtr     btnCbAuthToggled;

            void readConfigFromGConf();
            void writeConfigToGConf();

            void buttonSaveActivate();
            void buttonCancelActivate();
            void buttonAuthorizationToggled();
            
            void setPreferencesToDialog();

        public:
            GlobalConfig(const RSS::Controller  &control);
            ~GlobalConfig();

            void showDialog();

            inline const ProxySettings& getProxy() const {
                return proxy;
            }
    };
}

#endif /* __GLOBALCONFIG_HPP__ */
