#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

namespace Thor
{
    class ThPoolAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThPoolAllocator(const char* name = nullptr);
        ~ThPoolAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize chunkSize, ThSize numChunks, ThSize alignment, ThU8* memory = nullptr, ThiMemoryAllocator* parent = nullptr);
    private:
        ThU8* m_Memory;
        ThSize m_NumChunks;
        ThSize m_ChunkSize;
        ThSize m_Alignment;
        ThU32 m_FirstFreeBlock;
        ThU32 m_NumFreeBlocks;
        ThiMemoryAllocator* m_Parent;
    };
}