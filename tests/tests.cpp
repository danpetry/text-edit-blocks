#include <gtest/gtest.h>
#include <string>
#include <queue>
#include <vector>
#include <iostream>
#include "chain.h"

#define GTEST_BREAK_ON_FAILURE = 0


//
// Tests basic module functionality
//
TEST(simple_module_chain, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
    std::queue<std::string> expected; 
  

    // Test basic behaviour
    std::vector<std::string> input_strings = {"hello", "world"};
    std::vector<std::string> expected_strings = {"hello", "olleh", "dlrow"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    ASSERT_EQ(my_module_chain.add_module        ("alpha", "reverse"),      ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("beta", "delay"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("alpha", "beta"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ESUCCESS);

    EXPECT_EQ(output_buf, expected);
    

    // Test idempotency
    while (output_buf.size()) output_buf.pop();
    while (expected.size()) expected.pop();

    input_strings = {"foo", "bar"};
    expected_strings = {"hello", "oof", "rab"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ESUCCESS);

    EXPECT_EQ(output_buf, expected);


    // Test extendability of chain
    while (output_buf.size()) output_buf.pop();
    while (expected.size()) expected.pop();

    input_strings = {"hello", "world"};
    expected_strings = {"hellohello", "olleholleh", "dlrowdlrow"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    ASSERT_EQ(my_module_chain.add_module        ("gamma", "echo"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("delta", "noop"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("beta", "gamma"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("gamma", "delta"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ESUCCESS);

    EXPECT_EQ(output_buf, expected);
}


//
// Tests error output of modules 
//
TEST(errors, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
  
    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ENOMODULES   );
    ASSERT_EQ(my_module_chain.add_module        ("alpha", "foo"),          ENOMODULETYPE);
    ASSERT_EQ(my_module_chain.add_module        ("alpha", "noop"),         ESUCCESS     );
    ASSERT_EQ(my_module_chain.add_connection    ("alpha", "bar"),          ENOMODULENAME); 
    ASSERT_EQ(my_module_chain.add_module        ("alpha", "noop"),         ENAMEEXISTS  );
}


//
// Tests multiple delays
//
TEST(multi_delay, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
    std::queue<std::string> expected; 
  
    std::vector<std::string> input_strings = {"foo", "bar"};
    std::vector<std::string> expected_strings = {"hello", "hello", "hello", "hello", "foo", "bar"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    std::string module_name = "m";
    int number_of_delays = 4;

    for (int i = 1; i <= number_of_delays; i++){
        ASSERT_EQ(my_module_chain.add_module(module_name + std::to_string(i), "delay"), ESUCCESS);
    }

    for (int i = 1; i < number_of_delays; i++){
        ASSERT_EQ(my_module_chain.add_connection(module_name + std::to_string(i), module_name + std::to_string(i+1)), ESUCCESS);
    }
    
    ASSERT_EQ(my_module_chain.process(input_buf, output_buf), ESUCCESS);

    EXPECT_EQ(output_buf, expected);
}


//
// Tests behaviour when output is more than 16 times input
//
TEST(multi_delay_overflow, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
    std::queue<std::string> expected; 
  
    std::vector<std::string> input_strings = {"foo"};
    std::vector<std::string> expected_strings = {"hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    std::string module_name = "m";
    int number_of_delays = 16;

    for (int i = 1; i <= number_of_delays; i++){
        ASSERT_EQ(my_module_chain.add_module(module_name + std::to_string(i), "delay"), ESUCCESS);
    }

    for (int i = 1; i < number_of_delays; i++){
        ASSERT_EQ(my_module_chain.add_connection(module_name + std::to_string(i), module_name + std::to_string(i+1)), ESUCCESS);
    }
    
    ASSERT_EQ(my_module_chain.process(input_buf, output_buf), EOUTOVFL);

    EXPECT_EQ(output_buf, expected);
}


//
// Tests branching and summing in the following configuration:
// 
//      /- delay -\
// echo-           -noop
//      \-reverse-/
//
// delay-noop connected before reverse-noop.
//
TEST(branch_and_sum, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
    std::queue<std::string> expected; 
  
    std::vector<std::string> input_strings = {"hello", "world"};
    std::vector<std::string> expected_strings = {"helloolleholleh", "hellohellodlrowdlrow", "worldworld"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);
    
    ASSERT_EQ(my_module_chain.add_module        ("alpha", "echo"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("beta", "delay"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("gamma", "reverse"),      ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("delta", "noop"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("alpha", "beta"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("alpha", "gamma"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("beta", "delta"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("gamma", "delta"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ESUCCESS);

    EXPECT_EQ(output_buf, expected);
}


//
// Tests branching and summing in the following configuration:
// 
//      /- delay -\
// echo-           -noop
//      \-reverse-/
//
// reverse-noop connected before delay-noop.
//
TEST(branch_and_sum_2, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
    std::queue<std::string> expected; 
  
    std::vector<std::string> input_strings = {"hello", "world"};
    std::vector<std::string> expected_strings = {"ollehollehhello", "dlrowdlrowhellohello", "worldworld"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    ASSERT_EQ(my_module_chain.add_module        ("alpha", "echo"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("beta", "delay"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("gamma", "reverse"),      ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("delta", "noop"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("alpha", "beta"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("alpha", "gamma"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("gamma", "delta"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("beta", "delta"),         ESUCCESS);
    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ESUCCESS);

    EXPECT_EQ(output_buf, expected);
}


//
// Tests branching and summing with different number modules on each branch,
// in the following configuration:
//
//      /-echo-echo-\
// noop-             -noop
//      \- reverse -/
//
// echo-noop connected before reverse-noop.
// middle modules defined in order echo-reverse-echo.
//
TEST(branch_and_sum_3, chain_tests){

    Chain my_module_chain;
    std::queue<std::string> input_buf;
    std::queue<std::string> output_buf;
    std::queue<std::string> expected; 
  
    std::vector<std::string> input_strings = {"hello", "world"};
    std::vector<std::string> expected_strings = {"hellohellohellohelloolleh", "worldworldworldworlddlrow"};
    for (auto it : input_strings) input_buf.push(it);
    for (auto it : expected_strings) expected.push(it);

    ASSERT_EQ(my_module_chain.add_module        ("noop_start", "noop"),    ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("echo_1", "echo"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("reverse", "reverse"),    ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("echo_2", "echo"),        ESUCCESS);
    ASSERT_EQ(my_module_chain.add_module        ("noop_end", "noop"),      ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("noop_start", "echo_1"),  ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("noop_start", "reverse"), ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("echo_1", "echo_2"),      ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("echo_2", "noop_end"),    ESUCCESS);
    ASSERT_EQ(my_module_chain.add_connection    ("reverse", "noop_end"),   ESUCCESS);
    ASSERT_EQ(my_module_chain.process           (input_buf, output_buf),   ESUCCESS);
    EXPECT_EQ(output_buf, expected);
}
