project(Engine VERSION ${THOR_VERSION} LANGUAGES CXX)

set(Engine_SRC
    ThEntityManager.cpp
    ThBlackboard.cpp
    ThEventManager.cpp)

set(Engine_H
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEngineForward.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEngineTypeInfo.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEntityManager.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEvent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEventManager.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThBlackboard.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEntity.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThClusterHash.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThComponentTypes.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThResourceTypes.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEngineInterface.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThSimdMatrixUtil.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThSimdUtil.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThTransform.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThStringHash.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/Components/ThComponentManagerUtil.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/Components/ThMeshGeometryComponent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/Components/ThSpriteGeometryComponent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/Components/ThTaggingComponent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/Components/ThTransformComponent.h)

list(SORT Engine_SRC)
list(SORT Engine_H)
add_library(Engine ${Engine_SRC} ${Engine_H})
set_target_properties (Engine PROPERTIES FOLDER Lib)
thor_add_precompiled_header(Engine "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(Engine)
thor_ios_target_setup_library(Engine)

target_include_directories(Engine PUBLIC ${THOR_INCLUDE_DIR})
target_link_libraries(Engine Core)
