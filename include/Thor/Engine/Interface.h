#pragma once

#include <Thor/Engine/EngineForward.h>

namespace Thor
{
    class ThEvent
    {
    private:
        ThEventType m_Type;
        ThU32 m_Size;
        ThI8* m_Data;
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
        
        operator bool()const
        {
            return m_Uid != 0;
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
        ThI32 m_Index;
        ThU64 m_Uid;
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
    
    class ThiComponent
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
        virtual void Reserve(ThI32 capacity) = 0;
        virtual ThI32 GetNumEntities() = 0;
        virtual ThEntity GetEntity(ThI32 index) = 0;
    };
    
    class ThiResource
    {
    public:
        virtual ThResourceType GetType() = 0;
    };
    
    class ThiEngine
    {
    public:
        virtual ThiSystem* GetSystem(ThSystemType type) = 0;
        virtual ThiSystem* GetSystem(ThI32 index) = 0;
        virtual ThI32 GetNumSystems() = 0;
        virtual bool AddSystem(ThiSystem* system) = 0;
        virtual bool RemoveSystem(ThiSystem* system) = 0;
        virtual ThiComponent* GetComponent(ThComponentType type) = 0;
        virtual ThiComponent* GetComponent(ThI32 index) = 0;
        virtual ThI32 GetNumComponents() = 0;
        virtual bool AddComponent(ThiComponent* component) = 0;
        virtual bool RemoveComponent(ThiComponent* component) = 0;
        virtual ThiEventManager* GetEntityManager() = 0;
        virtual ThiEventManager* GetEventManamer() = 0;
        virtual ThU64 GenerateUid() = 0;
        virtual void SetUidGeneratorConstant(ThU8 constant) = 0;
        
        static ThiEngine* CreateEngine(ThI32 versionMajor, ThI32 versionMinor);
    };
}