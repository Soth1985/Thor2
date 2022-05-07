#pragma once

#include <sstream>
#include <limits>
#include <string>
#include <locale>

#undef max
#undef min

namespace Thor
{

template <class T>
std::string ToString( const T& val )
{
	std::ostringstream stream;
	stream << val;
	return stream.str();
};

static int ToInt(const std::string& str)
{
	
	int result;
	std::stringstream stream;
	stream << str;
	stream >> result;
	std::ostringstream ostream;
	ostream << result;
	std::string res=ostream.str();

	if( str!= ostream.str() )
	{
		if(str[0]!='-')
			return std::numeric_limits<int>::max();
		else
			return std::numeric_limits<int>::min();
	}

	return result;

}

static unsigned int ToUInt(const std::string& str)
{

	unsigned int result;
	std::stringstream stream;
	stream << str;
	stream >> result;
	std::ostringstream ostream;
	ostream << result;
	std::string res=ostream.str();

	if( str!= ostream.str() )
	{
		return std::numeric_limits<unsigned int>::max();
	}

	return result;

}

}
