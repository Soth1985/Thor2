project(MetalRenderer VERSION ${THOR_VERSION} LANGUAGES CXX)

set(MetalRenderer_SRC
    ThMetalContext.cpp
    ThMetalRenderer.cpp)

set(MetalRenderer_H
    ${THOR_INCLUDE_DIR}/Thor/MetalRenderer/ThMetalContext.h
    ${THOR_INCLUDE_DIR}/Thor/MetalRenderer/ThMetalRenderer.h)

list(SORT MetalRenderer_SRC)
list(SORT MetalRenderer_H)
add_library(MetalRenderer ${MetalRenderer_SRC} ${MetalRenderer_H})
set_target_properties (MetalRenderer PROPERTIES FOLDER Lib)
thor_add_precompiled_header(MetalRenderer "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(MetalRenderer)
thor_ios_target_setup_library(MetalRenderer)
thor_msvc_target_setup_library(MetalRenderer)

target_include_directories(MetalRenderer PUBLIC {$THOR_INCLUDE_DIR})
target_link_libraries(MetalRenderer Core MetalCpp)
