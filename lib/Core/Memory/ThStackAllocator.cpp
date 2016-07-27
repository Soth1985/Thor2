#include <Thor/Core/Memory/ThStackAllocator.h>
#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThStackAllocator::ThStackAllocator(ThSize size, const char* name)
    :
ThiMemoryAllocator(name),
m_Size(size),
m_Marker(0)
{
    ThiMemoryAllocator* systemAllocator = ThAllocators::Instance().GetSystemMemoryAllocator();
    
    m_Memory = (ThU8*)systemAllocator->Allocate(m_Size);
}

ThStackAllocator::~ThStackAllocator()
{
    
}

void* ThStackAllocator::Allocate(ThSize size, ThU32 alignment)
{
    ThU8* currentPos = &m_Memory[m_Marker];
    ThSize newMarker = m_Marker + size + (alignment - size % alignment);
    if (newMarker > m_Marker)
    {
        THOR_WRN("Stack allocator %s is out of memory, reverting to system allocator", coreSysLogTag, GetName());
        ThiMemoryAllocator* systemAllocator = ThAllocators::Instance().GetSystemMemoryAllocator();
        return systemAllocator->Allocate(size, alignment);
    }
    else
    {
        m_Marker = newMarker;
        return currentPos;
    }
}

void ThStackAllocator::Deallocate(void* ptr)
{
    ThU8* ptrU8 = (ThU8*)ptr;
    ThU8* end = &m_Memory[m_Size];
    if (ptrU8 >= end)
    {
        ThiMemoryAllocator* systemAllocator = ThAllocators::Instance().GetSystemMemoryAllocator();
        systemAllocator->Deallocate(ptr);
    }
        
}

ThSize ThStackAllocator::GetCapacity()
{
    return m_Size;
}

ThSize ThStackAllocator::GetTotalAllocated()
{
    return m_Marker;
}

ThSize ThStackAllocator::GetMarker()const
{
    return m_Marker;
}

void ThStackAllocator::FreeToMarker(ThSize Marker)
{
    if (Marker >= m_Marker)
        m_Marker = 0;
    else
        m_Marker = m_Marker - Marker;
}

void ThStackAllocator::Reset()
{
    m_Marker = 0;
}