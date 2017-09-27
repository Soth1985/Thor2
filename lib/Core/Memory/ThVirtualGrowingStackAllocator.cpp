#include <Thor/Core/Memory/ThVirtualGrowingStackAllocator.h>
#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThVirtualGrowingStackAllocator::ThVirtualGrowingStackAllocator(const char* name)
:
ThiMemoryAllocator(name)
{
    m_StackAllocator = new ThVirtualStackAllocator();
}

ThVirtualGrowingStackAllocator::~ThVirtualGrowingStackAllocator()
{
    delete m_StackAllocator;
}

void* ThVirtualGrowingStackAllocator::Allocate(ThSize size, ThU32 alignment)
{
    void* result = m_StackAllocator->Allocate(size, alignment);
    
    if (result == nullptr)
    {
        ThSize allocatedSize = m_StackAllocator->GetTotalAllocated();
        ThSize newSize = m_StackAllocator->GetCapacity() * 2;
        ThVirtualStackAllocator* newAllocator = new ThVirtualStackAllocator();
        newAllocator->Init(newSize, alignment);
        newAllocator->Allocate(allocatedSize);
        ThMemory::MemoryCopy(newAllocator->GetBaseAddress(), m_StackAllocator->GetBaseAddress(), allocatedSize);
        delete m_StackAllocator;
        m_StackAllocator = newAllocator;
        return Allocate(size, alignment);
    }
    else
    {
        return result;
    }
}

void ThVirtualGrowingStackAllocator::Deallocate(void* ptr)
{
    
}

ThSize ThVirtualGrowingStackAllocator::GetCapacity()
{
    return m_StackAllocator->GetCapacity();
}

ThSize ThVirtualGrowingStackAllocator::GetTotalAllocated()
{
    return m_StackAllocator->GetTotalAllocated();
}

void ThVirtualGrowingStackAllocator::Init(ThSize size, ThSize alignment)
{
    m_StackAllocator->Init(size, alignment);
}

ThSize ThVirtualGrowingStackAllocator::GetMarker()const
{
    return m_StackAllocator->GetMarker();
}

void ThVirtualGrowingStackAllocator::FreeToMarker(ThSize Marker)
{
    m_StackAllocator->FreeToMarker(Marker);
}

void ThVirtualGrowingStackAllocator::Reset()
{
    m_StackAllocator->Reset();
}
