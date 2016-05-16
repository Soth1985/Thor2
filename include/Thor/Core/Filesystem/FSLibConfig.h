#ifndef THOR_FS_LIBCONFIG_H
#define THOR_FS_LIBCONFIG_H

#include <Thor/Core/Common.h>
#include <Thor/Core/CoreExport.h>
#include <Thor/Core/CoreFwd.h>

namespace Thor
{
#ifdef THOR_USE_PHYSFS
	THOR_DECLARE_CLASS_NS(ThPhysFileStream, Thor);
	THOR_DECLARE_CLASS_NS(ThPhysFileSystem, Thor);
#endif

#ifdef THOR_USE_BOOSTFS
	THOR_DECLARE_CLASS_NS(ThBoostFileSystem, Thor);
	THOR_DECLARE_CLASS_NS(ThStlFileStream, Thor);
#endif	

	THOR_DECLARE_CLASS_NS(ThLoggerFileOutput, Thor);

#ifdef THOR_MS_WIN
	THOR_DECLARE_CLASS_NS(ThWinDynamicLibrary, Thor);
#endif
}

#endif