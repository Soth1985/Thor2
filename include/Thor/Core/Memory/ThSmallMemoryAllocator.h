#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThGrowingPoolAllocator.h>

namespace Thor
{
    class ThSmallMemoryAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThSmallMemoryAllocator(const char* name = nullptr);
        ~ThSmallMemoryAllocator();
        
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