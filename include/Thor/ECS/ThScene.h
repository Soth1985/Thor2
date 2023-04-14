#pragma once

#include <Thor/ECS/ThSparseStructuredStorage.h>
#include <Thor/ECS/ThEntityManager.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/ThFlags.h>

namespace Thor
{

class ThWorld;

template <class ...TComponents>
class ThSceneQuery;

class ThScene
{
public:

    ThScene(ThWorld* world, ThU64 uid)
    {
        m_World = world;
        m_Uid = uid;
        m_EntityManager = new ThEntityManager();
    }

    ~ThScene()
    {
        for (auto i = m_Components.Begin(); i != m_Components.End(); ++i)
        {
            delete i->Value();
        }
    }

    template <class TComponent>
    bool RegisterComponent()
    {
        if (m_Components.Find(TComponent::ComponentId) == m_Components.End())
        {
            auto newComponentStorage = new ThSparseStructuredStorage(TComponent::ComponentId, sizeof(TComponent));
            m_Components[TComponent::ComponentId] = newComponentStorage;
            return true;
        }
        else
        {
            THOR_ERR("Component %d is already registered", Private::LoggerTag, TComponent::ComponentId);
            return false;
        }
    }

    template <class... TComponents>
    bool RegisterComponents()
    {
        bool result = true;
        ((result = result && RegisterComponent<TComponents>()), ...);
        return result;
    }

    ThSparseStructuredStorage* GetComponentStorage(ThComponentId componentId)
    {
        auto found = m_Components.Find(componentId);

        if (found != m_Components.End())
        {
            return found->Value();
        }

        return nullptr;
    }

    template <class TComponent>
    bool RemoveComponent(ThEntityId entityId)
    {
        auto storage = GetComponentStorage(TComponent::ComponentId);

        if (storage)
        {
            return storage->RemoveComponent(entityId);
        }

        return false;
    }

    template <class TComponent>
    bool SetComponent(ThEntityId entityId, const TComponent& component)
    {
        auto storage = GetComponentStorage(TComponent::ComponentId);

        if (storage)
        {
            const ThI8* componentData = reinterpret_cast<const ThI8*>(&component);
            storage->SetComponent(entityId, componentData);
            return true;
        }

        THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent::ComponentId);
        return false;
    }

    template <class TComponent>
    bool GetComponent(ThEntityId entityId, TComponent& component)
    {
        auto storage = GetComponentStorage(TComponent::ComponentId);

        if (storage)
        {
            ThI8* componentData = reinterpret_cast<ThI8*>(&component);
            return storage->GetComponent(entityId, componentData);
        }

        THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent::ComponentId);
        return false;
    }

    ThWorld* GetWorld()
    {
        return m_World;
    }

    ThEntityManager* GetEntityManager()
    {
        return m_EntityManager;
    }
    
    ThU64 GetUid()const
    {
        return m_Uid;
    }
    
    template <class ...TComponents>
    ThSceneQuery<TComponents...> CreateQuery()
    {
        return ThSceneQuery<TComponents...>(this);
    }

    /*template <class TComponent>
    ThSceneQuery<TComponent> CreateQuery()
    {
        return ThSceneQuery<TComponent>(this);
    }

    template <class TComponent1, class TComponent2>
    ThSceneQuery<TComponent1, TComponent2> CreateQuery()
    {
        return ThSceneQuery<TComponent1, TComponent2>(this);
    }*/

private:

    ThHashMap<ThComponentId, ThSparseStructuredStorage*> m_Components;
    ThWorld* m_World {nullptr};
    ThEntityManager* m_EntityManager {nullptr};
    ThU64 m_Uid {0};

};

class ThSceneQueryBase
{
public:
    ThSceneQueryBase(ThScene* scene)
        :
    m_Scene(scene)
    {

    }

    template <class TComponent>
    void ExcludeComponent()
    {
        for (ThSize i = 0; i < m_ExcludedComponents.Size(); ++i)
        {
            if (m_ExcludedComponents[i] == TComponent::ComponentId)
            {
                THOR_ERR("Component %d is already excluded", Private::LoggerTag, TComponent::ComponentId);
                return;
            }
        }
        
        m_ExcludedComponents.PushBack(TComponent::ComponentId);
    }

    template <class... TComponents>
    void ExcludeComponents()
    {
        (ExcludeComponent<TComponents>(), ...);
    }

protected:
    
    bool IsEntityExcluded(ThEntityId entityId)
    {
        for (ThSize excluded = 0; excluded < m_ExcludedComponents.Size(); ++excluded)
        {
            ThSparseStructuredStorage* excludedStorage = m_Scene->GetComponentStorage(m_ExcludedComponents[excluded]);

            if (excludedStorage->HasEntity(entityId))
            {
                return true;
            }
        }

        return false;
    }

protected:

    template <int NumComponents>
    void FindPrimaryStorage(ThSparseStructuredStorage* storageArray[NumComponents], ThI32& primaryStorageIndex)
    {
        primaryStorageIndex = 0;
        ThSize minEntities = -1;

        for (ThSize i = 0; i < NumComponents; ++i)
        {
            ThSize componentsCount = storageArray[i]->ComponentsCount();
            if (componentsCount < minEntities)
            {
                primaryStorageIndex = i;
                minEntities = componentsCount;
            }
        }
    }

