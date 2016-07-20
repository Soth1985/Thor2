project(MetalTriangle VERSION ${THOR_VERSION})

set(MetalTriangle_SRC
    AppDelegate.m
    main.m
    ViewController.m
)

set(MetalTriangle_H
    AppDelegate.h
    ViewController.h
)

set(MetalTriangle_STORYBOARDS
    Main.storyboard
)

set(MetalTriangle_ICONS "")

add_executable(MetalTriangle MACOSX_BUNDLE ${MetalTriangle_SRC} ${MetalTriangle_H} ${MetalTriangle_STORYBOARDS})
target_link_libraries(MetalTriangle Core)

#link framework
target_link_libraries(MetalTriangle "-framework Cocoa" "-framework Metal" "-framework QuartzCore")

#plist
set_target_properties(MetalTriangle PROPERTIES
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/Info.plist.in"
    MACOSX_BUNDLE_INFO_STRING "Description"
    MACOSX_BUNDLE_ICON_FILE ""
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.test.MetalTriangle"
    MACOSX_BUNDLE_BUNDLE_NAME "AppName"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
    MACOSX_BUNDLE_MAIN_STORYBOARD "Main"
    MACOSX_BUNDLE_COPYRIGHT "Victor Rykov"
    RESOURCE "${MetalTriangle_ICONS};${MetalTriangle_STORYBOARDS}"
)