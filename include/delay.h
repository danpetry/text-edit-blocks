//
// Delay class. Delays input by one processing loop.
//
// Inherits from Module class.
//

#ifndef DELAY_H
#define DELAY_H

#include "module.h"

class Delay: public Module{

    public:
        Delay();
        void process();            // Buffers input and delays it by one processing loop.
        std::string next;          // Contains the next output.
    private:
};

#endif //DELAY_H
