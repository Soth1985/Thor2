#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{
    class ThiMemoryAllocator
    {
    public:
        ThiMemoryAllocator(const char* name = nullptr);
        virtual ~ThiMemoryAllocator();
        
        static const ThU32 DEFAULT_ALIGNMENT = 4;
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void Deallocate(void* ptr) = 0;
        const char* GetName()const;
        
        static const ThSize SIZE_UNTRACKED = -1;
        virtual ThSize GetCapacity() = 0;
        virtual ThSize GetTotalAllocated() = 0;
        
    protected:
        ThiMemoryAllocator(ThiMemoryAllocator&&) = delete;
        ThiMemoryAllocator(const ThiMemoryAllocator&) = delete;
        ThiMemoryAllocator& operator=(const ThiMemoryAllocator&) = delete;
        ThiMemoryAllocator& operator=(const ThiMemoryAllocator&&) = delete;
    private:
        const char* m_pName;
    };
    
    template<class T, class... Args>
    T* CreateObject(ThiMemoryAllocator* allocator, const Args&... args)
    {
        void* memory = allocator->Allocate(sizeof(T));
        T* result = new(memory)T(args...);
        return result;
    }
    
    
    template<class T>
    void DestroyObject(ThiMemoryAllocator* allocator, T* ptr)
    {
        ptr->~T();
        allocator->Deallocate(ptr);
    }
}