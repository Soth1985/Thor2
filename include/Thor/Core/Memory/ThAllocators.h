#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Containers/Singleton.h>
#include <Thor/Core/Memory/ThSystemAllocator.h>

namespace Thor
{
    class ThAllocators
    {
    public:
        ThAllocators();
        ThiMemoryAllocator* GetSystemMemoryAllocator();
        static ThAllocators& Instance();
    private:
        typedef SingletonWithLongevity<ThAllocators, eSingletonsLongevity::Allocators> SingletonT;
        friend SingletonT;
        ThSystemAllocator m_SystemAllocator;
    };
}