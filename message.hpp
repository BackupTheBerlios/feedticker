/**
 * @file message.hpp
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

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <iostream>

namespace RSS
{
    class UIMessage
    {
        private:
            static void showMessage(const char *message, GtkMessageType type, const char *func, const char *file, const int line);

        public:
            static void showErrorMessage (const char *format, const char *func, const char *file, const int line, ...);
            static void showErrorMessage (const char *msg, const char *func, const char *file, const int line, GError *error);
            static void showWarningMessage (const char  *format, const char *func, const char *file, const int line, ...);
    };
}

#define SHOW_GERROR_MESSAGE(a, b)  \
    RSS::UIMessage::showErrorMessage(a, __FUNCTION__, __FILE__, __LINE__, b)

#define SHOW_ERROR_MESSAGE(a, b...)  \
    RSS::UIMessage::showErrorMessage(a, __FUNCTION__, __FILE__, __LINE__, ## b)

#define SHOW_WARNING_MESSAGE(a, b...)  \
    RSS::UIMessage::showErrorMessage(a, __FUNCTION__, __FILE__, __LINE__, ## b)

#endif /* __MESSAGE_HPP */
