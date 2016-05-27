#pragma once

#include <Thor/Core/CoreExport.h>
#include <new>

#define THOR_OVERLOAD_GLOBAL_NEW
#define THOR_USE_ALIGNED_ALLOCATOR
#define THOR_GLOBAL_ALLOC_ALIGNMENT 16

namespace Thor{
//----------------------------------------------------------------------------------------
//
//					ThMemoryAllocator
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThMemoryAllocator
{
public:
	static void*	operator new(ThSize sz);
	static void		operator delete(void* ptr);
	static void*	operator new(ThSize sz, const std::nothrow_t& n) throw();
	static void		operator delete(void* ptr, const std::nothrow_t& n) throw();
	static void*	operator new[](ThSize sz);
	static void*	operator new[](ThSize sz, const std::nothrow_t& n) throw();
	static void		operator delete[](void* ptr);
	static void		operator delete[](void* ptr, const std::nothrow_t& n) throw();
	//placement ops
	static void* operator new (ThSize sz, void* ptr) throw();
	static void* operator new[] (ThSize sz, void* ptr) throw();
	static void operator delete (void* ptr1, void* ptr2) throw();
	static void operator delete[] (void* ptr1, void* ptr2) throw();
};
//----------------------------------------------------------------------------------------
//
//					ThMalloc
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Defines memory allocation functions, used by the framework.
*
* These functions are used to overload c++ operator new functions when THOR_OVERLOAD_GLOBAL_NEW
* is defined, via inserting trampolines to them from the crt library, see DllMain.cpp for details. If THOR_OVERLOAD_GLOBAL_NEW
* is not defined, all framework objects derived from ThiObject are still allocated through this interface.
* 
*/
class THOR_FRAMEWORK_DLL ThMemory
{
public:	

	/*!
	* \brief
	* Allocates a chunk of memory on the stack and returns a pointer to it, or zero if allocation request failed.
	*
	* \param size
	* Size of the chunk.
	* 
	*/
	static void* Alloca(ThSize size);
	/*!
	* \brief
	* Allocates a chunk of memory and returns a pointer to it, or zero if allocation request failed.
	*
	* \param size
	* Size of the chunk.
	* 
	*/
	static void* Malloc(ThSize size);

	/*!
	* \brief
	* Frees a chunk of memory.
	*
	* \param ptr
	* Pointer to the chunk.
	* 
	*/
	static void Free(void* ptr);

	/*!
	* \brief
	* The size of the memory block pointed to by the ptr parameter is changed to the size bytes, expanding or 
	* reducing the amount of memory available in the block. Should not be used with memory, allocated in static objects.
	*
	* \param ptr
	* Pointer to the memory chunk to be resized.
	*
	* \param size
	* New size.
	* 
	*/
	static void* Realloc (void* ptr, ThSize size);

	/*!
	* \brief
	* Allocates a block of memory for an array of nobj elements, each of them size bytes long.
	*
	* \param nobj
	* Number of objects in the array.
	*
	* \param size
	* Size of the array element.
	* 
	*/
	static void* Calloc(ThSize nobj, ThSize size);

	/*!
	* \brief
	* Allocates a chunk of aligned memory and returns a pointer to it, or zero if allocation request failed.
	*
	* \param size
	* Size of the chunk.
	*
	* \param alignment
	* Alignment of the chunk in bytes.
	* 
	*/
	static void* AlignedMalloc(ThSize size, ThSize alignment);

	/*!
	* \brief
	* The size of the memory block pointed to by the ptr parameter is changed to the size bytes, expanding or 
	* reducing the amount of memory available in the block. Should not be used with memory, allocated in static objects.
	*
	* \param ptr
	* Pointer to the memory chunk to be resized.
	*
	* \param size
	* New size.
	*
	* \param alignment
	* Memory alignment in bytes.
	* 
	*/
	static void* AlignedRealloc(void* ptr, ThSize size, ThSize alignment);

	/*!
	* \brief
	* Frees an aligned chunk of memory(allocated with ThMalloc::AlignedMalloc).
	*
	* \param ptr
	* Pointer to the chunk.
	* 
	*/
	static void AlignedFree(void* ptr);	

