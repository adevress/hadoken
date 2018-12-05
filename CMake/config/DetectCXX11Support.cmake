# DetectCXX11Support.cmake
#
# Detect C++11 standard version support
#
# License: Boost Software License 1.0
#
# Copyright (c) 2018, Adrien Devresse
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)


include(CheckCXXSourceCompiles)


if(NOT DEFINED CMAKE_CXX_SUPPORT_CXX11)


FILE(READ "${CMAKE_CURRENT_LIST_DIR}/__cpp11_test_prog1.testsrc" _CXX_SUPPORT_TEST_SRC)

set(CMAKE_REQUIRED_FLAGS_OLD ${CMAKE_REQUIRED_FLAGS})
set(CMAKE_REQUIRED_FLAGS "-std=c++11")


CHECK_CXX_SOURCE_COMPILES("${_CXX_SUPPORT_TEST_SRC}" _CXX_SUPPORT_CXX11)


set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_OLD})

set(CMAKE_CXX_SUPPORT_CXX11 ${_CXX_SUPPORT_CXX11} CACHE BOOL "support for C++11")

if(CMAKE_CXX_SUPPORT_CXX11)
    message(STATUS "Compiler ${CMAKE_CXX_COMPILER_ID} supports C++11 ")
else()
    message(STATUS "Compiler ${CMAKE_CXX_COMPILER_ID} supports C++03")
endif()

endif()
