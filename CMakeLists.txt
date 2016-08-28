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
set(THOR_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
set(CMAKE_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/cmake/Scripts)

project(Thor VERSION ${THOR_VERSION} LANGUAGES CXX)

include(ImportedTargetsNix)

function (thor_target_cpp_std target std)
    if(APPLE)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD ${std})
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    else()
        target_compile_options(${target} PRIVATE -std=${std} -stdlib=libc++)
    endif()
endfunction(thor_target_cpp_std)

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools)
add_subdirectory(templates)

install(DIRECTORY ${THOR_INCLUDE_DIR}/Thor DESTINATION include)