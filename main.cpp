#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include "chain.h"


//
// Main - Commnand line program. Compiled to main_app.
//
// This parses command line input and calls the appropriate methods
// in the Chain class, with the appropriate arguments.
//
int main(int argc, char *argv[]){

    Chain my_module_chain;
    std::string cmd;
    int r;

    while(true)
    {
        std::string cmd_line_input_str;

        /* get the line from stdin */
        getline(std::cin, cmd_line_input_str);
        std::stringstream iss(cmd_line_input_str);
        iss >> cmd;

        // Module command handler
        if (!cmd.compare("module"))
        {
            std::string name, type;
            if (!(iss >> name)){
                std::cout << "Usage: module [name] [type]" << std::endl;
            }
            else if (!(iss >> type)){
                std::cout << "Usage: module [name] [type]" << std::endl;
            }
            else if (iss.rdbuf()->in_avail() != 0){
                std::cout << "Usage: module [name] [type]" << std::endl;
            }
            else if ((r = my_module_chain.add_module(name, type)) < 0){
                std::cout << "Input error" << std::endl;
            }
            else{
                std::cout << "Module created" << std::endl;
            }
        }

        // Connect command handler 
        else if (!cmd.compare("connect"))
        {
            
            std::string name_from, name_to;

            if (!(iss >> name_from)){
                std::cout << "Usage: connect [from] [to]" << std::endl;
            }
            else if (!(iss >> name_to)){
                std::cout << "Usage: connect [from] [to]" << std::endl;
            }
            else if (iss.rdbuf()->in_avail() != 0){
                std::cout << "Usage: connect [from] [to]" << std::endl;
            }
            else if ((r = my_module_chain.add_connection(name_from, name_to)) < 0){
                std::cout << "Input error" << std::endl;
            }
            else{
                std::cout << "Connection created" << std::endl;
            }
        }
        
        // Process command handler 
        else if (!cmd.compare("process"))
        {
            std::string word;
            std::queue<std::string> text_input;
            
            while (iss >> word) text_input.push(word);
            std::queue<std::string> text_output;
            if((r = my_module_chain.process(text_input, text_output)) < 0){
                std::cout << "Input error" << std::endl;
            }
            else{
                while (!text_output.empty()){
                    std::cout << text_output.front() << " ";
                    text_output.pop();
                }
                std::cout << std::endl;
            }
        }
        else{
            std::cout << "Usage: [ module | connect | process ] [args]" << std::endl;
        }
    }

    return 0;
}
