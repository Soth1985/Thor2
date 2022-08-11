add_library(MetalCpp STATIC ${CMAKE_SOURCE_DIR}/lib/MetalCpp/Implementation.mm)
target_include_directories(MetalCpp PUBLIC ${CMAKE_SOURCE_DIR}/lib/MetalCpp)
set_target_properties (MetalCpp PROPERTIES FOLDER Lib)

thor_macos_target_setup_library(MetalCpp)
thor_ios_target_setup_library(MetalCpp)
thor_msvc_target_setup_library(MetalCpp)