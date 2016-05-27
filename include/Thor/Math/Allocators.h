#pragma once

#include <cstdlib>  
#include <memory>   
#include <stdlib.h>

namespace Thor{

template < unsigned int alignment = 16 >
class ThAlignedAllocator
{
public:
	void* operator new(size_t size)
	{
        return 0;//_aligned_malloc( size,alignment);
	};

	void* operator new[](size_t size)
	{
        return 0;//_aligned_malloc( size,alignment);
	};

	void operator delete(void* p)
	{
        //_aligned_free( p );
	};

	void operator delete[](void* p)
	{
        //_aligned_free( p );
	};

	//virtual ~AlignedAllocator(){};
};


class ThStandartAllocator
{
public:
	void* operator new(size_t size)
	{
		return ::operator new( size );
	};

	void* operator new[](size_t size)
	{
		return ::operator new[]( size );;
	};

	void operator delete(void* p)
	{
		return ::operator delete( p );
	};

	void operator delete[](void* p)
	{
		return ::operator delete[]( p );
	};

	//virtual ~StandartAllocator(){};
};

}//Thor