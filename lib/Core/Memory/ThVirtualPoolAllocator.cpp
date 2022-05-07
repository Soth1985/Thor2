#include <Thor/Core/Memory/ThVirtualPoolAllocator.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThVirtualPoolAllocator::ThVirtualPoolAllocator(const char* name)
:
m_BaseAddress(nullptr),
m_NextPage(nullptr),
m_NumChunks(0),
m_NumChunksPerPage(0),
m_ChunkSize(0),
m_Alignment(0),
m_FirstFreeChunk(0),
m_NumFreeChunks(0),
m_PageSize(0)
{
    
}

ThVirtualPoolAllocator::~ThVirtualPoolAllocator()
{
    ThMemory::VmFreeMemory(m_BaseAddress, m_ChunkSize * m_NumChunks);
}

void* ThVirtualPoolAllocator::Allocate(ThSize size, ThU32 alignment)
{
    THOR_ASSERT(size <= m_ChunkSize, "Size of allocated block should not exceed chunk size");
    THOR_ASSERT(alignment == m_Alignment, "Invalid alignment");
    
    if (m_NumFreeChunks > 0)
    {
        --m_NumFreeChunks;
        ThU8* memory = m_BaseAddress + m_FirstFreeChunk * m_ChunkSize;
        
        if (memory >= m_NextPage)
            AllocatePage();
        
        m_FirstFreeChunk = *(ThSize*)memory;
        return memory;
    }
    else
    {
        THOR_WRN("Pool %s is out of space.", ThLogger::TagSystem, GetName());
        return nullptr;
    }
}

void ThVirtualPoolAllocator::Deallocate(void* ptr)
{
    ThU8* ptrU8 = (ThU8*)ptr;
    THOR_ASSERT(IsInCommittedPool(ptrU8), "Pointer is not inside the pool");
    ThSize diff = ptrU8 - m_BaseAddress;
    ThU32 index = diff / m_ChunkSize;
    THOR_ASSERT(diff % m_ChunkSize == 0, "Pointer is not aligned to chunk size");
    THOR_ASSERT(index != m_FirstFreeChunk, "Attempting to deallocate same chunk twice");
    *(ThSize*)ptrU8 = m_FirstFreeChunk;
    m_FirstFreeChunk = index;
    ++m_NumFreeChunks;
}

ThSize ThVirtualPoolAllocator::GetCapacity()
{
    return m_ChunkSize * m_NumChunks;
}

ThSize ThVirtualPoolAllocator::GetTotalAllocated()
{
    return m_ChunkSize * (m_NumChunks - m_NumFreeChunks);
}

void ThVirtualPoolAllocator::Init(ThSize chunkSize, ThSize maxMemoryUsage, ThSize alignment)
{
    if (m_BaseAddress != nullptr)
    {
        THOR_WRN("Allocator %s is already initialized", ThLogger::TagSystem, GetName());
        return;
    }
    
    m_PageSize = ThMemory::GetPageSize();
    m_NumChunks = maxMemoryUsage / chunkSize;
    m_NumFreeChunks = m_NumChunks;
    m_NumChunksPerPage = m_PageSize / chunkSize;
    
    THOR_ASSERT(chunkSize * m_NumChunks > 0, "Invalid pool size");
    THOR_ASSERT(chunkSize >= sizeof (ThU32), "Chunk size is too small");
    THOR_ASSERT(chunkSize % alignment == 0, "Chunk size must be in multiples of alignment");
    THOR_ASSERT((chunkSize * m_NumChunks) % alignment == 0, "Pool size must be in multiples of alignment");
    
    m_ChunkSize = chunkSize;
    m_Alignment = alignment;
    m_BaseAddress = (ThU8*)ThMemory::VmReserveMemory(m_ChunkSize * m_NumChunks);
    m_NextPage = m_BaseAddress;
    
    THOR_ASSERT(m_BaseAddress != nullptr, "Failed to allocate memory");
}

void ThVirtualPoolAllocator::Reset()
{
    m_FirstFreeChunk = 0;
    m_NumFreeChunks = m_NumChunks;
    ThU8* memPos = m_BaseAddress;
    
    for (ThSize i = 0; i < m_NumChunks; ++i, memPos += m_ChunkSize)
    {
        *memPos = i + 1;
        
        if((memPos + m_ChunkSize) >= m_NextPage)
            break;
    }
}

void ThVirtualPoolAllocator::Release()
{
    ThMemory::VmDecommitMemory(m_BaseAddress, m_NextPage - m_BaseAddress);
}

bool ThVirtualPoolAllocator::IsFilled()const
{
    return m_NumFreeChunks == 0;
}

bool ThVirtualPoolAllocator::IsFree()const
{
    return m_NumFreeChunks == m_NumChunks;
}

ThSize ThVirtualPoolAllocator::GetChunkSize()const
{
    return m_ChunkSize;
}

ThSize ThVirtualPoolAllocator::GetNumChunks()const
{
    return m_NumChunks;
}

ThSize ThVirtualPoolAllocator::GetAlignment()const
{
    return m_Alignment;
}

bool ThVirtualPoolAllocator::IsInPool(ThU8* ptr)const
{
    if ((ptr >= m_BaseAddress) && (ptr < (m_BaseAddress + m_ChunkSize * m_NumChunks)))
        return true;
    else
        return false;
}

bool ThVirtualPoolAllocator::IsInCommittedPool(ThU8* ptr)const
{
    if ((ptr >= m_BaseAddress) && (ptr < m_NextPage))
        return true;
    else
        return false;
}

void ThVirtualPoolAllocator::AllocatePage()
{
    ThMemory::VmCommitMemory(m_NextPage, m_PageSize);
    m_NextPage += m_PageSize;
    ThSize pageCount = ( m_NextPage - m_BaseAddress + 1 ) / m_PageSize;
    
    m_FirstFreeChunk = (pageCount - 1) * m_NumChunksPerPage;
    ThSize lastFreeChunk = m_FirstFreeChunk + m_NumChunksPerPage;
    ThU8* memPos = m_BaseAddress + (pageCount - 1) * m_NumChunksPerPage * m_ChunkSize;
    
    for (ThSize i = m_FirstFreeChunk; i < lastFreeChunk; ++i, memPos += m_ChunkSize)
    {
        *memPos = i + 1;
        
        if((memPos + m_ChunkSize) >= m_NextPage)
            break;
    }
}
