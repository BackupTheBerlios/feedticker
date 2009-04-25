/**
 * @file xmlstring.hpp
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

#ifndef __XMLSTRING_HPP__
#define __XMLSTRING_HPP__

#include <boost/shared_ptr.hpp>
#include <libxml/parser.h>

namespace RSS
{
    typedef boost::shared_ptr<xmlChar>  xmlCharPtr;

    /**
     *
     */
    class XmlString
    {
        private:
            RSS::xmlCharPtr  shared_ptr;

        public:
            XmlString();
            ~XmlString();

            XmlString& operator = (xmlChar* value);

            inline const xmlChar* operator -> () const
            {
                return shared_ptr.get();
            }

            inline const char* c_str() const
            {
                return const_cast<const char*>(reinterpret_cast<char*>(shared_ptr.get()));
            }
    };
    typedef boost::shared_ptr<XmlString>  XmlStringPtr;
}

#endif /*  __XMLSTRING_HPP__ */
