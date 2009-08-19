/**
 * @file xmlstring.cc
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

#include "xmlstring.hpp"

/**
 *
 * name: XmlString::XmlString
 * @return XmlString*
 */
RSS::XmlString::XmlString()
{
}

/**
 *
 * name: RSS::XmlString::~XmlString
 * @return void
 */
RSS::XmlString::~XmlString()
{
/*    if (shared_ptr != NULL)
    {
        delete shared_ptr;
        shared_ptr = NULL;
    }*/
}

/**
 *
 * name: XmlString::operator =
 * @param  xmlChar*
 * @return XmlString&
 */
RSS::XmlString& RSS::XmlString::operator = (xmlChar* value)
{
    shared_ptr = RSS::xmlCharPtr(value, xmlFree);

    return (*this);
}
