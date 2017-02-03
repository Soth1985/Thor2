#pragma once

#include <Thor/Core/ThiObject.h>
#include <Thor/Core/ThObjectUtils.h>
#include <Thor/Core/ThException.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/Debug/ThAssert.h>

namespace Thor
{
	extern const ThChar* coreSysLogTag;
	//Framework core classes
	THOR_DECLARE_CLASS_NS(ThiApplication, Thor);
	THOR_DECLARE_CLASS_NS(ThiTask, Thor);
	THOR_DECLARE_CLASS_NS(ThiComponent, Thor);
	THOR_DECLARE_CLASS_NS(ThiComponentInterface, Thor);
	THOR_DECLARE_CLASS_NS(ThiFramework, Thor);	
    THOR_DECLARE_CLASS_NS(ThiEvent, Thor);
    THOR_DECLARE_CLASS_NS(ThiSysEvent, Thor);
    THOR_DECLARE_CLASS_NS(ThiSimEvent, Thor);
	THOR_DECLARE_CLASS_NS(ThiSystemWindow, Thor);
	THOR_DECLARE_CLASS_NS(ThiAsyncResult, Thor);
	//DataModel
	THOR_DECLARE_CLASS_NS(ThiEntity, Thor);
	THOR_DECLARE_PLAIN_CLASS_NS(ThiField, Thor);
	THOR_DECLARE_PLAIN_CLASS_NS(ThiEntityUserData, Thor);
	THOR_DECLARE_PLAIN_CLASS_NS(ThiStructField, Thor);
	//Filesystem classes
	THOR_DECLARE_CLASS_NS(ThiDataStream, Thor);
	THOR_DECLARE_CLASS_NS(ThiFileStream, Thor);
	THOR_DECLARE_CLASS_NS(ThiMemoryStream, Thor);
	THOR_DECLARE_CLASS_NS(ThiFileSystem, Thor);
	THOR_DECLARE_CLASS_NS(ThiDynamicLibrary, Thor);
	THOR_DECLARE_CLASS_NS(ThFileAsyncResult, Thor);
	//Archive
	THOR_DECLARE_CLASS_NS(ThiArchiveReader, Thor);
	THOR_DECLARE_CLASS_NS(ThiArchiveWriter, Thor);
	//Core classes that don`t use reference counting
	//Typedefs
	typedef ThDelegate<ThiAsyncResultPtr>	ThiAsyncResultCallback;
	typedef std::vector<ThiField*>			ThiFieldList;
	
}//Thor
