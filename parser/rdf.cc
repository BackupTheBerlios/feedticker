/**
 * @file rdf.cc
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
#include "rdf.hpp"

/**
 *
 */
RSS::RdfParser::RdfParser(RSS::xmlDocPtr  docPtr)
              : AbstractParser(docPtr)
{
}

/**
 *
 */
RSS::RdfParser::~RdfParser()
{
}

static void myFree(xmlChar* a)
{
    printf("%s: called with %p\n", __FUNCTION__, a);
    xmlFree(a);
}

/**
 * name: RdfParser::parseChannel
 * @param xmlNodePtr
 * @return void
 */
void RSS::RdfParser::parseChannel(xmlNodePtr              nodePtr,
                                  RSS::AbstractLayoutPtr  layout)
{
    RSS::XmlStringPtr   title(new RSS::XmlString);
    RSS::XmlStringPtr   description(new RSS::XmlString);
    RSS::XmlStringPtr   link(new RSS::XmlString);

    while (nodePtr != NULL)
    {
        if ((nodePtr->type == XML_ELEMENT_NODE) && (nodePtr->name != NULL))
        {
            if (xmlStrcasecmp (nodePtr->name, BAD_CAST"title") == 0)
                (*title) = xmlNodeListGetString(doc.get(), nodePtr->xmlChildrenNode, 1);
            else if (xmlStrcasecmp (nodePtr->name, BAD_CAST"description") == 0)
                (*description) = xmlNodeListGetString(doc.get(), nodePtr->xmlChildrenNode, 1);
            else if (xmlStrcasecmp (nodePtr->name, BAD_CAST"link") == 0)
                (*link) = xmlNodeListGetString(doc.get(), nodePtr->xmlChildrenNode, 1);
        }
        nodePtr = nodePtr->next;
    }

    layout->setFeedInfo(title->c_str(), description->c_str(), link->c_str());
}

/**
 * name: RdfParser::parseItem
 * @param xmlNodePtr
 * @return void
 */
void RSS::RdfParser::parseItem(xmlNodePtr              nodePtr,
                               RSS::AbstractLayoutPtr  layout)
{
    RSS::XmlStringPtr   title(new RSS::XmlString);
    RSS::XmlStringPtr   description(new RSS::XmlString);
    RSS::XmlStringPtr   link(new RSS::XmlString);

    while (nodePtr != NULL)
    {
        if ((nodePtr->type == XML_ELEMENT_NODE) && (nodePtr->name != NULL))
        {
            if (xmlStrcasecmp (nodePtr->name, BAD_CAST"title") == 0)
                (*title) = xmlNodeListGetString(doc.get(), nodePtr->xmlChildrenNode, 1);
            else if (xmlStrcasecmp (nodePtr->name, BAD_CAST"link") == 0)
                (*link) = xmlNodeListGetString(doc.get(), nodePtr->xmlChildrenNode, 1);
            else if (xmlStrcasecmp (nodePtr->name, BAD_CAST"description") == 0)
                (*description) = xmlNodeListGetString(doc.get(), nodePtr->xmlChildrenNode, 1);
        }
        nodePtr = nodePtr->next;
    }

    layout->addEntry(title->c_str(), description->c_str(), link->c_str());
}

/**
 *
 */
void RSS::RdfParser::reset ()
{
    RSS::AbstractParser::reset();
}

/**
 *
 */
bool RSS::RdfParser::canParseFeed (xmlNodePtr   nodePtr)
{
    if (xmlStrcasecmp (nodePtr->name, BAD_CAST"rdf") == 0)
        return true;

    return false;
}

/**
 *
 */
bool RSS::RdfParser::parseFeed(xmlNodePtr              nodePtr,
                               RSS::AbstractLayoutPtr  layout) throw (std::invalid_argument)
{
    if (canParseFeed (nodePtr))
        nodePtr = nodePtr->xmlChildrenNode;
    else
        throw std::invalid_argument ("RdfParser: unable to channel in feed");

    while (nodePtr != NULL)
    {
        if (xmlStrcasecmp (nodePtr->name, BAD_CAST"channel") == 0)
            parseChannel (nodePtr->xmlChildrenNode, layout);

        if (xmlStrcasecmp (nodePtr->name, BAD_CAST"item") == 0)
            parseItem (nodePtr->xmlChildrenNode, layout);

        nodePtr = nodePtr->next;
    }

    return true;
}
