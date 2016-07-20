project(template_mac_os VERSION ${THOR_VERSION})

set(template_mac_os_SRC
    AppDelegate.m
    main.m
    ViewController.m
)

set(template_mac_os_H
    AppDelegate.h
    ViewController.h
)

set(template_mac_os_STORYBOARDS
    Main.storyboard
)

set(template_mac_os_ICONS "")

add_executable(template_mac_os MACOSX_BUNDLE ${template_mac_os_SRC} ${template_mac_os_H} ${template_mac_os_STORYBOARDS})
target_link_libraries(template_mac_os Core)

#link framework
target_link_libraries(template_mac_os "-framework Cocoa" )

#plist
set_target_properties(template_mac_os PROPERTIES
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/Info.plist.in"
    MACOSX_BUNDLE_INFO_STRING "Description"
    MACOSX_BUNDLE_ICON_FILE ""
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.test.template_mac_os"
    MACOSX_BUNDLE_BUNDLE_NAME "AppName"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_BUNDLE_VERSION "1"
    MACOSX_BUNDLE_MAIN_STORYBOARD "Main"
    MACOSX_BUNDLE_COPYRIGHT "Copyright 2016 Victor Rykov"
    RESOURCE "${template_mac_os_ICONS};${template_mac_os_STORYBOARDS}"
)