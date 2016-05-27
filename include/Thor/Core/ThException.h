#pragma once

#include <Thor/Core/CoreExport.h>
#include <string>

namespace Thor{

class THOR_FRAMEWORK_DLL ThException: public std::exception
{
public:
	ThException();

	ThException(const char* str);

	ThException(const ThString& str);
};

}//Thor