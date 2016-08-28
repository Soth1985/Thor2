project(MetalTracer VERSION ${THOR_VERSION})

set(MetalTracer_SRC
    AppDelegate.mm
    ViewController.mm
    main.m
    RayTracer.cpp
)

set(MetalTracer_H
    AppDelegate.h
    ViewController.h
    MetalShared.h
    RayTracer.h
)

set(MetalTracer_SHADERS Shaders.metal)
set_source_files_properties(${MetalTracer_SHADERS} PROPERTIES LANGUAGE METAL)

set(MetalTracer_STORYBOARDS
    MetalTracer.storyboard
)

set(MetalTracer_ICONS "")


add_executable(MetalTracer MACOSX_BUNDLE ${MetalTracer_SRC} ${MetalTracer_H} ${MetalTracer_SHADERS} ${MetalTracer_STORYBOARDS})
target_link_libraries(MetalTracer Core)

#link framework
target_link_libraries(MetalTracer "-framework Cocoa" "-framework Metal" "-framework MetalKit" "-framework QuartzCore")

#plist
set_target_properties(MetalTracer PROPERTIES
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14"
    XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/Info.plist.in"
    MACOSX_BUNDLE_INFO_STRING "Description"
    MACOSX_BUNDLE_ICON_FILE ""
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.test.MetalTracer"
    MACOSX_BUNDLE_BUNDLE_NAME "AppName"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
    MACOSX_BUNDLE_MAIN_STORYBOARD "Main"
    MACOSX_BUNDLE_COPYRIGHT "Victor Rykov"
    RESOURCE "${MetalTracer_ICONS};${MetalTracer_STORYBOARDS}"
)