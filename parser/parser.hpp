/**
 * @file parser.hpp
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

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <list>

#include <boost/shared_ptr.hpp>
#include <libxml/parser.h>

#include "layout/layout.hpp"

namespace RSS
{
    /**
     *
     */
    typedef boost::shared_ptr<xmlDoc>         xmlDocPtr;
    typedef boost::shared_ptr<xmlParserCtxt>  xmlParserCtxtPtr;

    /**
     * abstract baseclass for all feed parser
     */
    class AbstractParser
    {
        protected:
            xmlDocPtr  doc;

        public:
            AbstractParser(xmlDocPtr  doc);
            virtual ~AbstractParser();

            virtual void reset();

            virtual bool canParseFeed(xmlNodePtr cur) = 0;

            virtual bool parseFeed(xmlNodePtr cur, RSS::AbstractLayoutPtr  layout) = 0;
    };

    typedef boost::shared_ptr<AbstractParser>  AbstractParserPtr;

    RSS::AbstractParserPtr  getParser (RSS::xmlDocPtr  docPtr, xmlNodePtr cur);
//    RSS::AbstractParserPtr  getParser (const TiXmlDocument  &doc);
}

#endif /*  __PARSER_HPP__ */
