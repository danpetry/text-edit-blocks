#include "noop.h"


Noop::Noop(){

    this->module_type = "noop";
    this->buf = "";
}

//
// Just loads the buffer, doing nothing to the input.
//
void Noop::process(){

    this->get_input(buf);
    DEBUG("Processing noop. buffer is: " + buf);
}
