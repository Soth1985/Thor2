#pragma once

#include <Thor/Core/CoreFwdInternal.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					eMinidumpLevel
//
//----------------------------------------------------------------------------------------
struct eMinidumpLevel
{
	enum Val
	{
		Low,
		Med,
		High
	};
};
//----------------------------------------------------------------------------------------
//
//					ThPlatformUtils
//
//----------------------------------------------------------------------------------------
struct THOR_FRAMEWORK_DLL ThPlatformUtils
{
	static void						DispatchOperatingSystemMessages();
	static ThiSystemWindowPtr		CreateSystemWindow();
	static void						SetupMinidump(eMinidumpLevel::Val lvl);
};

}