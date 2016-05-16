#pragma once

#include <Thor/Core/CoreExport.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThAssert
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThAssert
{
public:
	void Assert(ThBool result, const ThString& expr, const ThString& msg, const ThString& file, int line);
private:
	ThBool m_Ignore;
};

#if defined THOR_DEBUG || (!defined THOR_DEBUG && defined THOR_ASSERT_IN_RELEASE)
	#define THOR_ASSERT(expr, msg) {static ThAssert s_Assert##__LINE__;s_Assert##__LINE__.Assert(expr, #expr, msg, __FILE__, __LINE__);}
#else
	#define THOR_ASSERT(expr, msg)
#endif

}