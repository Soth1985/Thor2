add_executable(TestContainers TestContainers.cpp)
target_link_libraries(TestContainers Core)

add_executable(TestFileSystem TestFileSystem.cpp)
target_link_libraries(TestFileSystem Core)

add_executable(TestReflection TestReflection.cpp)
target_link_libraries(TestReflection Core)

add_executable(TestRefPtr TestRefPtr.cpp)
target_link_libraries(TestRefPtr Core)

add_executable(TestSSE TestSSE.cpp)
target_link_libraries(TestSSE Core Math)

add_executable(TestAllocators TestAllocators.cpp)
target_link_libraries(TestAllocators Core)

if(APPLE)
    set_target_properties(TestFileSystem PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
    set_target_properties(TestFileSystem PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

    set_target_properties(TestContainers PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
    set_target_properties(TestContainers PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

    set_target_properties(TestAllocators PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
    set_target_properties(TestAllocators PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

    set_target_properties(TestSSE PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
    set_target_properties(TestSSE PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()