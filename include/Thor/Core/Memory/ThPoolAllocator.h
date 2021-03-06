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
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize chunkSize, ThSize numChunks, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        void Reset();
        void Release();
        bool IsFilled()const;
        bool IsFree()const;
        ThSize GetChunkSize()const;
        ThU32 GetNumChunks()const;
        ThSize GetAlignment()const;
        ThiMemoryAllocator* GetParentAllocator()const;
        bool IsInPool(ThU8* ptr)const;
    private:
        ThU8* m_Memory;
        ThiMemoryAllocator* m_Parent;
        ThSize m_ChunkSize;
        ThSize m_Alignment;
        ThU32 m_NumChunks;
        ThU32 m_FirstFreeChunk;
        ThU32 m_NumFreeChunks;
    };
}