	/*!
	* \brief
	* Returns memory alignment of chunks allocated with ThMalloc::Malloc in bytes, zero if the allocator is not aligned.
	* 
	*/
	static ThSize GetAlignment();

	/*!
	* \brief
	* Returns true if the memory pointed by ptr was allocated by a custom memory allocator.
	* 
	*/
	static ThBool IsCustomAllocatedMemory(void* ptr);

	/*!
	* \brief
	* Returns the size of a memory buffer ptr points to.
	* 
	*/
	static ThSize Msize(void* ptr);

	/*!
	* \brief
	* Copies numBytes from destination into source buffer.
	* 
	*/
	static void* MemoryCopy( void * destination, const void * source, ThSize numBytes);

private:

	static ThSize m_Alignment;	
};

namespace Private{
//----------------------------------------------------------------------------------------
//
//					ThMallocToolbox
//
//----------------------------------------------------------------------------------------
struct THOR_FRAMEWORK_DLL ThMemoryToolbox
{
	static void* MallocOrThrow(ThSize sz);
	static void* CallocOrThrow(ThSize nobj, ThSize sz);
};

}//Private

//----------------------------------------------------------------------------------------
//
//					ThFrameworkAllocator
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* STL allocator template for the standard library containers,
* interfaces into the framework memory allocation infrastructure.
* 
*/
template<typename T>
class ThFrameworkAllocator 
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template<class U>
	struct rebind
	{
		typedef ThFrameworkAllocator<U> other;
	};

	ThFrameworkAllocator() throw() {}

	ThFrameworkAllocator( const ThFrameworkAllocator& ) throw() {}

	template<typename U> 
	ThFrameworkAllocator(const ThFrameworkAllocator<U>&) throw() {}

	pointer address(reference x)const
	{
		return &x;
	}

	const_pointer address(const_reference x) const
	{
		return &x;
	}

	//! Allocate space for n objects.
	pointer allocate( size_type n, const void* /*hint*/ =0 )
	{
		return static_cast<pointer>( ThMemory::Malloc( n * sizeof(value_type) ) );
	}

	//! Free previously allocated block of memory
	void deallocate( pointer p, size_type )
	{
		ThMemory::Free( p );
	}

	//! Largest value for which method allocate might succeed.
	size_type max_size() const throw()
	{
		size_type absolutemax = static_cast<size_type>(-1) / sizeof (value_type);
		return (absolutemax > 0 ? absolutemax : 1);
	}

	void construct( pointer p, const value_type& val )
	{ 
		new(static_cast<void*>(p)) value_type(val);
	}

	void destroy( pointer p )
	{
		p->~value_type();
	}
};

//----------------------------------------------------------------------------------------
//
//					ThFrameworkAlignedAllocator
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* STL allocator template for the standard library containers,
* interfaces into the framework aligned memory allocation infrastructure.
* 
*/
template<typename T, ThSize alignment>
class ThFrameworkAlignedAllocator 
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template<class U> 
	struct rebind
	{
		typedef ThFrameworkAlignedAllocator<U, alignment> other;
	};

	ThFrameworkAlignedAllocator() throw() {}

	ThFrameworkAlignedAllocator( const ThFrameworkAlignedAllocator& ) throw() {}

	template<typename U> 
	ThFrameworkAlignedAllocator(const ThFrameworkAlignedAllocator<U, alignment>&) throw() {}

	pointer address(reference x)const
	{
		return &x;
	}

	const_pointer address(const_reference x) const
	{
		return &x;
	}

	//! Allocate space for n objects.
	pointer allocate( size_type n, const void* /*hint*/ =0 )
	{
		return static_cast<pointer>( ThMemory::AlignedMalloc( n * sizeof(value_type), alignment ) );
	}

	//! Free previously allocated block of memory
	void deallocate( pointer p, size_type )
	{
		ThMemory::AlignedFree( p );
	}

	//! Largest value for which method allocate might succeed.
	size_type max_size() const throw()
	{
		size_type absolutemax = static_cast<size_type>(-1) / sizeof (value_type);
		return (absolutemax > 0 ? absolutemax : 1);
	}

	void construct( pointer p, const value_type& val )
	{ 
		new(static_cast<void*>(p)) value_type(val);
	}

	void destroy( pointer p )
	{
		p->~value_type();
	}
};
	
}//Thor