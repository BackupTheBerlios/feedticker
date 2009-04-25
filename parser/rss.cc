/**
 * @file rss.cc
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

#include <stdexcept>

#include <glib.h>
#include "util/xmlstring.hpp"
#include "rss.hpp"

/**
 *
 */
RSS::RssParser::RssParser(RSS::xmlDocPtr  docPtr)
              : RdfParser(docPtr)
{
}

/**
 *
 */
RSS::RssParser::~RssParser()
{
}

/**
 * name: RdfParser::parseChannel
 * @param xmlNodePtr
 * @return void
 */
void RSS::RssParser::parseChannel(xmlNodePtr              nodePtr,
                                  RSS::AbstractLayoutPtr  layout)
{
    RSS::RdfParser::parseChannel(nodePtr, layout);

    while (nodePtr != NULL)
    {
        if (xmlStrcasecmp (nodePtr->name, BAD_CAST"item") == 0)
            parseItem (nodePtr->xmlChildrenNode, layout);

        nodePtr = nodePtr->next;
    }
}

/**
 *
 */
bool RSS::RssParser::canParseFeed (xmlNodePtr   nodePtr)
{
    if (xmlStrcasecmp (nodePtr->name, BAD_CAST"rss") == 0)
        return true;

    return false;
}
