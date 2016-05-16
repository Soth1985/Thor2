#pragma once

#include <Thor/Core/Common.h>

#ifdef THOR_MS_WIN
	#ifdef THOR_FRAMEWORK_EXPORT
		#define THOR_FRAMEWORK_DLL __declspec(dllexport)
	#elif defined THOR_FRAMEWORK_IMPORT
		#define THOR_FRAMEWORK_DLL __declspec(dllimport)
	#else
		#define THOR_FRAMEWORK_DLL 
	#endif
#else 
		#define THOR_FRAMEWORK_DLL
#endif

namespace Thor
{
	struct eSingletonsLongevity
	{
		enum Val
		{
			TypeInfo,
			RttiManager
		};
	};
}