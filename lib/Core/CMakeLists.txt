project(Core VERSION ${THOR_VERSION} LANGUAGES CXX)

add_library(Core
		Containers/Singleton.cpp
		Debug/ThAssert.cpp
		Debug/ThLogger.cpp
		Filesystem/FSExceptions.cpp
		Filesystem/StdFileSystem.cpp
		Filesystem/ThiFileStream.cpp
		Filesystem/LibFileSystem.cpp
		Filesystem/StlFileStream.cpp
		Filesystem/ThiFileSystem.cpp
		Filesystem/LoggerFileOutput.cpp
		Filesystem/ThWinDynamicLibrary.cpp
		Filesystem/ThiMemoryStream.cpp
		Filesystem/PhysFileStream.cpp
		Filesystem/ThiDataStream.cpp
		Filesystem/PhysFileSystem.cpp
		Filesystem/ThiDynamicLibrary.cpp
		Platform/ThPlatformUtils.cpp
		Platform/ThWinSystemWindow.cpp
		Rtti/ThException.cpp
		Rtti/ThiObject.cpp
		Rtti/ThRttiManager.cpp
		Rtti/ThiRtti.cpp
        Memory/ThMemory.cpp
		String/ThStringUtilities.cpp
        ThiObject.cpp)

MESSAGE( STATUS "THOR_INCLUDE_DIR: " ${THOR_INCLUDE_DIR} )
target_include_directories(Core PUBLIC ${THOR_INCLUDE_DIR})

install(TARGETS Core
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
    		RUNTIME  DESTINATION bin/Thor)