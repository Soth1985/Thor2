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
            if (!m_Pools[i].IsFilled())
            {
                freePool = &m_Pools[i];
                break;
            }
        }
        
        if (!freePool)
        {
            freePool = AddPool();
            freePool->Init(m_Pools[0].GetChunkSize(), m_Pools[0].GetNumChunks(), m_Pools[0].GetAlignment(), m_Pools[0].GetParentAllocator());
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
            if (m_Pools[i].IsInPool(ptrU8))
            {
                targetPool = &m_Pools[i];
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
        result += m_Pools[i].GetCapacity();
    
    return result;
}

ThSize ThGrowingPoolAllocator::GetTotalAllocated()
{
    ThSize result = 0;
    
    for (ThSize i = 0; i < m_Pools.Size(); ++i)
        result += m_Pools[i].GetTotalAllocated();
    
    return result;
}

void ThGrowingPoolAllocator::Init(ThSize chunkSize, ThSize numChunks, ThSize alignment, ThiMemoryAllocator* parent)
{
    m_Pools.PushBack(ThPoolAllocator(GetName()));
    m_Pools.Back().Init(chunkSize, numChunks, alignment, parent);
    m_AllocPool = &m_Pools.Back();
    m_DeallocPool = m_AllocPool;
}

void ThGrowingPoolAllocator::InitToPageSize(ThSize chunkSize, ThSize minNumChunks, ThSize maxNumChunks, ThSize alignment, ThiMemoryAllocator* parent)
{
    ThSize pageSize = ThMemory::GetPageSize();
    ThSize numChunks = pageSize / chunkSize;
    
    if (numChunks < minNumChunks)
        numChunks = minNumChunks;
    else if (numChunks > maxNumChunks)
        numChunks = maxNumChunks;
    
    Init(chunkSize, numChunks, alignment, parent);
}

ThPoolAllocator* ThGrowingPoolAllocator::AddPool()
{
    m_Pools.PushBack(ThPoolAllocator(GetName()));
    return &m_Pools.Back();
}


void ThGrowingPoolAllocator::Shrink()
{
    for (ThSize i = 0; i < m_Pools.Size();)
    {
        if (m_Pools[i].IsFree() && (m_Pools.Size() > 1))
        {
            m_Pools.MoveToBackAndRemove(i);
        }
        else
            ++i;
    }
}