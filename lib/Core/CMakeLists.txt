project(Core VERSION ${THOR_VERSION} LANGUAGES CXX)

add_library(Core
		src/Containers/Singleton.cpp
		src/Debug/ThAssert.cpp
		src/Debug/ThLogger.cpp
		src/Filesystem/FSExceptions.cpp
		src/Filesystem/StdFileSystem.cpp
		src/Filesystem/ThiFileStream.cpp
		src/Filesystem/LibFileSystem.cpp
		src/Filesystem/StlFileStream.cpp
		src/Filesystem/ThiFileSystem.cpp
		src/Filesystem/LoggerFileOutput.cpp
		src/Filesystem/ThWinDynamicLibrary.cpp
		src/Filesystem/ThiMemoryStream.cpp
		src/Filesystem/PhysFileStream.cpp
		src/Filesystem/ThiDataStream.cpp
		src/Filesystem/PhysFileSystem.cpp
		src/Filesystem/ThiDynamicLibrary.cpp
		src/Platform/ThPlatformUtils.cpp
		src/Platform/ThWinSystemWindow.cpp
		src/Rtti/ThException.cpp
		src/Rtti/ThiObject.cpp
		src/Rtti/ThRttiManager.cpp
		src/Rtti/ThiRtti.cpp)

target_include_directories (Core PUBLIC  
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    		$<INSTALL_INTERFACE:include>
    		PRIVATE src)

install(TARGETS Core
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
    		RUNTIME  DESTINATION bin/Thor)

install(DIRECTORY include/ DESTINATION include/Thor)