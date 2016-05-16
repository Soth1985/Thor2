##########################################################################
# Thor Project                                                    	     #
##########################################################################
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(THOR_VERSION_MAJOR 0)
set(THOR_VERSION_MINOR 1)
set(THOR_VERSION_PATCH 0)
set(THOR_VERSION "${THOR_VERSION_MAJOR}.${THOR_VERSION_MINOR}.${THOR_VERSION_PATCH}")

set(THOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

project(Thor VERSION ${THOR_VERSION} LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools)

install(DIRECTORY ${THOR_INCLUDE_DIR}/Thor DESTINATION include)