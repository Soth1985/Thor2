#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThGrowingPoolAllocator.h>

namespace Thor
{
    class ThSmallMemoryAllocator : public ThiMemoryAllocator
    {
    public:
        
        static const ThSize MAX_OBJECT_SIZE = 512;
        
        ThSmallMemoryAllocator(const char* name = nullptr);
        ~ThSmallMemoryAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize maxObjectSize = MAX_OBJECT_SIZE, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        
        void Shrink();
    private:
        
        ThVector<ThGrowingPoolAllocator*> m_Pools;
        ThGrowingPoolAllocator* m_DeallocPool;
    };
}