#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

namespace Thor
{
    class ThVirtualPoolAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThVirtualPoolAllocator(const char* name = nullptr);
        ~ThVirtualPoolAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize chunkSize, ThSize maxMemoryUsage, ThSize alignment = DEFAULT_ALIGNMENT);
        void Reset();
        void Release();
        bool IsFilled()const;
        bool IsFree()const;
        ThSize GetChunkSize()const;
        ThSize GetNumChunks()const;
        ThSize GetAlignment()const;
        bool IsInPool(ThU8* ptr)const;
        bool IsInCommittedPool(ThU8* ptr)const;
        
    private:
        
        void AllocatePage();
        
        ThU8* m_BaseAddress;
        ThU8* m_NextPage;
        ThU32 m_NumChunks;
        ThU32 m_NumChunksPerPage;
        ThU32 m_FirstFreeChunk;
        ThU32 m_NumFreeChunks;
        ThSize m_ChunkSize;
        ThSize m_Alignment;        
        ThSize m_PageSize;
    };
}
