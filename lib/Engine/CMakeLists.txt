project(Engine VERSION ${THOR_VERSION} LANGUAGES CXX)

set(Engine_SRC
    ThEntityManager.cpp
    ThBlackboard.cpp
    ThEventManager.cpp)

set(Engine_H
    ${THOR_INCLUDE_DIR}/Thor/Engine/EngineForward.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/EngineTypeInfo.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEntityManager.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEvent.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEventManager.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThBlackboard.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThEntity.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/ThSpaceHash.h
    ${THOR_INCLUDE_DIR}/Thor/Engine/Interface.h)

list(SORT Engine_SRC)
list(SORT Engine_H)
add_library(Engine ${Engine_SRC} ${Engine_H})
thor_target_cpp_std(Engine "c++14")

target_include_directories(Engine PUBLIC ${THOR_INCLUDE_DIR})
target_link_libraries(Engine Core)

if(APPLE)
    target_link_libraries(Engine "-framework CoreFoundation" "-framework CoreServices" "-framework Carbon" "-framework IOKit")
endif()

export(TARGETS Engine FILE Engine.cmake)

install(TARGETS Engine
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
    		RUNTIME  DESTINATION bin/Thor)