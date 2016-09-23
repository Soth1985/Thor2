#pragma once

#include <Thor/Engine/EngineForward.h>

namespace Thor
{
    class ThEvent
    {
    public:
        ThEvent()
            :
        m_Type(0, 0),
        m_Size(0),
        m_Data(nullptr)
        {
            
        }
        
        ThEvent(ThEventType type, ThU32 size, ThI8* data)
            :
        m_Type(type),
        m_Size(size),
        m_Data(data)
        {
            
        }
        
        template <class T>
        ThEvent(ThEventType type, const T* data)
            :
        m_Type(type),
        m_Size(sizeof(T)),
        m_Data(data)
        {
            
        }
        
        template <class T>
        const T* Data()const
        {
            return (T*)m_Data;
        }
        
        const ThU32& Size()const
        {
            return m_Size;
        }
        
        const ThEventType& Type()const
        {
            return m_Type;
        }
        
        operator bool()const
        {
            return m_Data != nullptr;
        }
    private:
        ThEventType m_Type;
        ThU32 m_Size;
        const ThI8* m_Data;
    };
}