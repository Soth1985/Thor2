#pragma once

#include <string>

namespace Thor{

class ThException: public std::exception
{
public:
	ThException();

	ThException(const char* str);

	ThException(const ThString& str);
};

}//Thor