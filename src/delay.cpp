#include "delay.h"

Delay::Delay(){

    this->module_type = "delay";
    this->next = "hello";
    this->buf = "";
}


//
// Buffers input and delays it by one processing loop.
//
void Delay::process(){

    this->buf = this->next;
    this->get_input(this->next);
    DEBUG("Processing delay. Next string is: " + next + ", buffer is: " + this->buf);
}
