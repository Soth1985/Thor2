#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThPoolAllocator.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{
    class ThGrowingPoolAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThGrowingPoolAllocator(const char* name = nullptr);
        ~ThGrowingPoolAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize chunkSize, ThSize numChunks, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        
        void InitToPageSize(ThSize chunkSize, ThSize minNumChunks = 16, ThSize maxNumChunks = 256, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        
        void Shrink();
    private:
        ThPoolAllocator* AddPool();
        
        ThVector<ThPoolAllocator> m_Pools;
        ThPoolAllocator* m_AllocPool;
        ThPoolAllocator* m_DeallocPool;
    };
}