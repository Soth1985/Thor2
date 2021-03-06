#pragma once

#include <Thor/Engine/ThEngineForward.h>
#include <Thor/Engine/ThEntity.h>
#include <Thor/Engine/ThEvent.h>
#include <Thor/Containers/ThDelegate.h>

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
        virtual ThI32 GetIndex(const ThEntity& entity) = 0;
    };
    
    class ThiEntityManager
    {
    public:
        
        virtual ThEntity CreateEntity() = 0;
        virtual bool DestroyEntity(const ThEntity& ent) = 0;
        virtual bool IsAlive(const ThEntity& ent) = 0;
        virtual bool IsActive(const ThEntity& ent) = 0;
        virtual void Reserve(ThI32 capacity) = 0;
        virtual ThI32 GetNumEntities() = 0;
        virtual ThEntity GetEntity(ThI32 index) = 0;
        
        typedef ThDelegate<const ThEntity& entity, bool success> OnEntityCreatedDel;
        typedef ThDelegate<const ThEntity& entity, bool success> OnEntityDestroyedDel;
        
        OnEntityCreatedDel md_OnEntityCreated;
        OnEntityDestroyedDel md_OnEntityDestroyed;
    };
    
    class ThiResourceManager
    {
    public:
        virtual ThResourceType GetType() = 0;
        virtual const char* GetResourceName(ThU64 resUid) = 0;
        virtual const char* SetResourceName(ThU64 resUid, const char* name) = 0;
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
        virtual ThChar* GetName() = 0;
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
        
        virtual ThiEventManager* GetEventManager() = 0;
        virtual ThiBlackboard* GetBlackboard() = 0;
        virtual ThU64 GenerateUid() = 0;
        virtual ThU64 GenerateUid(const ThChar* name) = 0;
        virtual void GetVersion(ThI32& major, ThI32& minor) = 0;
        
        virtual ThStringHash64 InternString(const ThString& str) = 0;
        virtual const ThString& GetInternedString(const ThStringHash64& hash) = 0;
        
        virtual void SetLocalizedString(const ThString& tag, const ThString& value) = 0;
        virtual const ThString& GetLocalizedString(const ThString& tag) = 0;
        virtual void RequestLanguageChange(const ThString& lang) = 0;
        //static ThiEngine* CreateEngine(ThI32 versionMajor, ThI32 versionMinor);
    };
}
