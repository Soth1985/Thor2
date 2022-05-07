#include <Thor/Core/Memory/ThSmallMemoryAllocator.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThSmallMemoryAllocator::ThSmallMemoryAllocator(const char* name)
    :
ThiMemoryAllocator(name),
m_DeallocPool(nullptr),
m_MaxObjectSize(0)
{
    
}

ThSmallMemoryAllocator::~ThSmallMemoryAllocator()
{
    if (m_Pools.Empty())
        return;
    
    ThiMemoryAllocator* allocator = m_Pools[0]->GetParentAllocator();
    for (ThSize i = 0;  i < m_Pools.Size(); ++i)
    {
        FreeObject(allocator, m_Pools[i]);
    }
}

void* ThSmallMemoryAllocator::Allocate(ThSize size, ThU32 alignment)
{
    THOR_ASSERT(m_Pools.Size() > 0, "No pools to allocate from");
    THOR_ASSERT(size <= m_MaxObjectSize, "Invalid size");
    ThSize poolIndex = (size + alignment - 1) / alignment;
    
    if (poolIndex < m_Pools.Size())
    {
        return m_Pools[poolIndex]->Allocate(size, alignment);
    }
    else
    {
        THOR_WRN("Invalid object size for allocator %s", ThLogger::TagSystem, GetName());
        return nullptr;
    }
}

void ThSmallMemoryAllocator::Deallocate(void* ptr)
{
    ThU8* ptrU8 = (ThU8*)ptr;
    
    if (m_DeallocPool && m_DeallocPool->IsInPool(ptrU8))
        m_DeallocPool->Deallocate(ptr);
    else
    {
        for (ThSize i = 0; i < m_Pools.Size(); ++i)
        {
            if (m_Pools[i]->IsInPool(ptrU8))
            {
                m_DeallocPool = m_Pools[i];
                m_DeallocPool->Deallocate(ptr);
            }
        }
        
        THOR_WRN("Pointer is not inside the managed memory %s", ThLogger::TagSystem, GetName());
    }
}

ThSize ThSmallMemoryAllocator::GetCapacity()
{
    ThSize result = 0;
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
    {
        result += m_Pools[i]->GetCapacity();
    }
    return result;
}

ThSize ThSmallMemoryAllocator::GetTotalAllocated()
{
    ThSize result = 0;
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
    {
        result += m_Pools[i]->GetTotalAllocated();
    }
    return result;
}

void ThSmallMemoryAllocator::Init(ThSize maxObjectSize, ThSize alignment, ThiMemoryAllocator* parent)
{
    if (!m_Pools.Empty())
    {
        THOR_WRN("Allocator %s is already initialized", ThLogger::TagSystem, GetName());
        return;
    }
    
    THOR_ASSERT(maxObjectSize <= MAX_OBJECT_SIZE, "Object size is too large");
    THOR_ASSERT(maxObjectSize > 0, "Size must be positive number");
    THOR_ASSERT(maxObjectSize % alignment == 0, "maxObjectSize must be aligned");
    
    if (!parent)
        parent = ThCore::GetSystemAllocator();
    
    if (maxObjectSize > MAX_OBJECT_SIZE)
        maxObjectSize = MAX_OBJECT_SIZE;
    
    m_MaxObjectSize = maxObjectSize;
    ThSize numPools = maxObjectSize / alignment;
    m_Pools.Reserve(numPools);
    
    for (ThSize i = 0; i < numPools; ++i)
    {
        ThGrowingPoolAllocator* pool = AllocateObject<ThGrowingPoolAllocator>(parent, GetName());
        pool->InitToPageSize((i + 1) * alignment, alignment, parent);
        m_Pools.PushBack(pool);
    }
}

void ThSmallMemoryAllocator::Shrink()
{
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
    {
        m_Pools[i]->Shrink();
    }
}
