project(hadoken)
cmake_minimum_required(VERSION 2.8)

enable_testing()

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")

option(UNIT_TESTS "Enable or disable unit tests execution" TRUE)
option(PERF_TESTS "Enable or disable performance tests execution" FALSE)
option(DISABLE_INSTALL "Disable the installation for this component" TRUE)
option(TEST_WITH_TBB "Use Intel TBB to test parallel algorithms" FALSE)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
      ${PROJECT_SOURCE_DIR}/CMake/portability
      ${PROJECT_SOURCE_DIR}/CMake/packages
      ${PROJECT_SOURCE_DIR}/CMake/config)

set(Boost_NO_BOOST_CMAKE TRUE)

include(BlueGenePortability)
include(DefineInstallationPaths)
include(ReleaseDebugAutoFlags)
include(DetectCXX11Support)


find_package(Threads)

if(TEST_WITH_TBB)
	find_package(TBB)
	include_directories(${TBB_INCLUDE_DIR})
endif()

if(BLUEGENE)
	# scheld_yield is to avoid on BGQ
	add_definitions(-DHADOKEN_SPIN_NO_YIELD )
endif()

file(GLOB HADOKEN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/hadoken/*)
file(GLOB HADOKEN_HEADERS_1 ${CMAKE_CURRENT_SOURCE_DIR}/include/hadoken/*/*)


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/  )
include_directories(SYSTEM ${BOOST_INCLUDE_DIR} )

if(NOT DISABLE_INSTALL)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/hadoken DESTINATION ${INCLUDE_INSTALL_DIR}/)
endif()

add_subdirectory(tests/perf)
add_subdirectory(tests/unit)

