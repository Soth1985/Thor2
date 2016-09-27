#pragma once

#include <Thor/Engine/EngineInterface.h>
#include <Thor/Core/Memory/ThiMemoryAllocator.h>

namespace Thor
{
    namespace Private
    {
        struct ThEventBuffer
        {
            ThEventBuffer();
            ThI8* m_Data;
            ThI32 m_Size;
            ThI32 m_Capacity;
        };
    }
    
    class ThEventManager : public ThiEventManager
    {
    public:
        ThEventManager();
        virtual void PushEvent(const ThEvent& event)override;
        virtual ThEvent GetFirstEvent()override;
        virtual ThEvent GetNextEvent(const ThEvent& prevEvent)override;
        void SwapBuffers();
    private:
        void AllocateBuffers(ThU32 bufferSize);
        Private::ThEventBuffer m_Buffers[2];
        ThI32 m_WriteBuffer;
        ThiMemoryAllocator* m_Allocator;
    };
}