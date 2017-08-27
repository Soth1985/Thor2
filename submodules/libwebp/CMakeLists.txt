project(libwebp C)

# Options for coder / decoder executables.
option(WEBP_BUILD_CWEBP "Build the cwebp command line tool." OFF)
option(WEBP_BUILD_DWEBP "Build the dwebp command line tool." OFF)
option(WEBP_BUILD_GIF2WEBP "Build the gif2webp conversion tool." OFF)
option(WEBP_BUILD_IMG2WEBP "Build the img2webp animation tool." OFF)
option(WEBP_EXPERIMENTAL_FEATURES "Build with experimental features." OFF)
option(WEBP_ENABLE_SWAP_16BIT_CSP "Enable byte swap for 16 bit colorspaces." OFF)

set(WEBP_DEP_LIBRARIES)
set(WEBP_DEP_INCLUDE_DIRS)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE
    "Build type: Release, Debug or RelWithDebInfo" STRING FORCE
  )
endif()

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/include/webp/config.h)
  message(STATUS "WebP config.h is already cached at ${CMAKE_CURRENT_BINARY_DIR}/include/webp")
else()
  include(cmake/config.h.cmake)
endif()

################################################################################
# Options.
if(WEBP_ENABLE_SWAP_16BIT_CSP)
  add_definitions(-DWEBP_SWAP_16BIT_CSP)
endif()

################################################################################
# Android only.
if(ANDROID)
  include_directories(${ANDROID_NDK}/sources/android/cpufeatures)
  add_library(cpufeatures STATIC
    ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
  )
  target_link_libraries(cpufeatures dl)
  set(WEBP_DEP_LIBRARIES ${WEBP_DEP_LIBRARIES} cpufeatures)
  set(WEBP_DEP_INCLUDE_DIRS ${WEBP_DEP_INCLUDE_DIRS}
    ${ANDROID_NDK}/sources/android/cpufeatures
  )
endif()

################################################################################
# WebP source files.
# Read the Makefile.am to get the source files.

function(parse_Makefile_am FOLDER VAR)
  file(READ ${FOLDER}/Makefile.am MAKEFILE_AM)
  string(REGEX MATCHALL "_SOURCES \\+= [^\n]*"
    FILES_PER_LINE ${MAKEFILE_AM}
  )
  set(SRCS ${${VAR}})
  foreach(FILES ${FILES_PER_LINE})
    string(SUBSTRING ${FILES} 12 -1 FILES)
    string(REGEX MATCHALL "[0-9a-z\\._]+"
      FILES ${FILES}
    )
    foreach(FILE ${FILES})
      list(APPEND SRCS ${FOLDER}/${FILE})
    endforeach()
  endforeach()
  set(${VAR} ${SRCS} PARENT_SCOPE)
endfunction()

set(WEBP_SRCS)
parse_Makefile_am(${CMAKE_CURRENT_SOURCE_DIR}/src/dec "WEBP_SRCS")
parse_Makefile_am(${CMAKE_CURRENT_SOURCE_DIR}/src/demux "WEBP_SRCS")
parse_Makefile_am(${CMAKE_CURRENT_SOURCE_DIR}/src/dsp "WEBP_SRCS")
parse_Makefile_am(${CMAKE_CURRENT_SOURCE_DIR}/src/enc "WEBP_SRCS")
parse_Makefile_am(${CMAKE_CURRENT_SOURCE_DIR}/src/utils "WEBP_SRCS")

# Remove the files specific to SIMD we don't use.
foreach(FILE ${WEBP_SIMD_FILES_NOT_TO_INCLUDE})
  list(REMOVE_ITEM WEBP_SRCS ${FILE})
endforeach()

# Build the library.
add_definitions(-Wall)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${WEBP_DEP_INCLUDE_DIRS})
add_library(webp ${WEBP_SRCS})
target_link_libraries(webp ${WEBP_DEP_LIBRARIES})
thor_macos_target_setup_library(webp)
thor_ios_target_setup_library(webp)
set_target_properties (webp PROPERTIES FOLDER Submodules)

# Change the compile flags for SIMD files we use.
list(LENGTH WEBP_SIMD_FILES_TO_INCLUDE WEBP_SIMD_FILES_TO_INCLUDE_LENGTH)
math(EXPR WEBP_SIMD_FILES_TO_INCLUDE_RANGE
  "${WEBP_SIMD_FILES_TO_INCLUDE_LENGTH}-1"
)

foreach(I_FILE RANGE ${WEBP_SIMD_FILES_TO_INCLUDE_RANGE})
  list(GET WEBP_SIMD_FILES_TO_INCLUDE ${I_FILE} FILE)
  list(GET WEBP_SIMD_FLAGS_TO_INCLUDE ${I_FILE} SIMD_COMPILE_FLAG)
  set_source_files_properties(${FILE} PROPERTIES
    COMPILE_FLAGS ${SIMD_COMPILE_FLAG}
  )
endforeach()

set(imageioutil_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/imageio_util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/imageio_util.h)
    add_library(imageioutil ${imageioutil_SRCS})
thor_macos_target_setup_library(imageioutil)
thor_ios_target_setup_library(imageioutil)
set_target_properties (imageioutil PROPERTIES FOLDER Submodules)
target_link_libraries(imageioutil ${WEBP_DEP_LIBRARIES})

# Image-decoding utility library.
set(imagedec_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/gifdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/gifdec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/image_dec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/image_dec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/jpegdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/jpegdec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/metadata.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/metadata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/pngdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/pngdec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/tiffdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/tiffdec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/webpdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/webpdec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/wicdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/imageio/wicdec.h)
add_library(imagedec ${imagedec_SRCS})
thor_macos_target_setup_library(imagedec)
thor_ios_target_setup_library(imagedec)
set_target_properties (imagedec PROPERTIES FOLDER Submodules)
target_link_libraries(imagedec webp ${WEBP_DEP_LIBRARIES})

# Image-encoding utility library.
set(imageenc_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/imageio/image_enc.c
${CMAKE_CURRENT_SOURCE_DIR}/imageio/image_enc.h)
add_library(imageenc ${imageenc_SRCS})
thor_macos_target_setup_library(imageenc)
thor_ios_target_setup_library(imageenc)
set_target_properties (imageenc PROPERTIES FOLDER Submodules)
target_link_libraries(imageenc webp imageioutil ${WEBP_DEP_LIBRARIES})