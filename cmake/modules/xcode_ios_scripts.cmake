function (thor_ios_target_setup_compiler_shared target)
if (IOS)
	thor_set_xcode_property(${target} ARCHS "armv7 arm64")
	thor_set_xcode_property(${target} DEBUG_INFORMATION_FORMAT dwarf-with-dsym)
	thor_set_xcode_property(${target} DEBUG_INFORMATION_FORMAT[variant=Debug] dwarf)
	thor_set_xcode_property(${target} ONLY_ACTIVE_ARCH NO)
	thor_set_xcode_property(${target} ONLY_ACTIVE_ARCH[variant=Debug] YES)
	thor_set_xcode_property(${target} ENABLE_TESTABILITY NO)
	thor_set_xcode_property(${target} ENABLE_TESTABILITY[variant=Debug] YES)
	thor_set_xcode_property(${target} ENABLE_BITCODE NO)#no because some 3rd party libs are not built with bitcode support
	thor_set_xcode_property(${target} VALIDATE_PRODUCT YES)#maybe only exe
	thor_set_xcode_property(${target} VALIDATE_PRODUCT[variant=Debug] NO)#maybe only exe
	thor_set_xcode_property(${target} COPY_PHASE_STRIP YES)
	thor_set_xcode_property(${target} COPY_PHASE_STRIP[variant=Debug] NO)
	thor_set_xcode_property(${target} IPHONEOS_DEPLOYMENT_TARGET "11.0")
	thor_set_xcode_property(${target} MACOSX_DEPLOYMENT_TARGET "")
	thor_set_xcode_property(${target} SKIP_INSTALL NO)
	thor_set_xcode_property(${target} STRIP_INSTALLED_PRODUCT YES)
	thor_set_xcode_property(${target} STRIP_STYLE "all")
	thor_set_xcode_property(${target} STRIP_SWIFT_SYMBOLS YES)
	thor_set_xcode_property(${target} TARGETED_DEVICE_FAMILY "1,2")
	thor_set_xcode_property(${target} LD_RUNPATH_SEARCH_PATHS "$(inherited) @executable_path/Frameworks")
	thor_set_xcode_property(${target} GENERATE_PKGINFO_FILE YES)
	thor_set_xcode_property(${target} PRIVATE_HEADERS_FOLDER_PATH "$(CONTENTS_FOLDER_PATH)/PrivateHeaders")
	thor_set_xcode_property(${target} PUBLIC_HEADERS_FOLDER_PATH "$(CONTENTS_FOLDER_PATH)/Headers")
	thor_set_xcode_property(${target} GCC_C_LANGUAGE_STANDARD c99)
	thor_set_xcode_property(${target} CLANG_CXX_LANGUAGE_STANDARD c++14)
	thor_set_xcode_property(${target} CLANG_CXX_LIBRARY libc++)
	thor_set_xcode_property(${target} CLANG_ENABLE_MODULES NO)
	thor_set_xcode_property(${target} CLANG_ENABLE_OBJC_ARC YES)
	thor_set_xcode_property(${target} ENABLE_NS_ASSERTIONS NO)
	thor_set_xcode_property(${target} ENABLE_NS_ASSERTIONS[variant=Debug] YES)
	thor_set_xcode_property(${target} ENABLE_STRICT_OBJC_MSGSEND YES)
	thor_set_xcode_property(${target} GCC_WARN_UNDECLARED_SELECTOR YES)
endif()
endfunction()

function (thor_ios_target_setup_library target)
if (IOS)
	thor_ios_target_setup_compiler_shared(${target})
	thor_set_xcode_property(${target} MACH_O_TYPE "staticlib")
	thor_target_install(${target})
	set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/library)
endif()
endfunction()

function (thor_ios_target_setup_executable target bundle_id product_name plist_name)
if (IOS)
	thor_ios_target_setup_compiler_shared(${target})
	thor_set_xcode_property(${target} ENABLE_ON_DEMAND_RESOURCES YES)
	thor_set_xcode_property(${target} MACH_O_TYPE "Executable")
	thor_set_xcode_property(${target} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ios/${plist_name}")
	thor_set_xcode_property(${target} PRODUCT_NAME ${product_name})
	thor_set_xcode_property(${target} WRAPPER_EXTENSION app)
	thor_set_xcode_property(${target} CODE_SIGN_ENTITLEMENTS "")
	thor_set_xcode_property(${target} CODE_SIGN_IDENTITY THOR_IOS_CODE_SIGN_IDENTITY)
	thor_set_xcode_property(${target} DEVELOPMENT_TEAM THOR_IOS_DEVELOPMENT_TEAM)
	thor_set_xcode_property(${target} PRODUCT_BUNDLE_IDENTIFIER ${bundle_id})
	thor_set_xcode_property(${target} PRODUCT_MODULE_NAME "$(PRODUCT_NAME:c99extidentifier)")
	thor_set_xcode_property(${target} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
	thor_set_xcode_property(${target} ASSETCATALOG_COMPILER_LAUNCHIMAGE_NAME "LaunchImage")
	thor_target_install(${target})
	thor_target_add_framework(${target} Foundation)
	thor_target_add_framework(${target} Metal)
	thor_target_add_framework(${target} MetalKit)
	thor_target_add_framework(${target} QuartzCore)
	thor_target_add_framework(${target} AudioToolbox)
	thor_target_add_framework(${target} StoreKit)
	thor_target_add_framework(${target} SystemConfiguration)
	thor_target_add_framework(${target} CFNetwork)
	thor_target_add_framework(${target} Accelerate)
	thor_target_add_framework(${target} UIKit)
	thor_target_add_framework(${target} MessageUI)
	thor_target_add_framework(${target} AVFoundation)
	thor_target_add_framework(${target} CoreGraphics)
	thor_target_add_framework(${target} MediaPlayer)
	thor_set_xcode_property(${target} INSTALL_PATH "/Applications") #important for EVERYTHING to archive correctly
endif()
endfunction()