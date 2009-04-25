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

#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__

#include <map>
#include <string>

#define  SETTING_NOT_SET  -1

namespace RSS
{
    typedef std::map<std::string, std::string>   StringStringMap;

    class Settings
    {
        protected:
            static const gdouble  DEFAULT_OPACITY = 0.8;

        protected:
            StringStringMap    urlMap;
            std::string        tickerName;
            gdouble            opacity;
            bool               bSticky;
            bool               bSkip;
            bool               bAbove;
            bool               bDock;
            gint               xpos;
            gint               ypos;
            gint               xsize;
            gint               ysize;

        public:
            Settings();
            ~Settings() {
            }

            inline const std::string& getTickerName() const {
                return tickerName;
            }

            inline const StringStringMap&  getFeedMap() const {
                return urlMap;
            }

            inline const gdouble getOpacity() const {
                return opacity;
            }

            inline const bool isSticky() const {
                return bSticky;
            }

            inline const bool isSkip() const {
                return bSkip;
            }

            inline const bool isAbove() const {
                return bAbove;
            }

            inline const bool isDock() const {
                return bDock;
            }

            const gint getXPos() const;
            const gint getYPos() const;
            const gint getXSize() const;
            const gint getYSize(const gint def) const;
    };
}

#endif /* __SETTINGS_HPP__ */
