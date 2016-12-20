#pragma once

#include <Thor/Engine/EngineForward.h>
#include <Thor/Engine/ThEntity.h>
#include <Thor/Engine/ThEvent.h>
#include <Thor/Engine/ThClusterHash.h>

namespace Thor
{   
    class ThiEventManager
    {
    public:
        virtual void PushEvent(const ThEvent& event) = 0;
        virtual ThEvent GetFirstEvent() = 0;
        virtual ThEvent GetNextEvent(const ThEvent& prevEvent) = 0;
    };
    
    class ThiSystem
    {
    public:
        virtual ThSystemType GetType() = 0;
        virtual void Process(ThiWorld* world) = 0;
    };
    
    class ThiComponentManager
    {
    public:
        virtual ThComponentType GetType() = 0;
    };
    
    class ThiEntityManager
    {
    public:
        //virtual ThEntity CreateEntity(const ThClusterHash& cluster) = 0;
        //virtual bool IsClusterActive(const ThClusterHash& cluster) = 0;
        //virtual void SetClusterActive(const ThClusterHash& cluster, bool active) = 0;
        
        virtual ThEntity CreateEntity() = 0;
        virtual bool DestroyEntity(const ThEntity& ent) = 0;
        virtual bool IsAlive(const ThEntity& ent) = 0;
        virtual bool IsActive(const ThEntity& ent) = 0;
                virtual void Reserve(ThI32 capacity) = 0;
        virtual ThI32 GetNumEntities() = 0;
        virtual ThEntity GetEntity(ThI32 index) = 0;
    };
    
    class ThiResourceManager
    {
    public:
        virtual ThResourceType GetType() = 0;
    };
    
    class ThiBlackboardItem
    {
    public:
        virtual ThBlackboardType GetType() = 0;
    };
    
    class ThiBlackboard
    {
    public:
        virtual ThiBlackboardItem* GetItem(ThBlackboardType type) = 0;
        virtual ThiBlackboardItem* GetItem(ThI32 index) = 0;
        virtual ThI32 GetNumItems() = 0;
        virtual bool AddItem(ThiBlackboardItem* item) = 0;
        virtual bool RemoveItem(ThiBlackboardItem* item) = 0;
    };
    
    class ThiWorld
    {
    public:
        virtual ThiComponentManager* GetComponentManager(ThComponentType type) = 0;
        virtual ThiComponentManager* GetComponentManager(ThI32 index) = 0;
        virtual ThI32 GetNumComponentManagers() = 0;
        virtual bool AddComponentManager(ThiComponentManager* componentManager) = 0;
        virtual bool RemoveComponentManager(ThiComponentManager* componentManager) = 0;
        
        virtual ThiEntityManager* GetEntityManager() = 0;
    };
    
    class ThiEngine
    {
    public:
        virtual ThI32 GetNumWorlds() = 0;
        virtual ThiWorld* GetWorld(ThI32 index) = 0;
        virtual bool AddWorld(ThiWorld* world) = 0;
        virtual bool RemoveWorld(ThiWorld* world) = 0;
        
        virtual ThiSystem* GetSystem(ThSystemType type) = 0;
        virtual ThiSystem* GetSystem(ThI32 index) = 0;
        virtual ThI32 GetNumSystems() = 0;
        virtual bool AddSystem(ThiSystem* system) = 0;
        virtual bool RemoveSystem(ThiSystem* system) = 0;
        
        virtual ThiResourceManager* GetResourceManager(ThResourceType type) = 0;
        virtual ThiResourceManager* GetResourceManager(ThI32 index) = 0;
        virtual ThI32 GetNumResourceManagers() = 0;
        virtual bool AddResourceManager(ThiResourceManager* componentManager) = 0;
        virtual bool RemoveResourceManager(ThiResourceManager* componentManager) = 0;
        
        virtual const char* GetResourceName(ThU64 resUid) = 0;
        virtual bool RegisterResourceName(ThU64 resUid, const char* name) = 0;
        virtual bool UnregisterResourceName(ThU64 resUid) = 0;
        
        virtual ThiEventManager* GetEventManamer() = 0;
        virtual ThiBlackboard* GetBlackboard() = 0;
        virtual ThU64 GenerateUid() = 0;
        virtual void SetUidGeneratorConstant(ThU8 constant) = 0;
        virtual void GetVersion(ThI32& major, ThI32& minor) = 0;
        
        //static ThiEngine* CreateEngine(ThI32 versionMajor, ThI32 versionMinor);
    };
}
