#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Memory/ThMemoryUtil.h>

namespace Thor
{
    class ThiMemoryAllocator
    {
    public:
        
        static const ThSize SIZE_UNTRACKED = -1;
        static const ThU32 DEFAULT_ALIGNMENT = 4;
        
        ThiMemoryAllocator(const char* name = nullptr);
        virtual ~ThiMemoryAllocator();
        virtual void* Allocate(ThSize size, ThU32 alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void Deallocate(void* ptr) = 0;
        virtual ThSize GetCapacity() = 0;
        virtual ThSize GetTotalAllocated() = 0;
        const char* GetName()const;       
        
    protected:
        ThiMemoryAllocator(ThiMemoryAllocator&&) = delete;
        ThiMemoryAllocator(const ThiMemoryAllocator&) = delete;
        ThiMemoryAllocator& operator=(const ThiMemoryAllocator&) = delete;
        ThiMemoryAllocator& operator=(const ThiMemoryAllocator&&) = delete;
    private:
        const char* m_pName;
    };
    
    template<class T, class... Args>
    T* AllocateObject(ThiMemoryAllocator* allocator, const Args&... args)
    {
        T* memory = (T*)allocator->Allocate(sizeof(T));
        auto Func = Private::CreateObjectsImpl<std::is_pod<T>::value, T, Args...>();
        Func(memory, 1, args...);
        return memory;
    }
    
    template<class T, class... Args>
    T* AllocateObjects(ThiMemoryAllocator* allocator, ThSize numObjects, const Args&... args)
    {
        T* memory = (T*)allocator->Allocate(sizeof(T) * numObjects);
        auto Func = Private::CreateObjectsImpl<std::is_pod<T>::value, T, Args...>();
        Func(memory, numObjects, args...);
        return memory;
    }
    
    template<class T>
    void FreeObject(ThiMemoryAllocator* allocator, T* ptr)
    {
        auto Func = Private::DestroyObjectsImpl<std::is_pod<T>::value, T>();
        Func(ptr, 1);
        allocator->Deallocate(ptr);
    }
    
    template<class T>
    void FreeObjects(ThiMemoryAllocator* allocator, T* ptr, ThSize numObjects)
    {
        auto Func = Private::DestroyObjectsImpl<std::is_pod<T>::value, T>();
        Func(ptr, numObjects);
        allocator->Deallocate(ptr);
    }
}