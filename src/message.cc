/**
 * @file message.cc
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

#include <iostream>
#include <sstream>
#include <string>

#include "message.hpp"

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::UIMessage::showMessage(const char       *message,
                                 GtkMessageType    type,
                                 const char       *func,
                                 const char       *file,
                                 const int         line)
{
    std::ostringstream  os;

    os << "Function: " << func << std::endl;
    os << "File: " << file << std::endl;
    os << "Line: " << line << std::endl;
    os << message << std::endl;

    GtkWidget  *dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, type, GTK_BUTTONS_OK, os.str().c_str());
    gtk_dialog_run (GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::UIMessage::showErrorMessage (const char  *format, const char *func, const char *file, const int line, ...)
{
    va_list       ap;
    char          buf[2048];

    va_start (ap, line);
    vsnprintf (buf, sizeof(buf) - 1, format, ap);
    va_end (ap);
    buf[sizeof(buf) - 1] = '\0';

    showMessage (buf, GTK_MESSAGE_ERROR, func, file, line);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::UIMessage::showErrorMessage (const char *msg, const char *func, const char *file, const int line, GError *error)
{
    std::ostringstream  os;

    os << msg << std::endl;
    os << error->message << " [" << error->code;
    os << "][" << std::hex << error->code << "]";

    g_error_free (error);

    showMessage (os.str().c_str(), GTK_MESSAGE_ERROR, func, file, line);
}

/**
 *
 * name: unbekannt
 * @param
 * @return
 */
void RSS::UIMessage::showWarningMessage (const char  *format, const char *func, const char *file, const int line, ...)
{
    va_list       ap;
    char          buf[2048];

    va_start (ap, line);
    vsnprintf (buf, sizeof(buf) - 1, format, ap);
    va_end (ap);
    buf[sizeof(buf) - 1] = '\0';

    showMessage (buf, GTK_MESSAGE_WARNING, func, file, line);
}
