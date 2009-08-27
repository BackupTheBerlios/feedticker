#ifndef __ABSTRACT_MAINCONTROLLER_HPP__
#define __ABSTRACT_MAINCONTROLLER_HPP__

namespace Abstract
{
    class MainController
    {
        public:
            int init(int argc, char* argv[]) = 0;
            int deInit() = 0;
            
            int run() = 0;
    };
}

#endif /*  __ABSTRACT_MAINCONTROLLER_HPP__ */
