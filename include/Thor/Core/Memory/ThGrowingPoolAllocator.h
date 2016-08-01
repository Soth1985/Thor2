#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThPoolAllocator.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{
    class ThGrowingPoolAllocator : public ThiMemoryAllocator
    {
    public:
        
        static const ThSize MIN_NUM_CHUNKS = 16;
        static const ThSize MAX_NUM_CHUNKS = 256;
        
        ThGrowingPoolAllocator(const char* name = nullptr);
        ~ThGrowingPoolAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize chunkSize, ThSize numChunks, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        
        void InitToPageSize(ThSize chunkSize, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        
        void Shrink();
        bool IsInPool(ThU8* ptr);
        ThiMemoryAllocator* GetParentAllocator();
    private:
        ThPoolAllocator* AddPool();
        
        ThVector<ThPoolAllocator*> m_Pools;
        ThPoolAllocator* m_AllocPool;
        ThPoolAllocator* m_DeallocPool;
    };
}