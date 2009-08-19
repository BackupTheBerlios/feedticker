/**
 * @file feedcontroller.hpp
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

#ifndef __FEEDCONTROLLER_HPP__
#define __FEEDCONTROLLER_HPP__

#include <boost/shared_ptr.hpp>

/**
 *
 */
namespace RSS
{
    /**
     *
     * name: FeedHead
     */
    typedef struct
    {
        std::string  title;
        std::string  description;
        std::string  link;

        void clear() {
            title.clear();
            description.clear();
            link.clear();
        }
    } FeedHead;
    typedef boost::shared_ptr<FeedHead>  FeedHeadPtr;

    /**
     *
     */
    class FeedEntry
    {
        private:
            const AbstractLayout   &layout;
            bool                    is_on_layout;

        public:
            GtkWidget   *vbox;
            GtkWidget   *headline;
            GtkWidget   *description;
            std::string  link;
            gint         xpos;

            FeedEntry(const AbstractLayout &abstractLayout);
            virtual ~FeedEntry();

            void moveLeft();
            bool removeFromLayout();
            void putOnLayout(int newXpos);
            bool checkAndPutOnLayout(const boost::shared_ptr<FeedEntry> &lastEntry);

            void connectPopupmenu(PopupMenu *popupmenu);

            inline const bool isVisible() const {
                return is_on_layout;
            }
    };

    typedef boost::shared_ptr<FeedEntry>       FeedEntryPtr;
    typedef std::list<FeedEntryPtr>            FeedEntryList;
    typedef boost::shared_ptr<FeedEntryList>   FeedEntryListPtr;

    /**
     *
     */
    typedef struct
    {
        FeedHead         feedHead;
        FeedEntryList    feedList;
    } Feed;

    typedef boost::shared_ptr<Feed>  FeedPtr;
}

#endif /* __FEEDCONTROLLER_HPP__ */
