#include <Thor/Engine/ThEventManager.h>
#include <Thor/Core/Memory/ThSystemAllocator.h>

using namespace Thor;
using namespace Thor::Private;

ThEventBuffer::ThEventBuffer()
    :
m_Data(nullptr),
m_Size(0),
m_Capacity(0)
{
    
}

ThEventManager::ThEventManager()
    :
m_WriteBuffer(0)
{
    const ThI32 bufferSize = 1024;
    m_Allocator = new ThSystemAllocator("ThEventManager");
    AllocateBuffers(bufferSize);
}

void ThEventManager::PushEvent(const ThEvent& event)
{
    ThU32 eventSize = sizeof(ThU32) + sizeof(ThEventType) + event.Size();
    ThEventBuffer& writeBuffer = m_Buffers[m_WriteBuffer];
    
    if (writeBuffer.m_Size + eventSize > writeBuffer.m_Capacity)
        AllocateBuffers(writeBuffer.m_Capacity * 2);
    
    ThI8* destination = writeBuffer.m_Data + writeBuffer.m_Size;
    ThMemory::MemoryCopy(destination , &event.Type(), sizeof(ThEventType));
    destination += sizeof(ThEventType);
    ThMemory::MemoryCopy(destination , &event.Size(), sizeof(ThU32));
    destination += sizeof(ThU32);
    ThMemory::MemoryCopy(destination , event.Data<ThI8>(), event.Size());
    writeBuffer.m_Size += eventSize;
}

ThEvent ThEventManager::GetFirstEvent()
{
    ThEventBuffer& readBuffer = m_Buffers[!m_WriteBuffer];
    
    if (readBuffer.m_Size == 0)
        return ThEvent();
    
    ThI8* eventStream = readBuffer.m_Data;
    ThEventType type = *(ThEventType*)eventStream;
    eventStream += sizeof(ThEventType);
    ThU32 size = *(ThU32*)eventStream;
    eventStream += sizeof(ThU32);
    return ThEvent(type, size, eventStream);
}

ThEvent ThEventManager::GetNextEvent(const ThEvent& prevEvent)
{
    if (!prevEvent)
        return ThEvent();
    
    ThEventBuffer& readBuffer = m_Buffers[!m_WriteBuffer];
    ThI8* eventStream = const_cast<ThI8*>(prevEvent.Data<ThI8>());
    
    if (eventStream >= readBuffer.m_Data && eventStream < (readBuffer.m_Data + readBuffer.m_Size))
    {
        eventStream += prevEvent.Size();
        ThEventType type = *(ThEventType*)eventStream;
        eventStream += sizeof(ThEventType);
        ThU32 size = *(ThU32*)eventStream;
        eventStream += sizeof(ThU32);
        return ThEvent(type, size, eventStream);
    }
    else
        return ThEvent();
}

void ThEventManager::SwapBuffers()
{
    m_WriteBuffer = !m_WriteBuffer;
    m_Buffers[m_WriteBuffer].m_Size = 0;
}

void ThEventManager::AllocateBuffers(ThU32 bufferSize)
{
    ThI8* oldData0 = m_Buffers[0].m_Data;
    ThU32 oldSize0 = m_Buffers[0].m_Size;
    ThI8* oldData1 = m_Buffers[1].m_Data;
    ThU32 oldSize1 = m_Buffers[1].m_Size;
    
    m_Buffers[0].m_Data = (ThI8*)m_Allocator->Allocate(bufferSize * 2);
    m_Buffers[0].m_Capacity = bufferSize;
    m_Buffers[1].m_Data = m_Buffers[0].m_Data + bufferSize;
    m_Buffers[1].m_Capacity = bufferSize;
    
    if (oldData0)
    {
        ThMemory::MemoryCopy(m_Buffers[0].m_Data, oldData0, oldSize0);
        ThMemory::MemoryCopy(m_Buffers[1].m_Data, oldData1, oldSize1);
        m_Allocator->Deallocate(oldData0);
    }
}