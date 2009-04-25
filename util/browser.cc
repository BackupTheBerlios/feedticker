/**
 * @file browser.cc
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

#include <glib.h>

#include <list>

#include "browser.hpp"

/* */
typedef std::list<const char*>  ConstCharList;

/* list of browser to try on */
static const char*  BROWSER[] = {
    "xdg-open",
    "gnome-open",
    "firefox",
    "opera",
    "konqueror",
    "netscape"
};

/* this one uses the iterator constructor (first, last).
 * see: http://www.cplusplus.com/reference/stl/list/list.html
 *                                      [first]  [           last                              ] */
static const ConstCharList  browserList(BROWSER, BROWSER + sizeof(BROWSER) / sizeof(const char*));

/*
 *
 * name: unbekannt
 * @param
 * @return
 */
bool RSS::Browser::startBrowser(std::string &url)
{
    if (!url.empty())
    {
        const char *argv[3];

        argv[1] = url.c_str();
        argv[2] = NULL;

        for (ConstCharList::const_iterator it = browserList.begin(); it != browserList.end(); it++)
        {
            argv[0] = (*it);
            if (g_spawn_async(NULL, const_cast<char**>(argv), NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL))
                break;
            g_print ("%s: failed to launch %s\n", __FUNCTION__, (*it));
        }
    }
}

/*
void utils_start_browser(const gchar *uri)
{
#ifdef G_OS_WIN32
	win32_open_browser(uri);
#else
	gchar *cmdline = g_strconcat(tool_prefs.browser_cmd, " ", uri, NULL);

	if (! g_spawn_command_line_async(cmdline, NULL))
	{
		const gchar *argv[3];

		argv[0] = "xdg-open";
		argv[1] = uri;
		argv[2] = NULL;
		if (! g_spawn_async(NULL, (gchar**)argv, NULL, G_SPAWN_SEARCH_PATH,
				NULL, NULL, NULL, NULL))
		{
			argv[0] = "firefox";
			if (! g_spawn_async(NULL, (gchar**)argv, NULL, G_SPAWN_SEARCH_PATH,
					NULL, NULL, NULL, NULL))
			{
				argv[0] = "mozilla";
				if (! g_spawn_async(NULL, (gchar**)argv, NULL, G_SPAWN_SEARCH_PATH, NULL,
						NULL, NULL, NULL))
				{
					argv[0] = "opera";
					if (! g_spawn_async(NULL, (gchar**)argv, NULL, G_SPAWN_SEARCH_PATH,
							NULL, NULL, NULL, NULL))
					{
						argv[0] = "konqueror";
						if (! g_spawn_async(NULL, (gchar**)argv, NULL, G_SPAWN_SEARCH_PATH,
								NULL, NULL, NULL, NULL))
						{
							argv[0] = "netscape";
							g_spawn_async(NULL, (gchar**)argv, NULL, G_SPAWN_SEARCH_PATH,
								NULL, NULL, NULL, NULL);
						}
					}
				}
			}
		}
	}
	g_free(cmdline);
#endif
}
*/
