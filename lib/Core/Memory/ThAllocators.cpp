#include <Thor/Core/Memory/ThAllocators.h>

using namespace Thor;

ThAllocators::ThAllocators()
    :
m_SystemAllocator("Default System Allocator")
{
    
}

ThiMemoryAllocator* ThAllocators::GetSystemMemoryAllocator()
{
    return &m_SystemAllocator;
}

ThAllocators& ThAllocators::Instance()
{
    return SingletonT::Instance();
}