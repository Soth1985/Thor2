##########################################################################
# Thor Project                                                    	     #
##########################################################################
cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(THOR_VERSION_MAJOR 0)
set(THOR_VERSION_MINOR 2)
set(THOR_VERSION_PATCH 0)
set(THOR_VERSION "${THOR_VERSION_MAJOR}.${THOR_VERSION_MINOR}.${THOR_VERSION_PATCH}")

set(THOR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(THOR_SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(THOR_APPLICATION_DIR ${CMAKE_SOURCE_DIR}/applications)
set(THOR_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/cmake/scripts)

#set(CMAKE_GENERATOR_TOOLSET "ClangCL")

project(Thor VERSION ${THOR_VERSION} LANGUAGES CXX)

if (APPLE)
    if (NOT IOS)
        set(MACOS TRUE)
    endif()
endif()

if (MSVC)
	option(THOR_MSVC_STATIC_RUNTIME "Link c runtime statically." TRUE)
    option(THOR_MSVC_CLANG "Use clang toolset." TRUE)
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
include(import_ktx)

if (MSVC)
    include(import_dispatch)
endif()

if (THOR_MSVC_CLANG)
    include(ms_clang_scripts)
else()
    include(msvc_scripts)
endif()

include(xcode_ios_scripts)
include(xcode_macos_scripts)

#project modules
add_subdirectory(lib)

if (NOT IOS)
    add_subdirectory(tests)
endif()

add_subdirectory(applications)

install(DIRECTORY ${THOR_INCLUDE_DIR}/Thor DESTINATION include)