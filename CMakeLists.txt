##########################################################################
# Thor Project                                                    	     #
##########################################################################
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(THOR_VERSION_MAJOR 0)
set(THOR_VERSION_MINOR 1)
set(THOR_VERSION_PATCH 0)
set(THOR_VERSION "${THOR_VERSION_MAJOR}.${THOR_VERSION_MINOR}.${THOR_VERSION_PATCH}")

set(THOR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(THOR_SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(THOR_APPLICATION_DIR ${CMAKE_SOURCE_DIR}/applications)
set(THOR_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/cmake/scripts)

project(Thor VERSION ${THOR_VERSION} LANGUAGES CXX)

if (APPLE)
    if (NOT IOS)
        set(MACOS TRUE)
    endif()
endif()

if (MSVC)
	option(THOR_MSVC_STATIC_RUNTIME "Link c runtime statically." TRUE)
endif()

if (IOS)
    set(THOR_IOS_CODE_SIGN_IDENTITY "iPhone Developer" CACHE STRING "iOS signing identity." )
    set(THOR_IOS_DEVELOPMENT_TEAM "2PKXU69355" CACHE STRING "iOS development team.")
endif()

if (MACOS)
    set(THOR_MACOS_CODE_SIGN_IDENTITY "Mac Developer" CACHE STRING "macOS signing identity.")
    set(THOR_MACOS_DEVELOPMENT_TEAM "2PKXU69355" CACHE STRING "macOS development team.")
endif()

#extra cmake scripts
include(thor_utils)
include(msvc_scripts)
include(xcode_ios_scripts)
include(xcode_macos_scripts)

#imported/prebuilt libraries
include(import_bass)

#project modules
add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools)
add_subdirectory(templates)
add_subdirectory(applications)

#build external libs
add_subdirectory(submodules)

install(DIRECTORY ${THOR_INCLUDE_DIR}/Thor DESTINATION include)