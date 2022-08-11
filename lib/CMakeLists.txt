add_subdirectory(Core)
add_subdirectory(Math)
add_subdirectory(Engine)

if (APPLE)
    add_subdirectory(MetalCpp)
endif()