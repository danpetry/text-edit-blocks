#include <algorithm>
#include <typeinfo>
#include "chain.h"
#include "echo.h"
#include "delay.h"
#include "reverse.h"

//
// Generates modules of various types.
//
template <typename t_module>
Module* creator() {

    return new t_module();
}


//
// Compares the first element of a vector of pairs.
//
std::string comparison_key;
bool isEqual(const std::pair<std::string, Module*>& element)
{

    return element.first ==  comparison_key;
}


//
// Adds a module to the chain.
//
return_te Chain::add_module(std::string name, std::string type){
         
    // Initialize the map of module factories. New module types need to be included here.
    this->factory_map["noop"] = creator<Noop>;
    this->factory_map["echo"] = creator<Echo>;
    this->factory_map["delay"] = creator<Delay>;
    this->factory_map["reverse"] = creator<Reverse>;

    // Check that the module type exists
    if (this->factory_map.find(type) == this->factory_map.end()){
        DEBUG("Module type not found");
        return ENOMODULETYPE;
    }

    // Check whether the module already exists in the module list
    comparison_key = name;
    auto module_from_it = std::find_if(this->named_module_list.begin(), this->named_module_list.end(), isEqual);
    if (module_from_it != this->named_module_list.end()){
        DEBUG("Module with name " + comparison_key + "already exists");
        return ENAMEEXISTS;
    }
    
    // Create the module
    Module* m = this->factory_map[type]();

    // If this is the first module, create a blank input module for it
    if(this->named_module_list.empty()){
        this->input_module_p = this->factory_map["noop"](); 
        m->create_input_conn(this->input_module_p);
    }

    // Push the module into the list of created modules
    this->new_named_module.first = name;
    this->new_named_module.second = m;
    this->output_module_p = &this->new_named_module;
    this->named_module_list.push_back(this->new_named_module);

    DEBUG("Module created with name " + this->named_module_list.back().first);
    return ESUCCESS;
}


//
// Connects two modules together.
//
return_te Chain::add_connection(std::string name_from, std::string name_to){
    
    // Check that modules have been created
    if(this->named_module_list.empty()){
        DEBUG("No modules created yet");
        return ENOMODULENAME;
    }

    // Check that name_from is in the module list, and get a reference to it if so
    comparison_key = name_from;
    auto module_from_it = std::find_if(this->named_module_list.begin(), this->named_module_list.end(), isEqual);
    if (module_from_it == this->named_module_list.end()){
        DEBUG("Module with name " + comparison_key + " does not exist");
        return ENOMODULENAME;
    }
    
    // Check that name_to is in the module list, and get a reference to it if so
    comparison_key = name_to;
    auto module_to_it = std::find_if(this->named_module_list.begin(), this->named_module_list.end(), isEqual);
    if (module_to_it == this->named_module_list.end()){
        DEBUG("Module with name " + comparison_key + " does not exist");
        return ENOMODULENAME;
    }

    // Create the connection
    module_to_it->second->create_input_conn(module_from_it->second);

    DEBUG("Connection created between " + name_from + " and " + name_to + ".");
    return ESUCCESS;
}


//
// Processes an input buffer.
//
return_te Chain::process(std::queue<std::string>& input_buf, std::queue<std::string>& output_buf){
    
    std::string output_word;

    // Check that modules have been created
    if(this->named_module_list.empty()){
        DEBUG("No modules created");
        return ENOMODULES;
    }

    // The above test should catch these, but to test explicitly...
    if((this->output_module_p == nullptr) || (this->input_module_p == nullptr)){
        DEBUG("No modules created");
        return ENOMODULES;
    } 
    if (this->output_module_p->second == nullptr ){
        DEBUG("No modules created");
        return ENOMODULES;
    }

    // Re-initialise the delays
    for (auto named_module_it : this->named_module_list){
        // This should have been caught above, but just in case...
        if (named_module_it.second == nullptr){
            DEBUG("Named module vector holding a deleted module");
            return EDELMODULE;
        }
        if (named_module_it.second->module_type == "delay") ((Delay*)named_module_it.second)->next = "hello";
    }
    
    int input_buf_sz = input_buf.size();

    // Process the input buffer 
    for(int i = 0; i < (input_buf_sz * CHAIN_MAX_OUTPUT_BUFFER_MULT); i++){

        // Insert the input string into the chain
        if (input_buf.size()){
            DEBUG("Inserting string into chain: " + input_buf.front() + ". Input buffer size is " + std::to_string(input_buf.size()) + ".");
            this->input_module_p->replace_buf(input_buf.front());
            input_buf.pop();
        }else{
            DEBUG("Empty input buffer. Clearing chain input buffer");
            this->input_module_p->clear_buf();
        }

        // Process the string through the chain
        for (auto named_module_it : this->named_module_list) named_module_it.second->process();
        
        // Get the chain's output
        if((output_word = this->output_module_p->second->output()).empty()){
            return ESUCCESS; // We've cleared the chain
        }
        else{
            output_buf.push(output_word);
        }
    }

    return EOUTOVFL;
}
