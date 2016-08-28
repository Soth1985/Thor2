#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{
    template <class Class>
    class ThTypeID
    {
    public:
        explicit ThTypeID(ThI32 typeID)
            :
        m_TypeID(typeID)
        {}
        
        bool operator==(const ThTypeID& other)
        {
            return m_TypeID == other.m_TypeID;
        }
        
        ThTypeID& operator=(const ThTypeID& rhs) = delete;
        
    private:
        ThI32 m_TypeID;
    };
    
    class ThiSystem;
    class ThiComponent;
    class ThEvent;
    typedef ThTypeID<ThiSystem> ThSystemType;
    typedef ThTypeID<ThiComponentDataManager> ThComponentManagerType;
    typedef ThTypeID<ThEvent> ThEventType;
    
    class ThEvent
    {
    private:
        ThEventType m_Type;
        ThU32 m_Size;
    };
    
    class ThiEventManager
    {
    public:
        virtual void PushMessage() = 0;
    };
    
    class ThiSystem
    {
    public:
        virtual ThSystemType GetType() = 0;
        virtual void Update() = 0;
    };
    
    class ThiComponentManager
    {
    public:
        virtual ThComponentManagerType GetType() = 0;
    };
    
    class ThiEngine
    {
    public:
        virtual ThiSystem* GetSystem(ThSystemType type) = 0;
        virtual ThiSystem* GetSystem(ThU32 index) = 0;
        virtual ThU32 GetNumSystems() = 0;
        virtual bool AddSystem(ThiSystem* system) = 0;
        virtual bool RemoveSystem(ThiSystem* system) = 0;
    };
}