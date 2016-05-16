#include <Thor/Framework/ThMemory.h>
#include <malloc.h>

#ifdef THOR_USE_TBB_MALLOC
	#include <tbb/scalable_allocator.h>
#endif

namespace Thor{

//----------------------------------------------------------------------------------------
//
//					ThMemoryAllocator
//
//----------------------------------------------------------------------------------------
void* ThMemoryAllocator::operator new(ThSize sz)
{
	return Private::ThMemoryToolbox::MallocOrThrow(sz);
}
//----------------------------------------------------------------------------------------
void* ThMemoryAllocator::operator new[](ThSize sz)
{
	return Private::ThMemoryToolbox::MallocOrThrow(sz);
}
//----------------------------------------------------------------------------------------
void ThMemoryAllocator::operator delete(void* ptr)
{
	ThMemory::Free(ptr);
}
//----------------------------------------------------------------------------------------
void ThMemoryAllocator::operator delete[](void* ptr)
{
	ThMemory::Free(ptr);
}
//----------------------------------------------------------------------------------------
void* ThMemoryAllocator::operator new(ThSize sz, const std::nothrow_t&) throw()
{
	return ThMemory::Malloc(sz);
}
//----------------------------------------------------------------------------------------
void* ThMemoryAllocator::operator new[](ThSize sz, const std::nothrow_t&) throw()
{
	return ThMemory::Malloc(sz);
}
//----------------------------------------------------------------------------------------
void ThMemoryAllocator::operator delete(void* ptr, const std::nothrow_t&) throw()
{
	ThMemory::Free(ptr);
}
//----------------------------------------------------------------------------------------
void ThMemoryAllocator::operator delete[](void* ptr, const std::nothrow_t&) throw()
{
	ThMemory::Free(ptr);
}
//----------------------------------------------------------------------------------------
void* ThMemoryAllocator::operator new (ThSize sz, void* ptr) throw() 
{ 
	return ptr;
}
//----------------------------------------------------------------------------------------
void* ThMemoryAllocator::operator new[] (ThSize sz, void* ptr) throw() 
{
	return ptr;
}
//----------------------------------------------------------------------------------------
void ThMemoryAllocator::operator delete (void* ptr1, void* ptr2) throw()
{ }
//----------------------------------------------------------------------------------------
void ThMemoryAllocator::operator delete[] (void* ptr1, void* ptr2) throw()
{ }
//----------------------------------------------------------------------------------------
//
//					ThMalloc
//
//----------------------------------------------------------------------------------------
void* ThMemory::Alloca(ThSize size)
{
#ifdef _MSC_VER
	return _malloca(size);
#endif
}
//----------------------------------------------------------------------------------------
void* ThMemory::Malloc(ThSize size)
{
#ifdef THOR_USE_ALIGNED_ALLOCATOR
	return ThMemory::AlignedMalloc(size, m_Alignment);
#else
	#ifdef THOR_USE_TBB_MALLOC
		return scalable_malloc(size);		
	#else
		return malloc(size);
	#endif //THOR_USE_TBB_MALLOC
#endif //THOR_USE_ALIGNED_ALLOCATOR
}
//----------------------------------------------------------------------------------------
void ThMemory::Free(void* ptr)
{
#ifdef THOR_USE_ALIGNED_ALLOCATOR
	ThMemory::AlignedFree(ptr);
#else
	#ifdef THOR_USE_TBB_MALLOC
		scalable_free(ptr);		
	#else
		free(ptr);
	#endif //THOR_USE_TBB_MALLOC
#endif //THOR_USE_ALIGNED_ALLOCATOR
}
//----------------------------------------------------------------------------------------
void* ThMemory::AlignedMalloc(ThSize size, ThSize alignment)
{	
#ifdef THOR_USE_TBB_MALLOC
	return scalable_aligned_malloc(size, alignment);
#else
	return ::_aligned_malloc(size, alignment);
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
void* ThMemory::AlignedRealloc(void* ptr, ThSize size, ThSize alignment)
{
#ifdef THOR_USE_TBB_MALLOC
	return scalable_aligned_realloc(ptr, size, alignment);
#else
	return _aligned_realloc(ptr, size, alignment);
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
void ThMemory::AlignedFree(void* ptr)
{
#ifdef THOR_USE_TBB_MALLOC
	scalable_aligned_free(ptr);
#else
	_aligned_free(ptr);
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
void* ThMemory::Calloc(ThSize nobj, ThSize size)
{
#ifdef THOR_USE_TBB_MALLOC
	return scalable_calloc(nobj, size);
#else
	return calloc(nobj, size);
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
void* ThMemory::Realloc (void* ptr, ThSize size)
{
#ifdef THOR_USE_TBB_MALLOC
	return scalable_realloc(ptr, size);
#else
	return realloc(ptr, size);
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
ThSize ThMemory::GetAlignment()
{
	return m_Alignment;
}
//----------------------------------------------------------------------------------------
ThBool ThMemory::IsCustomAllocatedMemory(void* ptr)
{
#ifdef THOR_USE_TBB_MALLOC
	if(ptr)
		return is_tbb_malloc_ptr(ptr);
	else
		return false;
#else
	return false;
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
ThSize ThMemory::Msize(void* ptr)
{
#ifdef THOR_USE_TBB_MALLOC
	return scalable_msize(ptr);
#else
	return _msize(ptr);
#endif //THOR_USE_TBB_MALLOC
}
//----------------------------------------------------------------------------------------
void* ThMemory::MemoryCopy( void * destination, const void * source, ThSize numBytes)
{
	return ::memcpy(destination, source, numBytes);
}
//----------------------------------------------------------------------------------------
#ifdef THOR_USE_ALIGNED_ALLOCATOR
	ThSize ThMemory::m_Alignment = THOR_GLOBAL_ALLOC_ALIGNMENT;
#else
	ThSize ThMemory::m_Alignment = 0;
#endif

namespace Private{
//----------------------------------------------------------------------------------------
//
//					ThMallocToolbox
//
//----------------------------------------------------------------------------------------
void* ThMemoryToolbox::MallocOrThrow(ThSize sz)
{
	void *res = ThMemory::Malloc(sz);
	if (0 == res)
		throw std::bad_alloc();
	return res;
}
//----------------------------------------------------------------------------------------
void* ThMemoryToolbox::CallocOrThrow(ThSize nobj, ThSize sz)
{
	void *res = ThMemory::Calloc(nobj, sz);
	if (0 == res)
		throw std::bad_alloc();
	return res;
}

}//Private

}//Thor