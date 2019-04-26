//
// Echo class. Repeats input once.
//
// Inherits from Module class.
//

#ifndef ECHO_H
#define ECHO_H

#include "module.h"

class Echo: public Module{

    public:
        Echo();
        void process();  // Appends input onto itself.
};

#endif // ECHO_H
