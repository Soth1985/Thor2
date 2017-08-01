project(Engine VERSION ${THOR_VERSION} LANGUAGES CXX)

set(Engine_SRC
    ThEntityManager.cpp
    ThBlackboard.cpp
    ThClusterHashComponent.cpp
    ThClusterGridComponent.cpp
    ThEventManager.cpp)

set(Engine_H
    ${THOR_INCLUDE_DIR}/Thor/Engine/EngineForward.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/EngineTypeInfo.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEntityManager.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEvent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEventManager.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThBlackboard.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEntity.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThClusterHash.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThClusterHashComponent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThClusterGridComponent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ComponentInterfaces.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/EngineInterface.h)

list(SORT Engine_SRC)
list(SORT Engine_H)
add_library(Engine ${Engine_SRC} ${Engine_H})
set_target_properties (Engine PROPERTIES FOLDER Lib)
thor_add_precompiled_header(Engine "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(Engine)
thor_ios_target_setup_library(Engine)

target_include_directories(Engine PUBLIC ${THOR_INCLUDE_DIR})
target_link_libraries(Engine Core)
