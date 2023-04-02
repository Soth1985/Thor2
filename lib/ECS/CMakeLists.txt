project(ECS VERSION ${THOR_VERSION} LANGUAGES CXX)

set(ECS_SRC
    ThScene.cpp
    ThWorld.cpp
    )

set(ECS_H
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThECS.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThEntity.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThEntityManager.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThEventManager.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThEventStream.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThResourceManager.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThScene.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThSparseStructuredStorage.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThSparseStructuredStorageBuffer.h
    ${THOR_INCLUDE_DIR}/Thor/ECS/ThWorld.h
    )

list(SORT ECS_SRC)
list(SORT ECS_H)
add_library(ECS ${ECS_SRC} ${ECS_H})
set_target_properties (ECS PROPERTIES FOLDER Lib)
thor_add_precompiled_header(ECS "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(ECS)
thor_ios_target_setup_library(ECS)
thor_msvc_target_setup_library(ECS)

target_include_directories(ECS PUBLIC ${THOR_INCLUDE_DIR})
target_link_libraries(ECS Core)
