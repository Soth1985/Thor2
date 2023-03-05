project(UniversalAppTemplateCpp VERSION ${THOR_VERSION})

set(${PROJECT_NAME}_SRC
    MetalViewDelegate.mm
    MetalContext.cpp
    MetalRenderer.cpp
    MetalRendererTriangle.cpp)

set(${PROJECT_NAME}_H
    MetalViewDelegate.h
    MetalContext.h
    MetalRenderer.h
    MetalRendererTriangle.h)

set(${PROJECT_NAME}_RESOURCE_FILES)

set(${PROJECT_NAME}_SHADERS
    Shaders.metal)

if(MACOS)
    list(APPEND ${PROJECT_NAME}_SRC
        macos/AppDelegate.mm
        macos/ViewController.mm
        macos/main.mm)

    list(APPEND ${PROJECT_NAME}_H
        macos/AppDelegate.h
        macos/ViewController.h)

    list(APPEND ${PROJECT_NAME}_RESOURCE_FILES
	    macos/Assets.xcassets
        macos/platform_data
        macos/Main.storyboard)

    set_source_files_properties(${${PROJECT_NAME}_RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${${PROJECT_NAME}_SHADERS} PROPERTIES LANGUAGE METAL)
endif()

if(IOS)
    list(APPEND ${PROJECT_NAME}_SRC
        ios/AppDelegate.mm
        ios/ViewController.mm
        ios/main.mm)

    list(APPEND ${PROJECT_NAME}_H
        ios/AppDelegate.h
        ios/ViewController.h)

    list(APPEND ${PROJECT_NAME}_RESOURCE_FILES
	    ios/Assets.xcassets
        ios/platform_data
        ios/Main.storyboard
        ios/LaunchScreen.storyboard)
        	
    set_source_files_properties(${${PROJECT_NAME}_RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${${PROJECT_NAME}_SHADERS} PROPERTIES LANGUAGE METAL)
endif()

list(SORT ${PROJECT_NAME}_SRC)
list(SORT ${PROJECT_NAME}_H)
list(SORT ${PROJECT_NAME}_RESOURCE_FILES)
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_H} ${${PROJECT_NAME}_SHADERS} ${${PROJECT_NAME}_RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} MetalCpp)
target_link_libraries(${PROJECT_NAME} SimdMath)
source_group ("Resources" FILES ${PROJECT_NAME}_RESOURCE_FILES)
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER Applications)
thor_add_precompiled_header(Core "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_executable(${PROJECT_NAME} "com.rv.macos.apptemplate" "${PROJECT_NAME}" "info.plist")
thor_ios_target_setup_executable(${PROJECT_NAME} "com.rv.ios.apptemplate" "${PROJECT_NAME}" "info.plist")
