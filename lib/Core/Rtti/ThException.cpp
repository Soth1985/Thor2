#include <Thor/Core/ThException.h>
#include <Thor/Core/Debug/ThLogger.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThException
//
//----------------------------------------------------------------------------------------
ThException::ThException()
{
	//
};
//----------------------------------------------------------------------------------------
ThException::ThException(const char* str)
{
	THOR_CRT("%s", "ThException", str );
};	
//----------------------------------------------------------------------------------------
ThException::ThException(const ThString& str)
{
	THOR_CRT("%s", "ThException", str.c_str() );
};

};//Thor