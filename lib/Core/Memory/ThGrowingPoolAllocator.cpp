#include <Thor/Core/Memory/ThGrowingPoolAllocator.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThGrowingPoolAllocator::ThGrowingPoolAllocator(const char* name)
    :
ThiMemoryAllocator(name),
m_AllocPool(nullptr),
m_DeallocPool(nullptr)
{
    
}

ThGrowingPoolAllocator::~ThGrowingPoolAllocator()
{
    if (m_Pools.Empty())
        return;
    
    ThiMemoryAllocator* allocator = m_Pools[0]->GetParentAllocator();
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
    {
        FreeObject(allocator, m_Pools[i]);
    }
}

void* ThGrowingPoolAllocator::Allocate(ThSize size, ThU32 alignment)
{
    THOR_ASSERT(m_Pools.Size() > 0, "No pools to allocate from");
    
    if (!m_AllocPool->IsFilled())
    {
        return m_AllocPool->Allocate(size, alignment);
    }
    else
    {
        ThPoolAllocator* freePool = 0;
        
        for (ThSize i = 0; i < m_Pools.Size(); ++i)
        {
            if (!m_Pools[i]->IsFilled())
            {
                freePool = m_Pools[i];
                break;
            }
        }
        
        if (!freePool)
        {
            freePool = AddPool();
            freePool->Init(m_Pools[0]->GetChunkSize(), m_Pools[0]->GetNumChunks(), m_Pools[0]->GetAlignment(), m_Pools[0]->GetParentAllocator());
        }
        
        m_AllocPool = freePool;
        return m_AllocPool->Allocate(size, alignment);
    }
}

void ThGrowingPoolAllocator::Deallocate(void* ptr)
{
    ThU8* ptrU8 = (ThU8*)ptr;
    
    if (m_DeallocPool->IsInPool(ptrU8))
    {
        m_DeallocPool->Deallocate(ptr);
    }
    else
    {
        ThPoolAllocator* targetPool = 0;
        
        for (ThSize i = 0; i < m_Pools.Size(); ++i)
        {
            if (m_Pools[i]->IsInPool(ptrU8))
            {
                targetPool = m_Pools[i];
                break;
            }
        }
        
        if (targetPool)
        {
            m_DeallocPool = targetPool;
            m_DeallocPool->Deallocate(ptr);
        }
        else
        {
            THOR_WRN("Pointer is not inside the pool %s", coreSysLogTag, GetName());
        }
    }
}

ThSize ThGrowingPoolAllocator::GetCapacity()
{
    ThSize result = 0;
    
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
        result += m_Pools[i]->GetCapacity();
    
    return result;
}

ThSize ThGrowingPoolAllocator::GetTotalAllocated()
{
    ThSize result = 0;
    
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
        result += m_Pools[i]->GetTotalAllocated();
    
    return result;
}

void ThGrowingPoolAllocator::Init(ThSize chunkSize, ThSize numChunks, ThSize alignment, ThiMemoryAllocator* parent)
{
    if (!m_Pools.Empty())
    {
        THOR_WRN("Allocator %s is already initialized", coreSysLogTag, GetName());
        return;
    }
    
    if (!parent)
        parent = ThAllocators::Instance().GetSystemMemoryAllocator();
    
    ThPoolAllocator* pool = AllocateObject<ThPoolAllocator>(parent, GetName());
    m_Pools.PushBack(pool);
    pool->Init(chunkSize, numChunks, alignment, parent);
    m_AllocPool = pool;
    m_DeallocPool = m_AllocPool;
}

void ThGrowingPoolAllocator::InitToPageSize(ThSize chunkSize, ThSize alignment, ThiMemoryAllocator* parent)
{
    if (!m_Pools.Empty())
    {
        THOR_WRN("Allocator %s is already initialized", coreSysLogTag, GetName());
        return;
    }
    
    if (!parent)
        parent = ThAllocators::Instance().GetSystemMemoryAllocator();
    
    ThSize pageSize = ThMemory::GetPageSize();
    ThSize numChunks = pageSize / chunkSize;
    
    if (numChunks < MIN_NUM_CHUNKS)
        numChunks = MIN_NUM_CHUNKS;
    else if (numChunks > MAX_NUM_CHUNKS)
        numChunks = MAX_NUM_CHUNKS;
    
    Init(chunkSize, numChunks, alignment, parent);
}

ThPoolAllocator* ThGrowingPoolAllocator::AddPool()
{
    ThPoolAllocator* pool = AllocateObject<ThPoolAllocator>(m_Pools[0]->GetParentAllocator(), GetName());
    m_Pools.PushBack(pool);
    return pool;
}


void ThGrowingPoolAllocator::Shrink()
{
    for (ThSize i = 0; i < m_Pools.Size();)
    {
        if (m_Pools[i]->IsFree() && (m_Pools.Size() > 1))
        {
            FreeObject(m_Pools[0]->GetParentAllocator(), m_Pools[i]);
            m_Pools.MoveToBackAndRemove(i);
        }
        else
            ++i;
    }
}

bool ThGrowingPoolAllocator::IsInPool(ThU8* ptr)
{
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
    {
        if (m_Pools[i]->IsInPool(ptr))
            return true;
    }
    return false;
}

ThiMemoryAllocator* ThGrowingPoolAllocator::GetParentAllocator()
{
    THOR_ASSERT(m_Pools.Size() > 0, "Uninitialized pool");
    return m_Pools[0]->GetParentAllocator();
}