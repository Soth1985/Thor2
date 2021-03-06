#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

namespace Thor
{
    class ThStackAllocator : public ThiMemoryAllocator
    {
    public:
        
        ThStackAllocator(const char* name = nullptr);
        ~ThStackAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT)override;
        virtual void Deallocate(void* ptr)override;
        virtual ThSize GetCapacity()override;
        virtual ThSize GetTotalAllocated()override;
        
        void Init(ThSize size, ThSize alignment = DEFAULT_ALIGNMENT, ThiMemoryAllocator* parent = nullptr);
        ThSize GetMarker()const;
        void FreeToMarker(ThSize Marker);
        void Reset();
        
    private:
        ThU8* m_Memory;
        ThiMemoryAllocator* m_Parent;
        ThSize m_Size;
        ThSize m_Alignment;
        ThSize m_Marker;
    };
}