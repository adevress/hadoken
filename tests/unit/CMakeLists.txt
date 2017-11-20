
if(HADOKEN_UNIT_TESTS)

find_package(Boost 1.41.0 QUIET REQUIRED system unit_test_framework)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} )

link_directories( ${Boost_LIBRARY_DIRS} )

if(NOT Boost_USE_STATIC_LIBS)
        add_definitions(-DBOOST_TEST_DYN_LINK=TRUE)
endif()



## generic Test
LIST(APPEND test_generic_src "test_generic.cpp")

add_executable(test_generic ${test_generic_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_generic ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_generic_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_generic)


## random generator related tests

LIST(APPEND test_random_src "test_random.cpp")

add_executable(test_random ${test_random_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_random ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} )

add_test(NAME test_random_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_random)


## crypto generator related tests

LIST(APPEND test_crypto_src "test_crypto.cpp")

add_executable(test_crypto ${test_crypto_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_crypto ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} )

add_test(NAME test_crypto_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_crypto)


##  C++11 only
if(CMAKE_CXX_SUPPORT_CXX11)

add_definitions(-std=c++11 )

## format related tests
LIST(APPEND test_format_src "test_format.cpp")

add_executable(test_format ${test_format_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_format ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_format_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_format)




## spmd related tests
LIST(APPEND test_spmd_src "test_spmd.cpp")

add_executable(test_spmd ${test_spmd_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_spmd ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
target_compile_options(test_spmd PRIVATE ${CMAKE_CXX_VECTORIZE} ${CMAKE_CXX_GEN_NATIVE} )
#target_compile_options(test_spmd PRIVATE ${CMAKE_CXX_VECTORIZE} -fopt-info-vec ${CMAKE_CXX_GEN_NATIVE} )


add_test(NAME test_spmd_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_spmd)



## geometry related tests
LIST(APPEND test_geometry_src "test_geometry.cpp")

add_executable(test_geometry ${test_geometry_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_geometry ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_geometry_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_geometry)


## containers Test
LIST(APPEND test_container_src "test_container.cpp")

add_executable(test_container ${test_container_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_container ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_container_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_container)


## thread Test
LIST(APPEND test_thread_src "test_thread.cpp")

add_executable(test_thread ${test_thread_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_thread ${CMAKE_THREAD_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_thread_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_thread)



## Parallel Test
LIST(APPEND test_parallel_src "test_parallel.cpp")

add_executable(test_parallel_base ${test_parallel_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_parallel_base ${CMAKE_THREAD_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_parallel_base_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_parallel_base)



# Network Test
LIST(APPEND test_network_src "test_network.cpp")

add_executable(test_network_base ${test_network_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_network_base ${CMAKE_THREAD_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_network_base_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_network_base)


endif()


endif()
