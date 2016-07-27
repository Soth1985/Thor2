#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

#include <atomic>

namespace Thor
{
    class ThSystemAllocator : public ThiMemoryAllocator
    {
    public:
        ThSystemAllocator(const char* name = nullptr);
        
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT);
        virtual void Deallocate(void* ptr);
        virtual ThSize GetCapacity();
        virtual ThSize GetTotalAllocated();
    private:
        std::atomic<std::size_t> m_Allocated;
    };
}