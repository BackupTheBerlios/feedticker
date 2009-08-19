/**
 * @file parser.cc
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
#include <list>
#include <string>

#include "parser.hpp"
#include "rdf.hpp"
#include "rss.hpp"

/**
 *
 */
RSS::AbstractParser::AbstractParser(xmlDocPtr  dc)
                   : doc(dc)
{
}

/**
 *
 */
RSS::AbstractParser::~AbstractParser()
{
    reset();
}

/**
 *
 */
void RSS::AbstractParser::reset ()
{
}

namespace RSS
{
    /**
     *
     */
    typedef std::list<RSS::AbstractParserPtr>    ParserList;
    /**
     * pack list into a shared_ptr for automatic garbage collection
     */
    typedef boost::shared_ptr<ParserList>   ParserListPtr;
}

/**
 *
 * name: initParser
 * @return ParserListPtr
 */
static RSS::ParserListPtr initParser(RSS::xmlDocPtr  doc)
{
    RSS::ParserListPtr     parserList(new RSS::ParserList);

    parserList.get()->push_back (RSS::AbstractParserPtr(new RSS::RdfParser(doc)));
    parserList.get()->push_back (RSS::AbstractParserPtr(new RSS::RssParser(doc)));

    return parserList;
}

/**
 *
 * name: getParser
 * @param  xmlNodePtr
 * @return AbstractParserPtr
 * @throw std::invalid_argument
 */
RSS::AbstractParserPtr  RSS::getParser(RSS::xmlDocPtr  docPtr, xmlNodePtr cur)
{
    RSS::ParserList::const_iterator   it;
    RSS::ParserListPtr                pList = initParser(docPtr);

    for (it = pList->begin(); it != pList->end(); ++it)
    {
        RSS::AbstractParserPtr  ptr = (*it);
        if (ptr->canParseFeed (cur))
            return ptr;
    }

    std::string  message("can not parse feed with name = ");
    message += reinterpret_cast<const char*>(cur->name);

    throw std::invalid_argument (message.c_str());
}
