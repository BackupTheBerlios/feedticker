/**
 * @file defaultlayout.hpp
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

#ifndef __DEFAULTLAYOUT_HPP__
#define __DEFAULTLAYOUT_HPP__

#include "layout.hpp"

/**
 *
 */
namespace RSS
{
    class DefaultLayout : public TickerLayout
    {
        public:
            DefaultLayout(ConfigurationPtr    cfg,
                          const Controller   &controll);

            virtual ~DefaultLayout() {
            }

            virtual void showTicker();
            virtual void clearFeed();
    };
    typedef boost::shared_ptr<DefaultLayout>  DefaultLayoutPtr;
}

#endif /* __DEFAULTLAYOUT_HPP__ */
