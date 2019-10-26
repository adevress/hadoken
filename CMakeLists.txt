project(hadoken)
cmake_minimum_required(VERSION 3.1)

enable_testing()

set(HADOKEN_VERSION_MAJOR "1")
set(HADOKEN_VERSION_MINOR "0")
add_definitions( -DHADOKEN_VERSION_MAJOR=\"${HADOKEN_VERSION_MAJOR}\" -DHADOKEN_VERSION_MINOR=\"${HADOKEN_VERSION_MINOR}\")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CUR_PROJ ON)
else()
  set(CUR_PROJ OFF)
endif()

include(CMakeDependentOption)
cmake_dependent_option(HADOKEN_DISABLE_INSTALL "Disable the installation for this component" ON "CUR_PROJ" ON)
cmake_dependent_option(HADOKEN_UNIT_TESTS "Enable or disable unit tests execution" ON "CUR_PROJ" OFF)
cmake_dependent_option(HADOKEN_PERF_TESTS "Enable or disable performance tests execution" OFF "CUR_PROJ" OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
      ${PROJECT_SOURCE_DIR}/CMake/portability
      ${PROJECT_SOURCE_DIR}/CMake/packages
      ${PROJECT_SOURCE_DIR}/CMake/config)

set(Boost_NO_BOOST_CMAKE TRUE)

include(GNUInstallDirs)
include(ReleaseDebugAutoFlags)

## Enforce CXX standard
set (CMAKE_CXX_STANDARD 11)

## Dependencies
find_package(Threads QUIET)


file(GLOB HADOKEN_HEADERS_1 "${CMAKE_CURRENT_SOURCE_DIR}/include/hadoken/*.hpp")
file(GLOB HADOKEN_HEADERS_2 "${CMAKE_CURRENT_SOURCE_DIR}/include/hadoken/*/*.hpp")


set(hadoken_version_src "${CMAKE_CURRENT_SOURCE_DIR}/src/hadoken_version.cpp")
add_executable(hadoken ${hadoken_version_src} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/  )
include_directories(SYSTEM ${BOOST_INCLUDE_DIR} )

if(NOT HADOKEN_DISABLE_INSTALL)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/hadoken DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/)
    install(TARGETS hadoken DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}/)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/pkgconfig/hadoken.pc" DESTINATION  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)
endif()

add_subdirectory(tests/perf)
add_subdirectory(tests/unit)

