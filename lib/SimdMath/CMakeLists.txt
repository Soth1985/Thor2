project(SimdMath VERSION ${THOR_VERSION} LANGUAGES CXX)

set(SimdMath_SRC
    ThGraphics.cpp)

set(SimdMath_H
    ${THOR_INCLUDE_DIR}/Thor/SimdMath/Simd.h
    ${THOR_INCLUDE_DIR}/Thor/SimdMath/ThGraphics.h)

list(SORT SimdMath_SRC)
list(SORT SimdMath_H)
add_library(SimdMath ${SimdMath_SRC} ${SimdMath_H})
set_target_properties (SimdMath PROPERTIES FOLDER Lib)
thor_add_precompiled_header(Math "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(SimdMath)
thor_ios_target_setup_library(SimdMath)
thor_msvc_target_setup_library(SimdMath)

target_include_directories(SimdMath PUBLIC {$THOR_INCLUDE_DIR})
target_link_libraries(SimdMath Core)