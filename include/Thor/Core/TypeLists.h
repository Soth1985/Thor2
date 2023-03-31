#pragma once

//see Modern C++ Design: Generic Programming and Design Patterns Applied for details
#include <Thor/Core/Common.h>

namespace Thor
{

namespace Private
{
    struct NullType {};
}

template <typename T, typename... S>
struct ThTypeList
{
	typedef T Head;
	typedef ThTypeList<S...> Tail;
};

template <typename T>
struct ThTypeList<T>
{
	typedef T Head;
	typedef Private::NullType Tail;
};

struct ThTypeList
{

};

template <typename T, typename TL>
struct ThTypeId
{
	enum { value = 1 + ThTypeId<T, typename TL::Tail>::value };
};

template <typename T, typename... S>
struct ThTypeId<T, TypeList<T, S...>>
{
	enum { value = 0 };
};

template <int I, typename TL>
struct ThTypeAt
{
	typedef typename ThTypeAt<I - 1, typename TL::Tail>::type type;
};

template <typename Head, typename... S>
struct ThTypeAt<0, TypeList<Head, S...>>
{
	typedef Head type;
};

template <typename TL>
struct ThLength
{
	enum { value =  1 + ThLength<TL::Tail>::value };
};

template <>
struct ThLength<Private::NullType>
{
	enum { value = 0 };
};

template <>
struct ThLength<ThTypeList>
{
	enum { value = 0 };
};

}//Thor