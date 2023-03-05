add_subdirectory(Core)
add_subdirectory(Math)
add_subdirectory(Engine)
add_subdirectory(SimdMath)

if (APPLE)
    add_subdirectory(MetalCpp)
endif()