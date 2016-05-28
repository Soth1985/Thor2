#include <Thor/Core/Debug/ThAssert.h>
#include <Thor/Core/Platform/ThPlatformUtils.h>
#include <Thor/Core/String/String.h>
#include <Thor/Core/Debug/ThLogger.h>

#ifdef THOR_PLATFORM_WIN
	#include <windows.h>
#endif

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThAssert
//
//----------------------------------------------------------------------------------------
#ifdef THOR_PLATFORM_WIN

void ThAssert::Assert(ThBool result, const ThString& expr, const ThString& msg, const ThString& file, int line)
{
	if(!m_Ignore)
	{
		if(!result)
		{
			ThString text("Expr: ");
			text += expr + " Msg: " + msg + "\n File: " + file + " Line: " + ToString(line);
			THOR_CRT("%s")(coreSysLogTag, text.c_str());
#ifdef _DEBUG
			text += "\nPress Retry to debug the application.";
			ThI32 msgboxID = MessageBox(0, text.c_str(), "Assertion Failed", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON2);			

			switch(msgboxID)
			{
			case IDABORT:
				exit(1);
				break;
			case IDRETRY:
				DebugBreak();
				break;
			case IDIGNORE:
				m_Ignore = true;
				break;
			}
#else
			m_Ignore = true;//in release mode ignore all asserts but the first one
#endif
		}
	}
}

#endif


}