##########################################################################
# Thor Project                                                    	     #
##########################################################################
cmake_minimum_required(VERSION 3.5)

project(Thor)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##########################################################################
# Version information                                                    #
##########################################################################
set(THOR_VERSION_MAJOR 0)
set(THOR_VERSION_MINOR 1)
set(THOR_VERSION_PATCH 0)
set(THOR_VERSION "${THOR_VERSION_MAJOR}.${THOR_VERSION_MINOR}.${THOR_VERSION_PATCH}")

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools)