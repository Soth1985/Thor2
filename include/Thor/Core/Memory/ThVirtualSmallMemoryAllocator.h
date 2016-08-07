#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThVirtualPoolAllocator.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{
    class ThVirtualSmallMemoryAllocator : public ThiMemoryAllocator
    {
    public:
        
        static const ThSize MAX_OBJECT_SIZE = 512;
        
        ThVirtualSmallMemoryAllocator(const char* name = nullptr);
        ~ThVirtualSmallMemoryAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize maxPoolSize, ThSize maxObjectSize = MAX_OBJECT_SIZE, ThSize alignment = DEFAULT_ALIGNMENT);
        
    private:
        
        ThVector<ThVirtualPoolAllocator*> m_Pools;
        ThVirtualPoolAllocator* m_DeallocPool;
        ThSize m_MaxObjectSize;
    };
}