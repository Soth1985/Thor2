#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{
    template <class Class>
    struct ThTypeID
    {
    public:
        explicit ThTypeID(ThI32 typeID)
            :
        m_TypeID(typeID)
        {}
        
        bool operator==(const ThTypeID& other)const
        {
            return m_TypeID == other.m_TypeID;
        }
        
        ThI32 TypeID()const
        {
            return m_TypeID;
        }
        
    private:
        ThI32 m_TypeID;
    };
    
    class ThiSystem;
    class ThiComponent;
    class ThEvent;
    typedef ThTypeID<ThiSystem> ThSystemType;
    typedef ThTypeID<ThiComponent> ThComponentType;
    typedef ThTypeID<ThEvent> ThEventType;
    
    class ThEvent
    {
    private:
        ThEventType m_Type;
        ThU32 m_Size;
    };
    
    class ThEntity
    {
    public:
        explicit ThEntity(ThU64 uid, ThI32 index)
            :
        m_Uid(uid),
        m_Index(index)
        {
            
        }
        
        ThU64 Uid()const
        {
            return m_Uid;
        }
        
        ThI32 Index()const
        {
            return m_Index;
        }
    private:
        ThU64 m_Uid;
        ThI32 m_Index;
    };
    
    class ThComponentRef
    {
        ThComponentType m_Type;
        ThI32 m_Index;
    };
    
    class ThForeignKey
    {
    public:
    private:
        ThComponentType m_Type;
        ThU64 m_Uid;
        ThI32 m_Index;
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
        virtual ThComponentType GetType() = 0;
    };
    
    class ThiEntityManager
    {
    public:
        virtual ThEntity CreateEntity() = 0;
        virtual bool DestroyEntity(const ThEntity& ent) = 0;
        virtual bool IsAlive(const ThEntity& ent) = 0;
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