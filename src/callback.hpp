/**
 * @file callback.hpp
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

#ifndef __CALLBACK_HPP__
#define __CALLBACK_HPP__

#include <gdk/gdk.h>

#include <boost/shared_ptr.hpp>

namespace RSS
{
    class CallbackBase
    {
        public:
            CallbackBase() {
            }
            virtual ~CallbackBase() {
            }
            virtual void doCallback() const = 0;
            virtual void doCallback(GdkEvent *event) const = 0;
    };

    template <class C>
    class CallbackImpl1 : public RSS::CallbackBase
    {
        private:
            C  cb;

        public:
            CallbackImpl1(C c)
            : cb(c)
            {  }
            virtual ~CallbackImpl1()
            {  }

            virtual void doCallback() const {
                cb();
            }
            virtual void doCallback(GdkEvent *event) const {
            }
    };

    template <class C>
    class CallbackImpl2 : public RSS::CallbackBase
    {
        private:
            C  cb;

        public:
            CallbackImpl2(C c)
            : cb(c)
            {  }
            virtual ~CallbackImpl2()
            {  }

            virtual void doCallback() const
            {  }

            virtual void doCallback(GdkEvent *event) const {
                cb(event);
            }
    };

    class ButtonCallback
    {
        private:
            CallbackBase  *theCallback;
            gulong         handlerId;
            gpointer       instance;

        public:
            template <class C>
            ButtonCallback(gpointer instanze, const char* signal, C c)
            : handlerId(0),
              instance(instanze)
            {
                theCallback = new RSS::CallbackImpl1<C>(c);
                signalConnect(instance, signal);
            }

            template <class C>
            ButtonCallback(gpointer instanze, const char* signal, C c, int)
            : handlerId(0),
              instance(instanze)
            {
                theCallback = new RSS::CallbackImpl2<C>(c);
                signalConnectWithEvent(instance, signal);
            };

            virtual ~ButtonCallback();

            inline void callBack() const {
                theCallback->doCallback();
            }

            inline void callBack(GdkEvent *event) const {
                theCallback->doCallback(event);
            }

            inline void signalConnect(gpointer instance, const gchar *signal) {
                handlerId = g_signal_connect_swapped(instance, signal, G_CALLBACK(gCallback), this);
            }
            inline void signalConnectWithEvent(gpointer instance, const gchar *signal) {
                handlerId = g_signal_connect_swapped(instance, signal, G_CALLBACK(gCallbackWithEvent), this);
            }

            static void gCallback(ButtonCallback *bcb);
            static void gCallbackWithEvent(ButtonCallback *bcb, GdkEvent *event);
    };
    typedef boost::shared_ptr<ButtonCallback>  ButtonCallbackPtr;
}

#endif /* __CALLBACK_HPP__ */
