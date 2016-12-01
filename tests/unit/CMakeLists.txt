
if(UNIT_TESTS)

find_package(Boost 1.41.0 REQUIRED system unit_test_framework)

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




LIST(APPEND test_parallel_src "test_parallel.cpp")

add_executable(test_parallel_base ${test_parallel_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_parallel_base ${CMAKE_THREAD_LIBS_INIT}  ${CMAKE_parallel_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_parallel_base_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_parallel_base)

if(TBB_FOUND)

add_executable(test_parallel_tbb ${test_parallel_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_parallel_tbb  ${CMAKE_THREAD_LIBS_INIT} ${TBB_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
target_compile_definitions(test_parallel_tbb PRIVATE "-DHADOKEN_PARALLEL_USE_TBB=1")

add_test(NAME test_parallel_unit_tbb COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_parallel_tbb)


endif()

if(OPENMP_FOUND)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
add_executable(test_parallel_omp ${test_parallel_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_parallel_omp  ${CMAKE_THREAD_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
target_compile_definitions(test_parallel_omp PRIVATE "-DHADOKEN_PARALLEL_USE_OMP=1")
add_test(NAME test_parallel_unit_omp COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_parallel_omp)


endif()


if(TEST_WITH_GNU_PSTL AND OPENMP_FOUND)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
add_executable(test_parallel_gnu_pstl ${test_parallel_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(test_parallel_gnu_pstl  ${CMAKE_THREAD_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
target_compile_definitions(test_parallel_gnu_pstl PRIVATE "-DHADOKEN_PARALLEL_USE_GNU_PSTL=1")
add_test(NAME test_parallel_unit_gnu_pstl COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_parallel_gnu_pstl)


endif()


endif()



endif()
