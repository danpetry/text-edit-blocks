//
// Chain class. Creates a chain of modules and process a string input through them.
//

#ifndef CHAIN_H
#define CHAIN_H

#include <string>
#include <map>
#include <queue>
#include <queue>
#include "module.h"
#include "noop.h"

typedef Module* (*creator_t)(void);
typedef std::pair<Module*, Module*> connection_t;
typedef std::pair<std::string, Module*> named_module_t;

typedef enum{
    EOUTOVFL      = -6,
    EDELMODULE    = -5,
    ENOMODULENAME = -4,
    ENAMEEXISTS   = -3,
    ENOMODULETYPE = -2,
    ENOMODULES    = -1,
    ESUCCESS      =  0
} return_te;

#define CHAIN_MAX_OUTPUT_BUFFER_MULT (16)

class Chain{

    public:
        return_te add_module(std::string name, std::string type);                                    // Adds a module to the chain.
        return_te add_connection(std::string name_from, std::string name_to);                        // Connects two modules together.
        return_te process(std::queue<std::string>& input_buf, std::queue<std::string>& output_buf);  // Processes an input buffer.
    private:
        Module* input_module_p;                                                                           // Input module, into which the buffer is fed.
        named_module_t* output_module_p;                                                             // Pointer to the last module in the chain.
        named_module_t new_named_module;                                                             // To construct a pair containing a module name and pointer to the module.
        std::map<std::string, creator_t> factory_map;                                                // A map of functions to create the different kinds of modules.
        std::vector<named_module_t> named_module_list;                                                // A list holding pairs of module names and pointers.
};

#endif // CHAIN_H
