#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

namespace Thor
{
    class ThVirtualStackAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThVirtualStackAllocator(const char* name = nullptr);
        ~ThVirtualStackAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize size, ThSize alignment = DEFAULT_ALIGNMENT);
        ThSize GetMarker()const;
        void FreeToMarker(ThSize Marker);
        void Reset();
        
    private:
        
        void AllocatePage();
        
        ThU8* m_BaseAddress;
        ThU8* m_NextPage;
        ThSize m_PageSize;
        ThSize m_Size;
        ThSize m_Alignment;
        ThSize m_Marker;
    };
}