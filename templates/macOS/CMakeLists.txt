project(macOS VERSION ${THOR_VERSION})

set(macOS_SRC
    AppDelegate.m
    main.m
    ViewController.m
)

set(macOS_H
    AppDelegate.h
)

set(macOS_STORYBOARDS
    Main.storyboard
)

set(macOS_ICONS "")

add_executable(macOS MACOSX_BUNDLE ${macOS_SRC} ${macOS_H} ${macOS_STORYBOARDS})
target_link_libraries(macOS Core)

#link framework
target_link_libraries(macOS "-framework Cocoa" )

#plist
set_target_properties(macOS PROPERTIES
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/Info.plist.in"
    MACOSX_BUNDLE_INFO_STRING "Description"
    MACOSX_BUNDLE_ICON_FILE ""
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.test.macOS"
    MACOSX_BUNDLE_BUNDLE_NAME "AppName"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
    MACOSX_BUNDLE_MAIN_STORYBOARD "Main"
    MACOSX_BUNDLE_COPYRIGHT "Victor Rykov"
    RESOURCE "${macOS_ICONS};${macOS_STORYBOARDS}"
)