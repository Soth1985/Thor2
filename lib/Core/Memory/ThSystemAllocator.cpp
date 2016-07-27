#include <Thor/Core/Memory/ThSystemAllocator.h>
#include <Thor/Core/Memory/ThMemory.h>

using namespace Thor;

ThSystemAllocator::ThSystemAllocator(const char* name)
    :
ThiMemoryAllocator(name),
m_Allocated(0)
{
    
}

void* ThSystemAllocator::Allocate(ThSize size, ThU32 alignment)
{
    void* ptr = ThMemory::AlignedMalloc(size, alignment);
    m_Allocated += size;
    return ptr;
}

void ThSystemAllocator::Deallocate(void* ptr)
{
    ThSize size = ThMemory::MallocSize(ptr);
    ThMemory::AlignedFree(ptr);
    m_Allocated -= size;
}

ThSize ThSystemAllocator::GetCapacity()
{
    return ThiMemoryAllocator::SIZE_UNTRACKED;
}

ThSize ThSystemAllocator::GetTotalAllocated()
{
    return m_Allocated;
}