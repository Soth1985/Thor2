add_library(ktx STATIC IMPORTED)
set_property(TARGET ktx PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/import/ktx/include)

if (MACOS)
    set_property(TARGET ktx PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/import/ktx/lib/macArm/libktx.a)
endif()

if (IOS)
    #set_property(TARGET ktx PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/import/ktx/lib/iosSimArm/libktx.a)
    set_property(TARGET ktx PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/import/ktx/lib/iosArm/lib$(EFFECTIVE_PLATFORM_NAME)/libktx.a)
endif()