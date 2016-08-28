#pragma once

#include <Thor/Core/Memory/ThMemory.h>
#include <type_traits>

namespace Thor
{
    namespace Private
    {
        template<bool IsPod, class T, class... Args>
        struct CreateObjectsImpl
        {
            void operator()(T* memory, ThSize numObjects, const Args&... args);
        };
        
        template<class T, class... Args>
        struct CreateObjectsImpl<true, T, Args...>
        {
            void operator()(T* memory, ThSize numObjects, const Args&... args)
            {
                
            }
        };
        
        template<class T, class... Args>
        struct CreateObjectsImpl<false, T, Args...>
        {
            void operator()(T* memory, ThSize numObjects, const Args&... args)
            {
                for (ThSize i = 0; i < numObjects; ++i)
                    new(&memory[i])T(std::move(args)...);
            }
        };
        
        template<bool IsPod, class T>
        struct DestroyObjectsImpl
        {
            void operator()(T* ptr, ThSize numObjects);
        };
        
        template<class T>
        struct DestroyObjectsImpl<true, T>
        {
            void operator()(T* ptr, ThSize numObjects)
            {
            }
        };
        
        template<class T>
        struct DestroyObjectsImpl<false, T>
        {
            void operator()(T* ptr, ThSize numObjects)
            {
                for (ThSize i = 0; i < numObjects; ++i)
                    ptr[i].~T();
            }
        };
        
        template<bool IsTriviallyCopyable, class T>
        struct MoveObjectsImpl
        {
            void operator()(const T* from, T* to, ThSize numObjects);
        };
        
        template<class T>
        struct MoveObjectsImpl<true, T>
        {
            void operator()(const T* from, T* to, ThSize numObjects)
            {
                ThMemory::MemoryMove(to, from, numObjects * sizeof(T));
            }
        };
        
        template<class T>
        struct MoveObjectsImpl<false, T>
        {
            void operator()(const T* from, T* to, ThSize numObjects)
            {
                for (ThSize i = 0; i < numObjects; ++i)
                    new (&to[i])T(std::move(from[i]));
            }
        };
        
        template<bool IsTriviallyCopyable, class T>
        struct CopyObjectsImpl
        {
            void operator()(const T* from, T* to, ThSize numObjects);
        };
        
        template<class T>
        struct CopyObjectsImpl<true, T>
        {
            void operator()(const T* from, T* to, ThSize numObjects)
            {
                ThMemory::MemoryMove(to, from, numObjects * sizeof(T));
            }
        };
        
        template<class T>
        struct CopyObjectsImpl<false, T>
        {
            void operator()(const T* from, T* to, ThSize numObjects)
            {
                for (ThSize i = 0; i < numObjects; ++i)
                    new (&to[i])T(from[i]);
            }
        };
    }
    
    template<class T, class... Args>
    void ConstructObject(T* ptr, const Args&... args)
    {
        auto Func = Private::CreateObjectsImpl<std::is_pod<T>::value, T, Args...>();
        Func(ptr, 1, args...);
    }
    
    template<class T, class... Args>
    void ConstructObjects(T* ptr, ThSize numObjects, const Args&... args)
    {
        auto Func = Private::CreateObjectsImpl<std::is_pod<T>::value, T, Args...>();
        Func(ptr, numObjects, args...);
    }
    
    template<class T>
    void DestroyObject(T* ptr)
    {
        auto Func = Private::DestroyObjectsImpl<std::is_pod<T>::value, T>();
        Func(ptr, 1);
    }
    
    template<class T>
    void DestroyObjects(T* ptr, ThSize numObjects)
    {
        auto Func = Private::DestroyObjectsImpl<std::is_pod<T>::value, T>();
        Func(ptr, numObjects);
    }
    
    template <class T>
    void MoveObjects(const T* from, T* to, ThSize numObjects)
    {
        auto Func = Private::MoveObjectsImpl<std::is_trivially_copyable<T>::value, T>();
        Func(from, to, numObjects);
    }
    
    template <class T>
    void CopyObjects(T* from, T* to, ThSize numObjects)
    {
        auto Func = Private::CopyObjectsImpl<std::is_trivially_copyable<T>::value, T>();
        Func(from, to, numObjects);
    }
}
