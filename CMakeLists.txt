cmake_minimum_required (VERSION 3.4)

project (dashmm)

set (default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message (STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set (CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
endif()

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -g")
set (CMAKE_CXX_FLAGS_RELEASE "-Wall -O3")

find_package (PkgConfig)
pkg_search_module (HPX REQUIRED hpx)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(demo)
add_subdirectory(test)
