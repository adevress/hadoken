project(hadoken)
cmake_minimum_required(VERSION 2.8)

enable_testing()

set(HADOKEN_VERSION_MAJOR "1")
set(HADOKEN_VERSION_MINOR "")
add_definitions( -DHADOKEN_VERSION_MAJOR=\"${HADOKEN_VERSION_MAJOR}\" -DHADOKEN_VERSION_MINOR=\"${HADOKEN_VERSION_MINOR}\")


option(HADOKEN_DISABLE_INSTALL "Disable the installation for this component" TRUE)
option(HADOKEN_UNIT_TESTS "Enable or disable unit tests execution" TRUE)
option(HADOKEN_PERF_TESTS "Enable or disable performance tests execution" FALSE)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
      ${PROJECT_SOURCE_DIR}/CMake/portability
      ${PROJECT_SOURCE_DIR}/CMake/packages
      ${PROJECT_SOURCE_DIR}/CMake/config)

set(Boost_NO_BOOST_CMAKE TRUE)

include(GNUInstallDirs)
include(ReleaseDebugAutoFlags)
include(DetectCXX11Support)


## Dependencies
find_package(Threads QUIET)


file(GLOB HADOKEN_HEADERS_1 "${CMAKE_CURRENT_SOURCE_DIR}/include/hadoken/*.hpp")
file(GLOB HADOKEN_HEADERS_2 "${CMAKE_CURRENT_SOURCE_DIR}/include/hadoken/*/*.hpp")


set(hadoken_version_src "${CMAKE_CURRENT_SOURCE_DIR}/src/hadoken_version.cpp")
add_executable(hadoken_version ${hadoken_version_src} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/  )
include_directories(SYSTEM ${BOOST_INCLUDE_DIR} )

if(NOT HADOKEN_DISABLE_INSTALL)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/hadoken DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/)
    install(TARGETS hadoken_version DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}/)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/pkgconfig/hadoken.pc" DESTINATION  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)
endif()

add_subdirectory(tests/perf)
add_subdirectory(tests/unit)

