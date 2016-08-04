#pragma once

#include <Thor/Core/CoreExport.h>

#define THOR_DEFAULT_ALIGNMENT 16

namespace Thor{

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
	* Frees an aligned chunk of memory(allocated with ThMalloc::AlignedMalloc).
	*
	* \param ptr
	* Pointer to the chunk.
	* 
	*/
	static void AlignedFree(void* ptr);
    
    static ThSize MallocSize(void* ptr);
    
    static ThSize GetPageSize();
    
    static void* VmReserveMemory(ThSize size);
    
    static void* VmCommitMemory(void* ptr, ThSize size);
    
    static void VmDecommitMemory(void* ptr, ThSize size);
    
    static void VmFreeMemory(void* ptr, ThSize size);
    
    static constexpr ThSize Kilobyte();
    
    static constexpr ThSize Megabyte();
    
    static constexpr ThSize Gigabyte();

	/*!
	* \brief
	* Copies numBytes from destination into source buffer.
	* 
	*/
	static void* MemoryCopy(void* destination, const void* source, ThSize numBytes);
};
	
}//Thor