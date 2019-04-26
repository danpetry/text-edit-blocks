#include "module.h"


//
// Registers input modules into the list of inputs.
//
void Module::create_input_conn(Module* input_module_p){

    this->input_list.push_back(input_module_p);
}


//
// Fills the internal buffer via callbacks to input modules' output() methods.
//
void Module::get_input(std::string& buf_to_fill){

    for (auto input_it : this->input_list){
        std::string input_str = input_it->output();
        DEBUG("Input string: " + input_str);
        if ((input_it == *(input_list.begin())) || buf_to_fill.empty()) buf_to_fill = input_str;
        else if (!input_str.empty()) buf_to_fill.append(input_str);
    }
}


//
// Replaces the contents of the internal buffer.
//
void Module::replace_buf(std::string& buf_to_insert){

    this->buf = buf_to_insert;
}


//
// Clears the contents of the internal buffer.
//
void Module::clear_buf(){

    this->buf = "";
}


//
// Delivers the contents of the internal buffer.
//
std::string Module::output(){

    return this->buf;
}
