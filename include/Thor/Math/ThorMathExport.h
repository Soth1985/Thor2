#pragma once

#include <Thor/Core/Common.h>

#ifdef THOR_PLATFORM_WIN
	#ifdef THOR_MATH_EXPORT
		#define THOR_MATH_DLL __declspec(dllexport)
	#elif defined THOR_MATH_IMPORT
		#define THOR_MATH_DLL __declspec(dllimport)
	#else
		#define THOR_MATH_DLL 
	#endif
#else 
	#define THOR_MATH_DLL
#endif