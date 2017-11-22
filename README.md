
Hadoken

# Summmary

 Hadoken is a general purpose C++ Utility library. Hadoken is designed to complement and enhanced missing feature inside the STL and Boost.

- Hadoken does not have any other mandatory dependency than STL and Boost 
- Hadoken is header-only and designed to be embedded
- Hadoken follows STL/Boost code conventions
- Hadoken is under Boost License

# Features

## Random

 - Random123 implementation with counter based random generator, high quality, crush resistant, faster than mersenne twister
 - Abstract interface to allow runtime random generator selection, fully compatible with C++11 and Boost random distribution

## Format
 - Variadic template one-line string concatenator

## Strings
 - Utility function for string manipulation

## Network
 - Uri library
 


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
hadoken is free of charge and distributed under the Boost Software License 1.0 ( https://opensource.org/licenses/BSL-1.0 )




