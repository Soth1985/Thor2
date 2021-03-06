project(Core VERSION ${THOR_VERSION} LANGUAGES CXX)

option(THOR_USE_PHYSFS "Whether to include support for PhysFS" ON)

set(Core_SRC
    Containers/Singleton.cpp
    Debug/ThAssert.cpp
    Debug/ThLogger.cpp
    Hash/ThIntHash.cpp
    Hash/ThCrc32.cpp
    Hash/ThSipHash.cpp
    Hash/ThMurmurHash.cpp
    Hash/ThSha3.cpp
    Filesystem/FSExceptions.cpp
    Filesystem/ThiFileStream.cpp
    Filesystem/LibFileSystem.cpp
    Filesystem/ThiFileSystem.cpp
    Filesystem/LoggerFileOutput.cpp
    Filesystem/ThiMemoryStream.cpp
    Filesystem/ThiDataStream.cpp
    Filesystem/ThiDynamicLibrary.cpp
    Platform/ThPlatformUtils.cpp
    Rtti/ThException.cpp
    Rtti/ThiObject.cpp
    Rtti/ThRttiManager.cpp
    Rtti/ThiRtti.cpp
    Memory/ThMemory.cpp
    Memory/ThStackAllocator.cpp
    Memory/ThPoolAllocator.cpp
    Memory/ThSystemAllocator.cpp
    Memory/ThSmallMemoryAllocator.cpp
    Memory/ThiMemoryAllocator.cpp
    Memory/ThAllocators.cpp
    Memory/ThGrowingPoolAllocator.cpp
    Memory/ThVirtualPoolAllocator.cpp
    Memory/ThVirtualSmallMemoryAllocator.cpp
    Memory/ThVirtualStackAllocator.cpp
    Memory/ThVirtualGrowingStackAllocator.cpp
    String/ThStringUtilities.cpp
    String/ThStringBuilder.cpp
    Util/ThUidGenerator.cpp
    Concurrent/ThDispatch.cpp
    ThiObject.cpp)

set(Core_H
    ${THOR_INCLUDE_DIR}/Thor/Core/Common.h
    ${THOR_INCLUDE_DIR}/Thor/Core/CoreExport.h
    ${THOR_INCLUDE_DIR}/Thor/Core/CoreFwd.h
    ${THOR_INCLUDE_DIR}/Thor/Core/CoreFwdInternal.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThException.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThFlags.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThObjectUtils.h
    ${THOR_INCLUDE_DIR}/Thor/Core/ThiObject.h
    ${THOR_INCLUDE_DIR}/Thor/Core/TypeLists.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/Singleton.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThDelegate.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThFlags.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThHashMap.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThHashSet.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Containers/ThVector.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Debug/ThAssert.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Debug/ThLogger.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThCrc32.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThIntHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThSipHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThMurmurHash.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Hash/ThSha3.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/FSExceptions.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/FSLibConfig.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/LibFileSystem.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/LoggerFileOutput.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/ThiDataStream.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/ThiDynamicLibrary.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/ThiFileStream.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/ThiFileSystem.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/ThiMemoryStream.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThMemory.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThMemoryUtil.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThStackAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThPoolAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThSystemAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThSmallMemoryAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThiMemoryAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThAllocators.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThGrowingPoolAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualPoolAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualSmallMemoryAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualStackAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Memory/ThVirtualGrowingStackAllocator.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Platform/ThPlatformUtils.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Rtti/ThRttiManager.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Rtti/ThTypeInfo.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Rtti/ThiRtti.h
    ${THOR_INCLUDE_DIR}/Thor/Core/SmartPointers/RefPtr.h
    ${THOR_INCLUDE_DIR}/Thor/Core/String/String.h
    ${THOR_INCLUDE_DIR}/Thor/Core/String/ThStringBuilder.h
    ${THOR_INCLUDE_DIR}/Thor/Core/String/ThStringUtilities.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Concurrent/ThDispatch.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Concurrent/ThSpinMutex.h
    ${THOR_INCLUDE_DIR}/Thor/Core/Util/ThUidGenerator.h)

if(THOR_USE_PHYSFS)
    set(Core_SRC ${Core_SRC}
        Filesystem/PhysFileStream.cpp
        Filesystem/PhysFileSystem.cpp)

    set(Core_H ${Core_H}
        ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/PhysFileStream.h
        ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/PhysFileSystem.h)
else()
    set(Core_SRC ${Core_SRC}
        Filesystem/StdFileSystem.cpp
        Filesystem/StlFileStream.cpp)

    set(Core_H ${Core_H}
        ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/StlFileStream.h
        ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/StdFileSystem.h)
endif()

if(WIN32)
    set(Core_SRC ${Core_SRC}
        Filesystem/ThWinDynamicLibrary.cpp
        Platform/ThWinSystemWindow.cpp)

    set(Core_H ${Core_H}
        ${THOR_INCLUDE_DIR}/Thor/Core/Filesystem/ThWinDynamicLibrary.h
        ${THOR_INCLUDE_DIR}/Thor/Core/Platform/ThWinSystemWindow.h)
endif()

list(SORT Core_SRC)
list(SORT Core_H)
add_library(Core ${Core_SRC} ${Core_H})
set_target_properties (Core PROPERTIES FOLDER Lib)
thor_add_precompiled_header(Core "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(Core)
thor_ios_target_setup_library(Core)

if(THOR_USE_PHYSFS)
    target_compile_definitions(Core PUBLIC THOR_USE_PHYSFS)
else()
    target_compile_definitions(Core PUBLIC THOR_USE_STLFS)
endif()

target_include_directories(Core PUBLIC ${THOR_INCLUDE_DIR})

if(THOR_USE_PHYSFS)
    target_link_libraries(Core physfs)    
endif()
