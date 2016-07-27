#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

#include <atomic>

namespace Thor
{
    class ThStackAllocator : public ThiMemoryAllocator
    {
    public:
        ThStackAllocator(ThSize size, const char* name = nullptr);
        ~ThStackAllocator();
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT);
        virtual void Deallocate(void* ptr);
        virtual ThSize GetCapacity();
        virtual ThSize GetTotalAllocated();
        ThSize GetMarker()const;
        void FreeToMarker(ThSize Marker);
        void Reset();
    private:
        ThU8* m_Memory;
        ThSize m_Size;
        ThSize m_Marker;
    };
}