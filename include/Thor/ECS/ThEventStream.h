#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/Core/Containers/ThVector.h>
#include <functional>

namespace Thor
{

namespace Private
{
    class ThEventStreamBase
    {
    public:
        virtual ~ThEventStreamBase()
        {

        }
    };
}

template <class TEvent, ThI32 ReservedEvents = 64>
class ThEventStream: public Private::ThEventStreamBase
{
public:
    using TCallback = std::function<void(const TEvent*, ThSize)>;
    using TConnection = ThI32;

    ThEventStream()
    {
        Reserve();
    }

    void Emit(const TEvent& event)
    {
        m_EventStream[m_WriteBufferIndex].PushBack(event);

        if (m_EventStream[m_WriteBufferIndex].Size() >= m_EventStream[m_WriteBufferIndex].Capacity())
        {
            Reserve();
        }
    }

    void ProcessEvents()
    {
        ThI32 readBufferIndex = m_WriteBufferIndex;
        m_WriteBufferIndex = (m_WriteBufferIndex + 1) % 2;

        for (ThI32 i = m_Slots.Size() - 1; i >= 0; --i)
        {
            m_Slots[i].m_Callback(m_EventStream[readBufferIndex].Data(), m_EventStream[readBufferIndex].Size());
        }
    }

    TConnection RegisterCallback(const TCallback& callback)
    {
        TConnection result = m_ConnectionId;
        m_Slots.EmplaceBack(result, callback);
        ++m_ConnectionId;
        return result;
    }

    void UnregisterCallback(TConnection connectionId)
    {
        for (ThI32 i = 0; i < m_Slots.Size; ++i)
        {
            if (m_Slots[i].m_ConnectionId == connectionId)
            {
                m_Slots.MoveToBackAndRemove(i);
                break;
            }
        }
    }

private:

    void Reserve()
    {
        ThSize oldSize = m_EventStream[0].Size();
        m_EventStream[0].Reserve(oldSize + ReservedEvents);
        m_EventStream[1].Reserve(oldSize + ReservedEvents);
    }

    struct Slot
    {
        TConnection m_ConnectionId = 0;
        TCallback m_Callback = nullptr;
    };

    ThVector<Slot> m_Slots;
    ThVector<TEvent> m_EventStream[2];
    ThI32 m_WriteBufferIndex = 0;
    TConnection m_ConnectionId = 0;
};

}