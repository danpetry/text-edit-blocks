#include "echo.h"

Echo::Echo(){

    this->module_type = "echo";
    this->buf = "";
}


//
// Appends input onto itself.
//
void Echo::process(){

    this->get_input(this->buf);
    if (!this->buf.empty()) this->buf.append(this->buf);
    DEBUG("Processing echo. buffer is: " + this->buf);
}
