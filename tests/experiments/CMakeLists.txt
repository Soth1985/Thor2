add_executable(TestContainers TestContainers.cpp)
thor_macos_target_setup_console_app(TestContainers)
thor_msvc_target_setup_console_executable(TestContainers)
target_link_libraries(TestContainers Core)
set_target_properties (TestContainers PROPERTIES FOLDER Tests)

add_executable(TestRefPtr TestRefPtr.cpp)
thor_macos_target_setup_console_app(TestRefPtr)
thor_msvc_target_setup_console_executable(TestRefPtr)
target_link_libraries(TestRefPtr Core)
set_target_properties (TestRefPtr PROPERTIES FOLDER Tests)

add_executable(TestSSE TestSSE.cpp)
thor_macos_target_setup_console_app(TestSSE)
thor_msvc_target_setup_console_executable(TestSSE)
target_link_libraries(TestSSE Core Math)
set_target_properties (TestSSE PROPERTIES FOLDER Tests)

add_executable(TestSIMD TestSimd.cpp)
thor_macos_target_setup_console_app(TestSIMD)
thor_msvc_target_setup_console_executable(TestSIMD)
target_link_libraries(TestSIMD Core Math)
set_target_properties (TestSIMD PROPERTIES FOLDER Tests)

add_executable(TestAllocators TestAllocators.cpp)
thor_macos_target_setup_console_app(TestAllocators)
thor_msvc_target_setup_console_executable(TestAllocators)
target_link_libraries(TestAllocators Core)
set_target_properties (TestAllocators PROPERTIES FOLDER Tests)

add_executable(TestECS TestECS.cpp)
thor_macos_target_setup_console_app(TestECS)
thor_msvc_target_setup_console_executable(TestECS)
target_link_libraries(TestECS Core ECS)
set_target_properties (TestECS PROPERTIES FOLDER Tests)