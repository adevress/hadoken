
[![Build Status](https://travis-ci.org/adevress/hadoken.svg?branch=master)](https://travis-ci.org/adevress/hadoken)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9377f9f6f3dc45eba06982192c853ac7)](https://app.codacy.com/app/adevress/hadoken?utm_source=github.com&utm_medium=referral&utm_content=adevress/hadoken&utm_campaign=Badge_Grade_Dashboard)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/adevress/hadoken.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/adevress/hadoken/context:cpp)
      _    _               _           _                   
     | |  | |             | |         | |                  
     | |__| |   __ _    __| |   ___   | | __   ___   _ __  
     |  __  |  / _` |  / _` |  / _ \  | |/ /  / _ \ | '_ \ 
     | |  | | | (_| | | (_| | | (_) | |   <  |  __/ | | | |
     |_|  |_|  \__,_|  \__,_|  \___/  |_|\_\  \___| |_| |_|
                                                       
                                                       


# Summmary

 Hadoken is Minimalist, Simple, Stupid, C++ Utility Library. Hadoken is designed to complement and enhanced missing feature inside the STL and Boost.

- Hadoken does not have any other mandatory dependency than STL and Boost 
- Hadoken is header-only and designed to be embedded
- Hadoken follows STL/Boost code conventions
- Hadoken is under Boost License

# Features

## Random

 - [Random123](https://www.deshawresearch.com/resources_random123.html) implementation of counter based random generator, high quality, crush resistant and faster than mersenne twister
 - Abstract interface to allow runtime random generator selection, fully compatible with C++11 and Boost random distribution

## Format
 - Variadic template one-line string concatenator

## Strings / Format
 - Utility function for one line string concatenation
 - Utility function for wildcard matching
 - Utility function for template substitution ( mustache style, minimalist ) 

## Network
 - Uri library

## Numeric
 - Float equality / close to functions
 - string to integral / float converters 

## Containers
 - small_vector: Vector with small size optimization. In the spirit of [LLVM small vector](http://llvm.org/doxygen/classllvm_1_1SmallVector.html)

## Parallel
 - Partial C++17 Parallel STL implementation compatible with C++11

## Thread
 - spinlock: simple implementation
 - latch: barrier with counter implementation

## Executors
 - C++ 20 Executors implementations
 - Thread pool executor
 - Single thread executor
 
## State Machine
 - Simple, type-safe, callback based Finite State Machine (FSM) implementation

## Parse opts / getopts
 - Command line option parser
 - Support sub-commands
 - Support auto-help generation
 - Sax style parsing

# How to use
Hadoken is mainly header only, you can use one of this

1- include the hadoken/include/ directory in your project as submodule

2- run the following command to install it
mkdir build
pushd build
cmake  ../
make
make install


# LICENSE
Hadoken is free of charge and distributed under the [Boost Software License 1.0](https://opensource.org/licenses/BSL-1.0)




