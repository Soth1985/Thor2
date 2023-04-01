#pragma once

#include <Thor/ECS/ThEventStream.h>
#include <Thor/Core/Containers/ThHashMap.h>

namespace Thor
{

class ThEventManager
{
public:

    ~ThEventManager()
    {
        for (auto i = m_EventStreams.Begin(); i != m_EventStreams.End(); ++i)
        {
            delete i->Value();
        }
        
    }

    template <class TEvent>
    void RegisterEvent()
    {
        if (m_EventStreams.Find(TEvent::EventId) == m_EventStreams.End())
        {
            auto newEventStream = new ThEventStream<TEvent>();
            m_EventStreams[TEvent::EventId] = newEventStream;
        }
    }

    template <class... TEvents>
    void RegisterEvents()
    {
        (RegisterEvent<TEvents>(), ...);
    }

    template <class TEvent>
    ThEventStream<TEvent>* GetEventStream()
    {
        auto found = m_EventStreams.Find(TEvent::EventId);

        if (found != m_EventStreams.End())
        {
            return static_cast<ThEventStream<TEvent>*>(found->Value());
        }

        return nullptr;
    }

    template <class TEvent>
    void EmitEvent(const TEvent& event)
    {
        auto eventStream = GetEventStream<TEvent>();

        if (eventStream)
        {
            eventStream->Emit(event);
        }
        else
        {
            THOR_ERR("Failed to emit event %d bacause it is not registred", Private::LoggerTag, TEvent::EventId);
        }
    }

    void ProcessEvents()
    {
        for (auto i = m_EventStreams.Begin(); i != m_EventStreams.End(); ++i)
        {
            i->Value()->ProcessEvents();
        }
    }

private:

    ThHashMap<ThEventId, ThEventStreamBase*> m_EventStreams;
};

}