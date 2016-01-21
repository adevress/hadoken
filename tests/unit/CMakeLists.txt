

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS} )

link_directories( ${Boost_LIBRARY_DIRS} )


## generic Test
LIST(APPEND test_generic_src "test_generic.cpp")

add_executable(test_generic ${test_generic_src})
target_link_libraries(test_generic ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

add_test(NAME test_generic_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/test_generic)


## MpiRuntime Test
#LIST(APPEND MpiRuntime_src "MpiRuntime.cpp")

#add_executable(MpiRuntime ${MpiRuntime_src})
#target_link_libraries(MpiRuntime ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${MPI_LIBRARIES} functionalizer)

#add_test(NAME MpiTest_unit COMMAND ${TESTS_PREFIX} ${TESTS_PREFIX_ARGS} ${CMAKE_CURRENT_BINARY_DIR}/MpiRuntime)
