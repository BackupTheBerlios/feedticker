/**
 * @file rss.h
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

#ifndef __RSS_H__
#define __RSS_H__

#include "rdf.hpp"

namespace RSS
{
    class RssParser : public RSS::RdfParser
    {
        protected:
            virtual void parseChannel(xmlNodePtr cur, RSS::AbstractLayoutPtr  layout);

        public:
            RssParser(RSS::xmlDocPtr  docPtr);
            virtual ~RssParser();

            virtual bool canParseFeed (xmlNodePtr cur);
    };

    typedef boost::shared_ptr<RssParser>  RssParserPtr;
}

#endif /* __RSS_H__ */
