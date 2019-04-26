//
// Module base class. Other module types are derived from it.
//

#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>
#include <iostream>

#ifndef DEBUG_EN
#define DEBUG_EN (false)
#endif

#define DEBUG(x) do { \
      if (DEBUG_EN) { std::cerr << x << std::endl; } \
} while (0)


class Module{

     public:
         virtual ~Module() = default;
         virtual void process() = 0;                        // Overridden with each module's individual processing method.
         void create_input_conn(Module* input_module_p);    // Registers input modules.
         void get_input(std::string& buf_to_fill);          // Fills the internal buffer via callbacks to input modules' output() methods.
         void replace_buf(std::string& buf_to_insert);      // Replaces the contents of the internal buffer. 
         void clear_buf();                                  // Clears the contents of the internal buffer.
         std::string output();                              // Delivers the contents of the internal buffer.
         std::string module_type;                           // Module type name, used for type-specific operations, e.g. re-initialising delays.
     protected:
         std::string buf;                                   // The internal buffer.
         std::vector<Module*> input_list;                   // List of input modules.
};

#endif //ndef MODULE_H
