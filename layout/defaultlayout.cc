/**
 * @file defaultlayout.cc
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

#include "defaultlayout.hpp"

/**
 * 
 * name: RSS::DefaultLayout::DefaultLayout
 * @param
 * @return
 */
RSS::DefaultLayout::DefaultLayout(ConfigurationPtr    cfg,
                                  const Controller   &controll)
                   : TickerLayout (cfg, controll)
{
    addEntry ("Feedticker", NULL, "feedticker.berlios.de");
    addEntry ("Copyright(c) 2009  T.Kordelle", NULL, "mailto:feedthk@users.berlios.de");
}
