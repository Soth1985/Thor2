add_executable(Ch1 Ch1.cpp)
target_link_libraries(Ch1 Core)
thor_macos_target_setup_console_app(Ch1)
set_target_properties (Ch1 PROPERTIES FOLDER Tests)