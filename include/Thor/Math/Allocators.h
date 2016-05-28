#pragma once

#include <new>
#include <Thor/Core/Memory/ThMemory.h>

namespace Thor{

template < unsigned int alignment = 16 >
class ThAlignedAllocator
{
public:
	void* operator new(size_t size)
	{
        return ThMemory::AlignedMalloc(size, alignment);
	}

	void* operator new[](size_t size)
	{
        return ThMemory::AlignedMalloc(size, alignment);
	}

	void operator delete(void* p)
	{
        ThMemory::AlignedFree(p);
	}

	void operator delete[](void* p)
	{
        ThMemory::AlignedFree(p);
	}

	//virtual ~AlignedAllocator(){};
};


class ThStandartAllocator
{
public:
	void* operator new(size_t size)
	{
		return ::operator new( size );
	}

	void* operator new[](size_t size)
	{
		return ::operator new[]( size );;
	}

	void operator delete(void* p)
	{
		return ::operator delete( p );
	}

	void operator delete[](void* p)
	{
		return ::operator delete[]( p );
	}

	//virtual ~StandartAllocator(){};
};

}//Thor