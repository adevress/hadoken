
if(PERF_TESTS)

find_package(Boost 1.41.0 REQUIRED system chrono)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS} )

link_directories( ${Boost_LIBRARY_DIRS} )





## random generator related tests

LIST(APPEND random_perf_src "random_perf.cpp")

add_executable(random_perf ${random_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(random_perf ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})


LIST(APPEND derivate_random_perf_src "random_perf_derivate.cpp")

add_executable(derivate_random_perf ${derivate_random_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(derivate_random_perf ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})



if(CMAKE_CXX_SUPPORT_CXX11)

LIST(APPEND small_vector_perf_src "small_vector_perf.cpp")

add_definitions( -std=c++11 )

add_executable(small_vector_perf ${small_vector_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(small_vector_perf ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})

endif()


endif()
