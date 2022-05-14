project(Core VERSION ${THOR_VERSION} LANGUAGES CXX)

set(Core_SRC
    Debug/ThAssert.cpp
    Debug/ThLogger.cpp
    Hash/ThIntHash.cpp
    Hash/ThCrc32.cpp
    Hash/ThSipHash.cpp
    Hash/ThMurmurHash.cpp
    Hash/ThSha3.cpp
    Memory/ThMemory.cpp
    Memory/ThStackAllocator.cpp
    Memory/ThPoolAllocator.cpp
    Memory/ThSystemAllocator.cpp
    Memory/ThSmallMemoryAllocator.cpp
    Memory/ThiMemoryAllocator.cpp
    Memory/ThGrowingPoolAllocator.cpp
    Memory/ThVirtualPoolAllocator.cpp
    Memory/ThVirtualSmallMemoryAllocator.cpp
    Memory/ThVirtualStackAllocator.cpp
    Memory/ThVirtualGrowingStackAllocator.cpp
    String/ThStringUtilities.cpp
    String/ThStringBuilder.cpp
    Util/ThUidGenerator.cpp
    Concurrent/ThDispatch.cpp
    ThCore.cpp)

set(Core_H
    ${THOR_INCLUDE_DIR}/Thor/Core/Common.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThCore.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThException.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThFlags.h
    ${THOR_INCLUDE_DIR}/Thor/Core/TypeLists.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThDelegate.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThFlags.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThHashMap.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThHashSet.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThVector.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThRefPtr.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Debug/ThAssert.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Debug/ThLogger.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThCrc32.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThIntHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThSipHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThMurmurHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThSha3.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThMemory.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThMemoryUtil.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThStackAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThPoolAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThSystemAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThSmallMemoryAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThiMemoryAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThGrowingPoolAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualPoolAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualSmallMemoryAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualStackAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualGrowingStackAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/String/String.h
    ${THOR_INCLUDE_DIR}/Thor/Core/String/ThStringBuilder.h
    ${THOR_INCLUDE_DIR}/Thor/Core/String/ThStringUtilities.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Concurrent/ThDispatch.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Concurrent/ThSpinMutex.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Util/ThUidGenerator.h)

list(SORT Core_SRC)
list(SORT Core_H)
add_library(Core ${Core_SRC} ${Core_H})
set_target_properties (Core PROPERTIES FOLDER Lib)
target_include_directories(Core PUBLIC ${THOR_INCLUDE_DIR})
thor_add_precompiled_header(Core "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(Core)
thor_ios_target_setup_library(Core)
thor_msvc_target_setup_library(Core)

if (MSVC)
    include(import_dispatch)
    target_link_libraries(Core dispatch)
endif()
