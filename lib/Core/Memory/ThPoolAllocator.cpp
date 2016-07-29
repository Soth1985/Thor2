#include <Thor/Core/Memory/ThPoolAllocator.h>

using namespace Thor;

ThPoolAllocator::ThPoolAllocator(const char* name)
    :
m_Memory(nullptr),
m_NumChunks(0),
m_ChunkSize(0),
m_Alignment(0),
m_FirstFreeBlock(0),
m_NumFreeBlocks(0),
m_Parent(nullptr)
{
    
}

ThPoolAllocator::~ThPoolAllocator()
{
    
}

void* ThPoolAllocator::Allocate(ThSize size, ThU32 alignment)
{
    return nullptr;
}

void ThPoolAllocator::Deallocate(void* ptr)
{
    
}

ThSize ThPoolAllocator::GetCapacity()
{
    return m_ChunkSize * m_NumChunks;
}

ThSize ThPoolAllocator::GetTotalAllocated()
{
    return m_ChunkSize * (m_NumChunks - m_NumFreeBlocks);
}

void ThPoolAllocator::Init(ThSize chunkSize, ThSize numChunks, ThSize alignment, ThU8* memory, ThiMemoryAllocator* parent)
{
    m_ChunkSize = chunkSize;
    m_NumChunks = numChunks;
}