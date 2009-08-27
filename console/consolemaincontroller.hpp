#ifndef __CONSOLE_MAINCONTROLLER_HPP__
#define __CONSOLE_MAINCONTROLLER_HPP__

#include "abstractmaincontroller.hpp"

namespace Console
{
    class MainController : public AbstractMainController
    {
        public:
            int init(int argc, char* argv[]);
            int deInit();
            
            int run();
    };
}

#endif /*  __CONSOLE_MAINCONTROLLER_HPP__ */
