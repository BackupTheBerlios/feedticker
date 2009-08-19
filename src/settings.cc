/**
 * @file settings.cc
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

#include <gdk/gdk.h>

#include "settings.hpp"

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
RSS::Settings::Settings()
             : opacity (DEFAULT_OPACITY),
               bSticky(false),
               bSkip(false),
               bAbove(false),
               bDock(false),
               xpos(SETTING_NOT_SET),
               ypos(SETTING_NOT_SET),
               xsize(SETTING_NOT_SET),
               ysize(SETTING_NOT_SET),
               reloadInterval(DEFAULT_RELOAD_INTERVAL)
{
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const gint RSS::Settings::getXPos() const
{
    if (xpos == SETTING_NOT_SET)
        return 0;

    return xpos;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const gint RSS::Settings::getYPos() const
{
    if (ypos == SETTING_NOT_SET)
        return 0;

    return ypos;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const gint RSS::Settings::getXSize() const
{
    if (xsize == SETTING_NOT_SET)
        return gdk_screen_width();

    return xsize;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
const gint RSS::Settings::getYSize(const gint def) const
{
    if (ysize == SETTING_NOT_SET)
        return def;

    return ysize;
}
