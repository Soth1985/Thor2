add_executable(TestContainers TestContainers.cpp)
target_link_libraries(TestContainers Core)

add_executable(TestFileSystem TestFileSystem.cpp)
target_link_libraries(TestFileSystem Core)

add_executable(TestReflection TestReflection.cpp)
target_link_libraries(TestReflection Core)

add_executable(TestRefPtr TestRefPtr.cpp)
target_link_libraries(TestRefPtr Core Math)