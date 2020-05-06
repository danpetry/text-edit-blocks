INTRODUCTION
------------

This program takes command line input and allows the user to create a chain of string
processing modules, and then process a string through it.

The processing modules inherit from a Module base class, and a Chain class
holds the state of the chain, including a vector of all existing modules.
The inputs are stored in each module as a list of references to the input
modules. When each module's process() method is called, the module gets the
contents of the input modules' buffers in turn, summing them if there are
more than one.

The Chain class implements all methods to access and control the processing
chain, including methods to add modules, create connections and process FIFO
string buffers.

This package includes the command line application and a series of tests.


ASSUMPTIONS
-----------

It has been assumed that modules are chained in order of their definition:
input modules are always defined before the modules they are feeding into.
This is reasonable given the task description, which requires that the input to
the chain is the first defined module, and the output is the last.

Intermediate modules could have been defined in any order. If this assumption
is not made, one solution could have been to process all modules and then all
connections, shifting the processed text along the chain. This would have come
at the expense of latency, however.


REQUIREMENTS
------------

Cmake v2.8.9 minimum and a Cmake compatible build environment such as make
(Linux), Visual Studio (Windows) or Xcode (Mac).


BUILD AND RUN
-------------

1. Download Cmake from https://cmake.org/download/

2. Follow platform specific instructions for building the project. For
example:

Linux:

```
cmake ./
make
./main_app or ./test_app
```

Windows:

In Visual Studio 2015 onwards, there is support for Cmake natively within the
IDE. Please see
https://docs.microsoft.com/en-us/cpp/ide/cmake-tools-for-visual-cpp?view=vs-2017.

In previous versions, VS, NMake and Borland build and project files can be
generated using the CMake GUI. See https://cmake.org/runningcmake/ for further
details.


FURTHER WORK
------------

This is based on a vector of module pointers. The pointers are simple C style
pointers. The vector is run through, and
the outputs for each module are generated in turn and consumed by the next
module. Each module contains a pointer to its input modules, which allows it
to get their outputs.

These modules are created in a factory function but never deleted, which leaks
memory.

Improvements might be:
  - Use std::unique_ptr's to point to the modules, so they automatically get
    deleted at the end of the program
  - Have a vector of input connections in each module. Each connection is
    represented by an index of an input module. Since you can access each
    module by index (because they're stored in a vector), getting each input is
    an O(1) operation.
