#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/Memory/ThVirtualStackAllocator.h>
#include <Thor/Core/Memory/ThStackAllocator.h>

using namespace Thor;
static const ThChar* logTag = "TestAllocators";

void TouchMemory(ThU8* ptr, ThSize allocSize)
{
    for (ThSize j = 0; j < allocSize; ++j)
        ptr[j] = j;
}

void TestVirtualStackAllocator()
{
    const ThSize allocSize = 5;
    const ThSize bufferSize = ThMemory::Megabyte() * 160;
    ThVirtualStackAllocator allocator("Virtual Stack");
    allocator.Init(bufferSize);
    
    ThSize numAllocs = 0;
    
    for (ThSize i = 0; i < bufferSize; i+=allocSize, ++numAllocs)
    {
        ThU8* ptr = (ThU8*)allocator.Allocate(allocSize);
        
        if (!ptr)
            break;
        //without access, memory will not be physically allocated
        TouchMemory(ptr, allocSize);
    }
    
    ThSize capacity = allocator.GetCapacity();
    ThSize totalAllocated = allocator.GetTotalAllocated();
    THOR_INF("Allocator %s bufferSize = %d allocSize = %d capacity = %d totalAllocated = %d numAllocs = %d", logTag, allocator.GetName(), bufferSize, allocSize, capacity, totalAllocated, numAllocs);
}

void TestStackAllocator()
{
    const ThSize allocSize = 5;
    const ThSize bufferSize = ThMemory::Kilobyte() * 16;
    ThStackAllocator allocator("Stack");
    allocator.Init(bufferSize);
    
    ThSize numAllocs = 0;
    
    for (ThSize i = 0; i < bufferSize; i+=allocSize, ++numAllocs)
    {
        ThU8* ptr = (ThU8*)allocator.Allocate(allocSize);
        
        if (!ptr)
            break;
        
        TouchMemory(ptr, allocSize);
    }
    
    ThSize capacity = allocator.GetCapacity();
    ThSize totalAllocated = allocator.GetTotalAllocated();
    THOR_INF("Allocator %s bufferSize = %d allocSize = %d capacity = %d totalAllocated = %d numAllocs = %d", logTag, allocator.GetName(), bufferSize, allocSize, capacity, totalAllocated, numAllocs);
}

int main()
{
    THOR_INF("Program has started", logTag );
    TestVirtualStackAllocator();
    TestStackAllocator();
	return 0;
}
