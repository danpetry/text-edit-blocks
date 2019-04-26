//
// Noop class. Does nothing to text and just passes it on to the next module in the chain.
//
// Inherits from Module class.
//

#ifndef NOOP_H
#define NOOP_H

#include "module.h"

class Noop: public Module{

    public:
        Noop();
        void process();   // Does nothing to the input.
};

#endif //NOOP_H
