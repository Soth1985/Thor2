#include <Thor/Core/Memory/ThStackAllocator.h>
#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThStackAllocator::ThStackAllocator(const char* name)
    :
ThiMemoryAllocator(name),
m_Memory(nullptr),
m_Parent(nullptr),
m_Size(0),
m_Alignment(0),
m_Marker(0)
{
    
}

void ThStackAllocator::Init(ThSize size, ThSize alignment, ThU8* memory, ThiMemoryAllocator* parent)
{
    THOR_ASSERT(size > 0, "Invalid size");
    THOR_ASSERT(size % alignment, "Size must be in multiples of alignment");
    THOR_ASSERT(memory || parent, "Parent allocator or memory block must be provided");
    
    m_Size = size;
    m_Alignment = alignment;
    
    if (parent)
        m_Parent = parent;
    else
        m_Parent = ThAllocators::Instance().GetSystemMemoryAllocator();
    
    if (memory)
    {
        m_Memory = memory;
        m_Parent = 0;
    }
    else
        m_Memory = (ThU8*)m_Parent->Allocate(m_Size, m_Alignment);
}

ThStackAllocator::~ThStackAllocator()
{
    if (m_Parent)
        m_Parent->Deallocate(m_Memory);
}

void* ThStackAllocator::Allocate(ThSize size, ThU32 alignment)
{
    THOR_ASSERT(m_Alignment == alignment, "Invalid alignment");
    
    ThU8* currentPos = &m_Memory[m_Marker];
    ThSize newMarker = m_Marker + size + (alignment - size % alignment);
    if (newMarker > m_Size)
    {
        if (m_Parent)
        {
            THOR_WRN("Stack allocator %s is out of memory, reverting to parent allocator", coreSysLogTag, GetName());
            return m_Parent->Allocate(size, alignment);
        }
        else
        {
            THOR_WRN("Stack allocator %s is out of memory", coreSysLogTag, GetName());
            return nullptr;
        }
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
        if (m_Parent)
            m_Parent->Deallocate(ptr);
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
    if (Marker >= m_Size)
        m_Marker = 0;
    else
        m_Marker = Marker;
}

void ThStackAllocator::Reset()
{
    m_Marker = 0;
}