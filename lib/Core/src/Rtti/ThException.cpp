#include <Thor/Framework/ThException.h>
#include <Thor/Framework/ThLogger.h>

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
	:
std::exception(str)
{
	THOR_CRT("%s")("ThException", What() );
};	
//----------------------------------------------------------------------------------------
ThException::ThException(const ThString& str)
	:
std::exception( str.c_str() )
{
	THOR_CRT("%s")("ThException", What() );
};
//----------------------------------------------------------------------------------------
const char* ThException::What()const
{
	return what();
}

};//Thor