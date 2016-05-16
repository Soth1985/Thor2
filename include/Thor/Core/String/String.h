#pragma once

#include <sstream>
#include <limits>
#include <string>
#include <locale>

//#include <boost/lexical_cast.hpp>

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

/*std::wstring StringToWString(const std::string& s)
{
std::wstring temp(s.length(),L' ');
std::copy(s.begin(), s.end(), temp.begin());
return temp; 
}


std::string WStringToString(const std::wstring& s)
{
std::string temp(s.length(), ' ');
std::copy(s.begin(), s.end(), temp.begin());
return temp; 
}*/

}//Thor

//namespace boost
//{
//	template<>
//    inline std::wstring lexical_cast<std::wstring, std::string>(const std::string& arg)
//    {
//		std::wstring result;
//		std::locale loc;
//		for(unsigned int i= 0; i < arg.size(); ++i)
//		{
//			result += std::use_facet<std::ctype<wchar_t> >(loc).widen(arg[i]);
//		}
//		return result;
//    }
//
//	template<>
//    inline std::string lexical_cast<std::string, std::wstring>(const std::wstring& arg)
//    {
//		std::string result;
//		std::locale loc;
//		for(unsigned int i= 0; i < arg.size(); ++i)
//		{
//			result += std::use_facet<std::ctype<wchar_t> >(loc).narrow(arg[i]);
//		}
//		return result;
//    }
//}
