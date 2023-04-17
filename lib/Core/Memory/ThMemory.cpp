#include <Thor/Core/Memory/ThMemory.h>
#include <Thor/Core/Debug/ThAssert.h>
#include <stdlib.h>

#ifdef THOR_PLATFORM_APPLE
    #include <malloc/malloc.h>
    #include <unistd.h>
    #include <sys/mman.h>
#endif

#ifdef THOR_PLATFORM_WIN
    #include <windows.h>
    #include <malloc.h>
#endif

using namespace Thor;

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
#ifdef THOR_PLATFORM_APPLE
    //void* result = aligned_alloc(alignment, size);
    void* result = 0;
        
    if (alignment == THOR_DEFAULT_ALIGNMENT)
        result = malloc(size);
    else
    {
        if (alignment < sizeof (void*))
            alignment = sizeof (void*);
        
        /*int status =*/posix_memalign(&result, alignment, size);
    }
    THOR_ASSERT(result != nullptr, "Failed to allocate memory");
    return result;
#elif defined THOR_PLATFORM_WIN
    return _aligned_malloc(size, alignment);
#else
    static_assert(0, "AlignedMalloc must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
void ThMemory::AlignedFree(void* ptr)
{
#ifdef THOR_PLATFORM_APPLE
    free(ptr);
#elif defined THOR_PLATFORM_WIN
    _aligned_free(ptr);
#else
    static_assert(0, "AlignedFree must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
ThSize ThMemory::MallocSize(void* ptr)
{
#ifdef THOR_PLATFORM_APPLE
    ThSize sz = malloc_size(ptr);
    return malloc_good_size(sz);
#elif defined THOR_PLATFORM_WIN
    return _msize(ptr);
#else
    static_assert(0, "MallocSize must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
ThSize ThMemory::GetPageSize()
{
#ifdef THOR_PLATFORM_APPLE
    return getpagesize();
#elif defined THOR_PLATFORM_WIN
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
#else
    static_assert(0, "GetPageSize must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::VmReserveMemory(ThSize size)
{
#ifdef THOR_PLATFORM_APPLE
    void* ptr = mmap((void*)0, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC | MS_INVALIDATE);
    return ptr;
#elif defined THOR_PLATFORM_WIN
    return VirtualAlloc(NULL, size, MEM_RESERVE , PAGE_NOACCESS);
#else
    static_assert(0, "VmReserveMemory must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::VmCommitMemory(void* ptr, ThSize size)
{
#ifdef THOR_PLATFORM_APPLE
    void* result = mmap(ptr, size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED | MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC|MS_INVALIDATE);
    return result;
#elif defined THOR_PLATFORM_WIN
    return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
#else
    static_assert(0, "VmCommitMemory must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
void ThMemory::VmDecommitMemory(void* ptr, ThSize size)
{
#ifdef THOR_PLATFORM_APPLE
    mmap(ptr, size, PROT_NONE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC | MS_INVALIDATE);
#elif defined THOR_PLATFORM_WIN
    VirtualFree((void*)ptr, size, MEM_DECOMMIT);
#else
    static_assert(0, "VmDecommitMemory must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
void ThMemory::VmFreeMemory(void* ptr, ThSize size)
{
#ifdef THOR_PLATFORM_APPLE
    msync(ptr, size, MS_SYNC);
    munmap(ptr, size);
#elif defined THOR_PLATFORM_WIN
    VirtualFree((void*)ptr, 0, MEM_RELEASE);
#else
    static_assert(0, "VmFreeMemory must be implemented");
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::MemoryCopy(void* destination, const void* source, ThSize numBytes)
{
	return memcpy(destination, source, numBytes);
}
//----------------------------------------------------------------------------------------
void* ThMemory::MemoryMove(void* destination, const void* source, ThSize numBytes)
{
    return memmove(destination, source, numBytes);
}

void ThMemory::StringCopy(ThChar* destination, ThSize destinationSize, const ThChar* source)
{
    strlcpy(destination, source, destinationSize);
}

void ThMemory::MemorySet(void* destination, ThU8 value, ThSize numBytes)
{
    memset(destination, value, numBytes);
}
