#include <Thor/Core/Memory/ThVirtualStackAllocator.h>
#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThVirtualStackAllocator::ThVirtualStackAllocator(const char* name)
    :
ThiMemoryAllocator(name),
m_BaseAddress(nullptr),
m_NextPage(nullptr),
m_PageSize(0),
m_Size(0),
m_Alignment(0),
m_Marker(0)
{
    
}

ThVirtualStackAllocator::~ThVirtualStackAllocator()
{
    ThMemory::VmFreeMemory(m_BaseAddress, m_Size);
}

void* ThVirtualStackAllocator::Allocate(ThSize size, ThU32 alignment)
{
    ThU8* currentPos = &m_BaseAddress[m_Marker];
    ThU8* endPos = &m_BaseAddress[m_Size];
    ThSize offset = size % alignment;
    
    if (offset != 0)
        offset = alignment - offset;
    else
        offset = 0;
    
    ThSize newMarker = m_Marker + size + offset;
    ThU8* newPos = &m_BaseAddress[newMarker];
    
    if (newPos > endPos)
    {
        THOR_WRN("Stack allocator %s is out of memory", coreSysLogTag, GetName());
        return nullptr;
    }
    else if (newPos >= m_NextPage)
    {
        ThMemory::VmCommitMemory(m_NextPage, m_PageSize);
        m_NextPage += m_PageSize;
        m_Marker = newMarker;
        return currentPos;
    }
    else
    {
        m_Marker = newMarker;
        return currentPos;
    }
}

void ThVirtualStackAllocator::Deallocate(void* ptr)
{
    
}

ThSize ThVirtualStackAllocator::GetCapacity()
{
    return m_Size;
}

ThSize ThVirtualStackAllocator::GetTotalAllocated()
{
    return m_Marker;
}

void ThVirtualStackAllocator::Init(ThSize size, ThSize alignment)
{
    if (m_BaseAddress != nullptr)
    {
        THOR_WRN("Allocator %s is already initialized", coreSysLogTag, GetName());
        return;
    }
    
    THOR_ASSERT(size > 0, "Invalid size");
    THOR_ASSERT(size % alignment == 0, "Size must be in multiples of alignment");
    
    m_Size = size;
    m_Alignment = alignment;
    m_PageSize = ThMemory::GetPageSize();
    m_BaseAddress = (ThU8*)ThMemory::VmReserveMemory(size);
    m_NextPage = m_BaseAddress;
}

ThSize ThVirtualStackAllocator::GetMarker()const
{
    return m_Marker;
}

void ThVirtualStackAllocator::FreeToMarker(ThSize Marker)
{
    if (Marker < m_Marker)
    {
        ThSize size = m_Marker - Marker;
        ThMemory::VmDecommitMemory(&m_BaseAddress[Marker - 1], size);
        m_Marker = Marker;
    }
}

void ThVirtualStackAllocator::Reset()
{
    ThSize size = (m_NextPage - m_BaseAddress);// / m_PageSize;
    ThMemory::VmDecommitMemory(m_BaseAddress, size);
    m_Marker = 0;
}