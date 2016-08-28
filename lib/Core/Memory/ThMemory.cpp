#include <Thor/Core/Memory/ThMemory.h>
#include <Thor/Core/Debug/ThAssert.h>
#include <stdlib.h>
#include <malloc/malloc.h>

#ifdef THOR_PLATFORM_OSX
    #include <unistd.h>
    #include <sys/mman.h>
#endif

#ifdef THOR_PLATFORM_WIN
    #include <windows.h>
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
#ifdef THOR_PLATFORM_OSX
    void* result = 0;
    
    if (alignment == 4)
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
ThSize ThMemory::GetPageSize()
{
#ifdef THOR_PLATFORM_OSX
    return getpagesize();
#elif defined THOR_PLATFORM_WIN
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::VmReserveMemory(ThSize size)
{
#ifdef THOR_PLATFORM_OSX
    void* ptr = mmap((void*)0, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC | MS_INVALIDATE);
    return ptr;
#elif defined THOR_PLATFORM_WIN
    return VirtualAlloc(NULL, size, MEM_RESERVE , PAGE_NOACCESS);
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::VmCommitMemory(void* ptr, ThSize size)
{
#ifdef THOR_PLATFORM_OSX
    void* result = mmap(ptr, size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED | MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC|MS_INVALIDATE);
    return result;
#elif defined THOR_PLATFORM_WIN
    return VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
#endif
}
//----------------------------------------------------------------------------------------
void ThMemory::VmDecommitMemory(void* ptr, ThSize size)
{
#ifdef THOR_PLATFORM_OSX
    mmap(ptr, size, PROT_NONE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC | MS_INVALIDATE);
#elif defined THOR_PLATFORM_WIN
    VirtualFree((void*)addr, size, MEM_DECOMMIT);
#endif
}
//----------------------------------------------------------------------------------------
void ThMemory::VmFreeMemory(void* ptr, ThSize size)
{
#ifdef THOR_PLATFORM_OSX
    msync(ptr, size, MS_SYNC);
    munmap(ptr, size);
#elif defined THOR_PLATFORM_WIN
    VirtualFree((void*)addr, 0, MEM_RELEASE);
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