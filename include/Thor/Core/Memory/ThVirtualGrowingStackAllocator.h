#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThVirtualStackAllocator.h>

namespace Thor
{
    class ThVirtualGrowingStackAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThVirtualGrowingStackAllocator(const char* name = nullptr);
        ~ThVirtualGrowingStackAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize size, ThSize alignment = DEFAULT_ALIGNMENT);
        ThSize GetMarker()const;
        void FreeToMarker(ThSize Marker);
        void Reset();
        
    private:
        
        ThVirtualStackAllocator* m_StackAllocator;
    };
}

