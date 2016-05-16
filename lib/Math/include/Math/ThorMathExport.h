#pragma once

#include <Thor/Framework/Common.h>

#ifdef THOR_MS_WIN
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