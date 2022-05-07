#pragma once

#if defined THOR_DEBUG || (!defined THOR_DEBUG && defined THOR_ASSERT_IN_RELEASE)
    #define THOR_ASSERT(expr, msg) {static ThAssert s_Assert##__LINE__;s_Assert##__LINE__.Assert(expr, #expr, msg, __FILE__, __LINE__);}
#else
    #define THOR_ASSERT(expr, msg)
#endif

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThAssert
//
//----------------------------------------------------------------------------------------
class ThAssert
{
public:
	void Assert(bool result, const char* expr, const char* msg, const char* file, int line);
private:
	bool m_Ignore;
};

}
