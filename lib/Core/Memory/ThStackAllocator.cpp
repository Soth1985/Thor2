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

ThStackAllocator::~ThStackAllocator()
{
    m_Parent->Deallocate(m_Memory);
}

void ThStackAllocator::Init(ThSize size, ThSize alignment, ThiMemoryAllocator* parent)
{
    THOR_ASSERT(size > 0, "Invalid size");
    THOR_ASSERT(size % alignment == 0, "Size must be in multiples of alignment");
    THOR_ASSERT(parent, "Parent allocator must be provided");
    
    m_Size = size;
    m_Alignment = alignment;
    
    if (parent)
        m_Parent = parent;
    else
        m_Parent = ThAllocators::Instance().GetSystemMemoryAllocator();
    
    m_Memory = (ThU8*)m_Parent->Allocate(m_Size, m_Alignment);
    
    THOR_ASSERT(m_Memory != nullptr, "Failed to allocate memory");
}

void* ThStackAllocator::Allocate(ThSize size, ThU32 alignment)
{
    THOR_ASSERT(m_Alignment == alignment, "Invalid alignment");
    
    ThU8* currentPos = &m_Memory[m_Marker];
    ThSize newMarker = m_Marker + size + (alignment - size % alignment);
    if (newMarker > m_Size)
    {
        THOR_WRN("Stack allocator %s is out of memory", coreSysLogTag, GetName());
        return nullptr;
    }
    else
    {
        m_Marker = newMarker;
        return currentPos;
    }
}

void ThStackAllocator::Deallocate(void* ptr)
{
        
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