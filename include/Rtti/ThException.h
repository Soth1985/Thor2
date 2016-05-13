#pragma once

#include <Thor/Framework/FrameworkExport.h>
#include <string>

namespace Thor{

class THOR_FRAMEWORK_DLL ThException: public std::exception
{
public:
	ThException();

	ThException(const char* str);

	ThException(const ThString& str);

	const char* What()const;
};

}//Thor