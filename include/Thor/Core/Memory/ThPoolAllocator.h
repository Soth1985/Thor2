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
        
        void Init(ThSize chunkSize, ThSize numChunks, ThSize alignment, ThiMemoryAllocator* parent = nullptr);
        void Reset();
        void Release();
        bool IsFilled()const;
        ThSize GetChunkSize()const;
        ThSize GetNumChunks()const;
        bool IsInPool(ThU8* ptr)const;
    private:
        ThU8* m_Memory;
        ThSize m_NumChunks;
        ThSize m_ChunkSize;
        ThSize m_Alignment;
        ThSize m_FirstFreeChunk;
        ThSize m_NumFreeChunks;
        ThiMemoryAllocator* m_Parent;
    };
}