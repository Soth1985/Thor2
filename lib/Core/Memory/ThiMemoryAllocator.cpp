#include <Thor/Core/Memory/ThiMemoryAllocator.h>
#include <Thor/Core/Memory/ThSystemAllocator.h>

using namespace Thor;

ThiMemoryAllocator::ThiMemoryAllocator(const char* name)
    :
m_pName(name)
{
    
}

ThiMemoryAllocator::~ThiMemoryAllocator()
{
    
}

const char* ThiMemoryAllocator::GetName()const
{
    return m_pName;
}