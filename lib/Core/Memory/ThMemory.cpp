#include <Thor/Core/Memory/ThMemory.h>
#include <stdlib.h>
#include <malloc/malloc.h>

namespace Thor{

//----------------------------------------------------------------------------------------
//
//					ThMalloc
//
//----------------------------------------------------------------------------------------
void* ThMemory::Alloca(ThSize size)
{
#ifdef THOR_PLATFORM_WIN
	return _malloca(size);
#else
    return alloca(size);
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::Malloc(ThSize size)
{
    return malloc(size);
}
//----------------------------------------------------------------------------------------
void ThMemory::Free(void* ptr)
{
    free(ptr);
}
//----------------------------------------------------------------------------------------
void* ThMemory::AlignedMalloc(ThSize size, ThSize alignment)
{	
#ifdef THOR_PLATFORM_OSX
    void* result = 0;
    posix_memalign(&result, size, alignment);
    return result;
#elif defined THOR_PLATFORM_WIN
    return _aligned_malloc(size, alignment);
#endif
}
//----------------------------------------------------------------------------------------
void ThMemory::AlignedFree(void* ptr)
{
#ifdef THOR_PLATFORM_OSX
    free(ptr);
#elif defined THOR_PLATFORM_WIN
    _aligned_free(ptr);
#endif
}
//----------------------------------------------------------------------------------------
ThSize ThMemory::MallocSize(void* ptr)
{
#ifdef THOR_PLATFORM_OSX
    ThSize sz = malloc_size(ptr);
    return malloc_good_size(sz);
#elif defined THOR_PLATFORM_WIN
    msize(ptr);
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::MemoryCopy(void* destination, const void* source, ThSize numBytes)
{
	return memcpy(destination, source, numBytes);
}

}//Thor