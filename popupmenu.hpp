/**
 * @file popupmenu.hpp
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

#ifndef __POPUPMENU_HPP__
#define __POPUPMENU_HPP__

#include <gtk/gtk.h>

#include <map>

#include "callback.hpp"

namespace RSS
{
    typedef std::map<GtkWidget*, ButtonCallbackPtr>  SignalMap;

    class PopupMenu
    {
        private:
            GtkMenu           *popupmenu;
            ButtonCallbackPtr  bcbPtr;
            SignalMap  signalMap;

        public:
            PopupMenu(GtkBuilder *gtkBuilder, GtkWindow  *windowToBind);
            ~PopupMenu();

            void connectWidget(GtkWidget *widget);
            void disconnectWidget(GtkWidget *widget);

            bool buttonPressEvent(GdkEvent *event);
    };
}

#endif /* __POPUPMENU_HPP__ */
