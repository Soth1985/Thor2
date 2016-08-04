#include <Thor/Core/Memory/ThPoolAllocator.h>
#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThPoolAllocator::ThPoolAllocator(const char* name)
    :
m_Memory(nullptr),
m_Parent(nullptr),
m_NumChunks(0),
m_ChunkSize(0),
m_Alignment(0),
m_FirstFreeChunk(0),
m_NumFreeChunks(0)
{
    
}

ThPoolAllocator::~ThPoolAllocator()
{
    Release();
}

void* ThPoolAllocator::Allocate(ThSize size, ThU32 alignment)
{
    THOR_ASSERT(size <= m_ChunkSize, "Size of allocated block should not exceed chunk size");
    THOR_ASSERT(alignment == m_Alignment, "Invalid alignment");
    
    if (m_NumFreeChunks > 0)
    {
        --m_NumFreeChunks;
        ThU8* memory = m_Memory + m_FirstFreeChunk * m_ChunkSize;
        m_FirstFreeChunk = *(ThU32*)memory;
        return memory;
    }
    else
    {
        THOR_WRN("Pool %s is out of space.", coreSysLogTag, GetName());
        return nullptr;
    }
}

void ThPoolAllocator::Deallocate(void* ptr)
{
    ThU8* ptrU8 = (ThU8*)ptr;
    THOR_ASSERT(IsInPool(ptrU8), "Pointer is not inside the pool");
    ThSize diff = ptrU8 - m_Memory;
    ThU32 index = diff / m_ChunkSize;
    THOR_ASSERT(diff % m_ChunkSize == 0, "Pointer is not aligned to chunk size");
    THOR_ASSERT(index != m_FirstFreeChunk, "Attempting to deallocate same chunk twice");
    *(ThU32*)ptrU8 = m_FirstFreeChunk;
    m_FirstFreeChunk = index;
    ++m_NumFreeChunks;
}

ThSize ThPoolAllocator::GetCapacity()
{
    return m_ChunkSize * m_NumChunks;
}

ThSize ThPoolAllocator::GetTotalAllocated()
{
    return m_ChunkSize * (m_NumChunks - m_NumFreeChunks);
}

void ThPoolAllocator::Init(ThSize chunkSize, ThSize numChunks, ThSize alignment, ThiMemoryAllocator* parent)
{
    if (m_Memory != nullptr)
    {
        THOR_WRN("Allocator %s is already initialized", coreSysLogTag, GetName());
        return;
    }
    
    THOR_ASSERT(chunkSize * numChunks > 0, "Invalid pool size");
    THOR_ASSERT(chunkSize >= sizeof (ThU32), "Chunk size is too small");
    THOR_ASSERT(chunkSize % alignment == 0, "Chunk size must be in multiples of alignment");
    THOR_ASSERT((chunkSize * numChunks) % alignment == 0, "Pool size must be in multiples of alignment");
    THOR_ASSERT(parent, "Parent allocator must be provided");
    
    m_ChunkSize = chunkSize;
    m_NumChunks = numChunks;
    m_Alignment = alignment;
    m_Parent = parent;
    
    if (parent)
        m_Parent = parent;
    else
        m_Parent = ThAllocators::Instance().GetSystemMemoryAllocator();
    
    m_Memory = (ThU8*)m_Parent->Allocate(m_ChunkSize * m_NumChunks, m_Alignment);
    
    THOR_ASSERT(m_Memory != nullptr, "Failed to allocate memory");
    
    Reset();
}

void ThPoolAllocator::Reset()
{
    m_FirstFreeChunk = 0;
    m_NumFreeChunks = m_NumChunks;
    ThU8* memPos = m_Memory;
    
    for (ThU32 i = 0; i < m_NumChunks; ++i, memPos += m_ChunkSize)
    {
        *memPos = i + 1;
    }
}

void ThPoolAllocator::Release()
{
    m_Parent->Deallocate(m_Memory);
    m_Memory = nullptr;
}

bool ThPoolAllocator::IsFilled()const
{
    return m_NumFreeChunks == 0;
}

bool ThPoolAllocator::IsFree()const
{
    return m_NumFreeChunks == m_NumChunks;
}

ThSize ThPoolAllocator::GetChunkSize()const
{
    return m_ChunkSize;
}

ThU32 ThPoolAllocator::GetNumChunks()const
{
    return m_NumChunks;
}

ThSize ThPoolAllocator::GetAlignment()const
{
    return m_Alignment;
}

ThiMemoryAllocator* ThPoolAllocator::GetParentAllocator()const
{
    return m_Parent;
}

bool ThPoolAllocator::IsInPool(ThU8* ptr)const
{
    if ((ptr >= m_Memory) && (ptr < (m_Memory + m_ChunkSize * m_NumChunks)))
        return true;
    else
        return false;
}