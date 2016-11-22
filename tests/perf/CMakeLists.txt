
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

## small vector perf test
LIST(APPEND small_vector_perf_src "small_vector_perf.cpp")

add_definitions( -std=c++11 )

add_executable(small_vector_perf ${small_vector_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(small_vector_perf ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})


## lock / spinlock perf test
LIST(APPEND lock_perf_src "lock_perf.cpp")

add_definitions( -std=c++11 )

add_executable(lock_perf ${lock_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(lock_perf ${CMAKE_THREAD_LIBS_INIT}  ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})


## executors perf test
LIST(APPEND executor_perf_src "executor_perf.cpp")

add_executable(executor_perf ${executor_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(executor_perf ${CMAKE_THREAD_LIBS_INIT}  ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})

## parallel perf test
LIST(APPEND parallel_perf_src "parallel_perf.cpp")

add_executable(parallel_perf ${parallel_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(parallel_perf ${CMAKE_THREAD_LIBS_INIT}  ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})

if(TBB_FOUND)
add_executable(parallel_perf_tbb ${parallel_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
target_link_libraries(parallel_perf_tbb ${CMAKE_THREAD_LIBS_INIT} ${TBB_LIBRARIES} ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})
target_compile_definitions(parallel_perf_tbb PRIVATE "-DHADOKEN_PARALLEL_USE_TBB=1")
endif()

if(OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    add_executable(parallel_perf_omp ${parallel_perf_src} ${HADOKEN_HEADERS} ${HADOKEN_HEADERS_1})
    target_link_libraries(parallel_perf_omp ${CMAKE_THREAD_LIBS_INIT} ${Boost_CHRONO_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})
endif()

endif()


endif()
