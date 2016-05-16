add_library(Core
		src/Containers/Singleton.cpp
		src/Debug/ThAssert.cpp
		src/Debug/ThLogger.cpp
		src/FileSystem/FSExceptions.cpp
		src/FileSystem/StdFileSystem.cpp
		src/FileSystem/ThiFileStream.cpp
		src/FileSystem/LibFileSystem.cpp
		src/FileSystem/	StlFileStream.cpp
		src/FileSystem/	ThiFileSystem.cpp
		src/FileSystem/LoggerFileOutput.cpp
		src/FileSystem/	ThWinDynamicLibrary.cpp
		src/FileSystem/	ThiMemoryStream.cpp
		src/FileSystem/PhysFileStream.cpp
		src/FileSystem/	ThiDataStream.cpp
		src/FileSystem/PhysFileSystem.cpp
		src/FileSystem/	ThiDynamicLibrary.cpp
		src/Rtti/ThException.cpp
		src/Rtti/ThiObject.cpp
		src/Rtti/ThRttiManager.cpp
		src/Rtti/ThiRtti.cpp)

target_include_directories (Core PUBLIC  
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    		$<INSTALL_INTERFACE:include>
    		PRIVATE src)

install(TARGETS Core
    		ARCHIVE  DESTINATION Thor/lib
    		LIBRARY  DESTINATION Thor/lib
    		RUNTIME  DESTINATION Thor/bin)

install(DIRECTORY include/ DESTINATION Thor/include)