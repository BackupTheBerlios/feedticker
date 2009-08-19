/**
 * @file main.cc
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

#include <libxml/parser.h>

#include "controller.hpp"
#include "message.hpp"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static RSS::Controller  *controller  = NULL;

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
static gboolean initApplication(gpointer data)
{
    controller = new RSS::Controller();
    controller->run();

    return FALSE;
}

/*
*/
static gboolean quitApplication(gpointer data)
{
    if (controller != NULL)
        delete controller;

    return FALSE;
}

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
int main(int argc, char* argv[])
{
    if (!g_thread_supported())
        g_thread_init(NULL);

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    gtk_init (&argc, &argv);

    RSS::Controller::init();

    gtk_quit_add (0, quitApplication, NULL);

    g_idle_add (initApplication, NULL);

    gtk_main ();

    return 0;
}