    template <int NumComponents, class TFunc>
    void IterateStorage(ThSparseStructuredStorage* storageArray[NumComponents], ThI32 primaryStorageIndex, TFunc&& func)
    {
        ThSparseStructuredStorage* primaryStorage = storageArray[primaryStorageIndex];
        ThI8* dataBuffersArray[NumComponents] = {nullptr};

        for (ThSize p = 0; p < primaryStorage->PageCount(); ++p)
        {
            auto page = primaryStorage->Page(p);

            for (ThI64 e = page->Size() - 1; e >= 0; --e)
            {
                ThEntityId entityId = page->EntityId(e);
                bool excluded = IsEntityExcluded(entityId);

                if (excluded)
                {
                    continue;
                }

                dataBuffersArray[primaryStorageIndex] = page->ComponentData(e);

                bool hasAllRequiredComponets = true;

                for (ThSize componentIdx = 0; componentIdx < NumComponents; ++componentIdx)
                {
                    if (componentIdx == primaryStorageIndex)
                    {
                        continue;
                    }

                    bool dataFound = storageArray[componentIdx]->GetComponentNoCopy(entityId, &dataBuffersArray[componentIdx]);

                    if (!dataFound)
                    {
                        hasAllRequiredComponets = false;
                        break;
                    }
                }

                func(entityId, dataBuffersArray);
            }
        }
    }

protected:
    ThVector<ThComponentId> m_ExcludedComponents;
    ThScene* m_Scene = nullptr;
};

/*template <class TComponent>
class ThSceneQuery: public ThSceneQueryBase
{
public:

    ThSceneQuery(ThScene* scene)
        :
    ThSceneQueryBase(scene)
    {

    }

    template <class TFunc>
    void Execute(TFunc&& func)
    {
        auto storage = m_Scene->GetComponentStorage(TComponent::ComponentId);

        if (!storage)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent::ComponentId);
            return;
        }

        ThSparseStructuredStorage* storageArray[1] = {storage};

        ThI32 primaryStorageIndex = 0;

        IterateStorage<1>(storageArray, primaryStorageIndex, [this, &func](ThEntityId entityId, ThI8* dataBuffersArray[1])
        {
            TComponent* data = static_cast<TComponent*>(dataBuffersArray[0]);
            func(m_Scene, entityId, data);
        });
    }
};

template <class TComponent1, class TComponent2>
class ThSceneQuery: public ThSceneQueryBase
{
public:

    ThSceneQuery(ThScene* scene)
        :
    ThSceneQueryBase(scene)
    {

    }

    template <class TFunc>
    void Execute(TFunc&& func)
    {
        auto storage1 = m_Scene->GetComponentStorage(TComponent1::ComponentId);

        if (!storage1)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent1::ComponentId);
            return;
        }

        auto storage2 = m_Scene->GetComponentStorage(TComponent2::ComponentId);

        if (!storage2)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent2::ComponentId);
            return;
        }

        ThSparseStructuredStorage* storageArray[2] = {storage1, storage2};

        ThI32 primaryStorageIndex = 0;
        FindPrimaryStorage<2>(storageArray, primaryStorageIndex);

        IterateStorage<2>(storageArray, primaryStorageIndex, [](ThEntityId entityId, ThI8* dataBuffersArray[2])
        {
            TComponent1* data1 = static_cast<TComponent1*>(dataBuffersArray[0]);
            TComponent2* data2 = static_cast<TComponent2*>(dataBuffersArray[1]);
            func(m_Scene, entityId, data1, data2);
        });
    }
};*/

template <class ...TComponents>
class ThSceneQuery: public ThSceneQueryBase
{
public:

    ThSceneQuery(ThScene* scene)
        :
    ThSceneQueryBase(scene)
    {

    }
    
    template <class TComponent>
    ThSparseStructuredStorage* GetComponentStorage()
    {
        auto storage = m_Scene->GetComponentStorage(TComponent::ComponentId);

        if (!storage)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent::ComponentId);
        }
        
        return storage;
    }

    template <class TFunc>
    void Execute(TFunc&& func)
    {
        ThSparseStructuredStorage* storageArray[sizeof...(TComponents)] = {(GetComponentStorage<TComponents>(), ...)};
        
        for (ThSize i = 0; i < sizeof...(TComponents); ++i)
        {
            if (storageArray[i] == nullptr)
            {
                return;
            }
        }
        
        ThI32 primaryStorageIndex = 0;
        FindPrimaryStorage<sizeof...(TComponents)>(storageArray, primaryStorageIndex);
        
        IterateStorage<sizeof...(TComponents)>(storageArray, primaryStorageIndex, [this, &func](ThEntityId entityId, ThI8* dataBuffersArray[sizeof...(TComponents)])
        {
            int bufferIndex = 0;
            func(m_Scene, entityId, (static_cast<TComponents*>(dataBuffersArray[bufferIndex++]), ...));
        });
    }
};

}
