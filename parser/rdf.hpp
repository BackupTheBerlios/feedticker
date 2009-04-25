/**
 * @file rdf.h
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

#ifndef __RDF_H__
#define __RDF_H__

#include "parser.hpp"

namespace RSS
{
    class RdfParser : public RSS::AbstractParser
    {
        protected:
            virtual void parseChannel(xmlNodePtr cur, RSS::AbstractLayoutPtr  layout);
            virtual void parseItem(xmlNodePtr cur, RSS::AbstractLayoutPtr  layout);

        public:
            RdfParser(RSS::xmlDocPtr  docPtr);
            virtual ~RdfParser();

            virtual void reset ();
            virtual bool canParseFeed (xmlNodePtr cur);

            virtual bool parseFeed(xmlNodePtr               cur,
                                   RSS::AbstractLayoutPtr   layout) throw (std::invalid_argument);
    };

    typedef boost::shared_ptr<RdfParser>  RdfParserPtr;
}

#endif /*  __RDF_H__ */
