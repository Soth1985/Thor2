#pragma once

#include <Thor/ECS/ThSparseStructuredStorage.h>
#include <Thor/ECS/ThEntityManager.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/ThFlags.h>

namespace Thor
{

class ThWorld;
class ThScene;


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
    enum IterationFlags
    {
        Finished,
        Skip,
        Apply
    };

    bool IsEntityExcluded(ThEntityId entityId)
    {
        for (ThSize excluded = 0; excluded < m_ExcludedComponents.Size(); ++excluded)
        {
            ThComponentStorageBase* excludedStorage = m_Scene->GetComponentStorage();

            if (excluded->HasEntity(page.EnitityId(e)))
            {
                return true;
            }
        }

        return false;
    }

    template <class TComponent>
    ThFlags32 NextComponentData(ThSparseStructuredStorage<TComponent>* storage, bool isPrimaryStorage, ThEntityId& entityId, TComponent& component, ThSize& pageIndex, ThU16& entityIndex)
    {
        ThFlags32 result;

        if (isPrimaryStorage)
        {
            if (storage->PageCount() == 0)
            {
                result.SetFlag(IterationFlags::Finished);
                return;
            }

            auto page = storage->Page(pageIndex);

            if (entityIndex >= page->Size())
            {
                ++pageIndex;
                entityIndex = 0;
            }
            
            if (pageIndex >= storage->PageCount())
            {
                result.SetFlag(IterationFlags::Finished);
                return;
            }

            page = storage->Page(pageIndex);
            entityId = page->EntityId(entityIndex);
            component = page->Data(entityIndex);
            ++entityIndex;
            result.SetFlag(IterationFlags::Apply);
        }
        else
        {
            bool found = storage->GetComponent(entityId, component);

            if (found)
            {
                result.SetFlag(IterationFlags::Apply);
            }
            else
            {
                result.SetFlag(IterationFlags::Skip);
            }
        }

        return result;
    }
protected:
    ThVector<ThComponentId> m_ExcludedComponents;
    ThScene* m_Scene = nullptr;
};

template <class TComponent>
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
        auto storage = m_Scene->GetComponentStorage<TComponent>();

        if (!storage)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent::ComponentId);
            return;
        }

        for (ThSize p = 0; p < storage->PageCount(); ++p)
        {
            auto page = storage.Page(p);

            for (ThI64 e = page->Size() - 1; e >= 0; --e)
            {
                bool excluded = IsEntityExcluded();

                if (excluded)
                {
                    continue;
                }

                func(m_Scene, page->EnitityId(e), page->Data(e));
            }
        }
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
        auto storage1 = m_Scene->GetComponentStorage<TComponent1>();

        if (!storage1)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent1::ComponentId);
            return;
        }

        auto storage2 = m_Scene->GetComponentStorage<TComponent2>();

        if (!storage2)
        {
            THOR_ERR("Component %d is not registered", Private::LoggerTag, TComponent2::ComponentId);
            return;
        }

        for (ThSize p = 0; p < storage->PageCount(); ++p)
        {
            auto page = storage.Page(p);

            for (ThI64 e = page->Size() - 1; e >= 0; --e)
            {
                bool excluded = IsEntityExcluded();

                if (excluded)
                {
                    continue;
                }

                func(m_Scene, page->EnitityId(e), page->Data(e));
            }
        }
    }
private:
    template <class TFunc>
    void ExecuteImpl(TFunc&& func)
    {

    }
};

class ThScene
{
public:

    ThScene(ThWorld* world)
    {
        m_World = world;
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
    void RegisterComponent()
    {
        if (m_Components.Find(TComponent::ComponentId) == m_Components.End())
        {
            auto newComponentStorage = new ThSparseStructuredStorage<TComponent>();
            m_Components[TComponent::ComponentId] = newComponentStorage;
        }
        else
        {
            THOR_ERR("Component %d is already registered", Private::LoggerTag, TComponent::ComponentId);
        }
    }

    template <class... TComponents>
    void RegisterComponents()
    {
        (RegisterComponent<TComponents>(), ...);
    }

    template <class TComponent>
    ThSparseStructuredStorage<TComponent>* GetComponentStorage()
    {
        auto found = m_Components.Find(TComponent::ComponentId);

        if (found != m_Components.End())
        {
            return static_cast<ThSparseStructuredStorage<TComponent>*>(found->Value());
        }

        return nullptr;
    }

    ThComponentStorageBase* GetComponentStorage(ThComponentId componentId)
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
        auto storage = GetComponentStorage<TComponent>();

        if (storage)
        {
            return storage->RemoveComponent(entityId);
        }

        return false;
    }

    template <class TComponent>
    bool SetComponent(ThEntityId entityId, const TComponent& component)
    {
        auto storage = GetComponentStorage<TComponent>();

        if (storage)
        {
            storage->SetComponent(entityId, component);
            return true;
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

    template <class TComponent>
    ThSceneQuery<TComponent> CreateQuery()
    {
        return ThSceneQuery<TComponent>(this);
    }

private:

    ThHashMap<ThComponentId, ThComponentStorageBase*> m_Components;
    ThWorld* m_World = nullptr;
    ThEntityManager* m_EntityManager = nullptr;
};

}