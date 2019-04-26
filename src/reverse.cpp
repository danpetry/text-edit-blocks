#include "reverse.h"
#include <algorithm>


Reverse::Reverse(){

    this->module_type = "reverse";
    this->buf = "";
}


//
// Reverses the input.
//
void Reverse::process(){

    this->get_input(buf);

    std::reverse(this->buf.begin(), this->buf.end());
    DEBUG("Processing reverse. buffer is: " + this->buf);
}
