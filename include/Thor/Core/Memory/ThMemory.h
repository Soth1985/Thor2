#pragma once

#include <Thor/Core/Common.h>

namespace Thor{

//----------------------------------------------------------------------------------------
//
//					ThMalloc
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Defines memory allocation functions, used by the engine.
*
* 
*/
class ThMemory
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
    
    static constexpr ThSize Kilobyte()
    {
        return 1024;
    }
    
    static constexpr ThSize Megabyte()
    {
        return 1024 * Kilobyte();
    }
    
    static constexpr ThSize Gigabyte()
    {
        return 1024 * Megabyte();
    }

	/*!
	* \brief
	* Copies numBytes from destination into source buffer.
	* 
	*/
	static void* MemoryCopy(void* destination, const void* source, ThSize numBytes);
    
    static void* MemoryMove(void* destination, const void* source, ThSize numBytes);

	static void MemorySet(void* destination, ThU8 value, ThSize numBytes);

	static void StringCopy(ThChar* destination, ThSize destinationSize, const ThChar* source);
};
	
}//Thor
