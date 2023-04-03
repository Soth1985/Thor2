#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

class ThEntityManager
{
public:
    ThEntityManager(ThSize reserveEntities = Private::PageSize / sizeof(ThEntityId))
    {
        m_ReserveEntities = reserveEntities;
        m_Entities.Reserve(m_ReserveEntities);
    }

    ThEntityId CreateEntity()
    {
        if (m_LastFreeSlot != InvalidEntitySlot)
        {
            ThEntityId newEntity = m_Entities[m_LastFreeSlot];
            ThEntityIndex nextFreeSlot = GetEntityIndex(newEntity);
            newEntity = SetEntityIndex(newEntity, m_LastFreeSlot);
            m_LastFreeSlot = nextFreeSlot;
            ++m_NumAliveEntities;
            return newEntity;
        }
        else
        {
            ThEntityId newEntity = CreateEntityId(m_Entities.Size(), 0);

            if (m_Entities.Size() >= m_Entities.Capacity())
            {
                m_Entities.Reserve(m_Entities.Size() + m_ReserveEntities);
            }

            m_Entities.PushBack(newEntity);
            ++m_NumAliveEntities;
            return newEntity;
        }
    }

    bool DestroyEntity(ThEntityId entityId)
    {
        ThEntityIndex entityIndex = GetEntityIndex(entityId);

        if (entityIndex >= m_Entities.Size())
        {
            return false;
        }

        ThEntityGeneration entityGeneration = GetEntityGeneration(entityId);
        ThEntityGeneration storedEntityGeneration = GetEntityGeneration(m_Entities[entityIndex]);

        if (entityGeneration == storedEntityGeneration)
        {
            m_Entities[entityIndex] = CreateEntityId(m_LastFreeSlot, entityGeneration + 1);
            m_LastFreeSlot = entityIndex;
            --m_NumAliveEntities;
            return true;
        }

        return false;
    }

    bool IsAlive(ThEntityId entityId)
    {
        ThEntityIndex entityIndex = GetEntityIndex(entityId);

        if (entityIndex >= m_Entities.Size())
        {
            return false;
        }

        ThEntityGeneration entityGeneration = GetEntityGeneration(entityId);
        ThEntityGeneration storedEntityGeneration = GetEntityGeneration(m_Entities[entityIndex]);
        return entityGeneration == storedEntityGeneration;
    }

    ThSize GetNumAliveEntities()const
    {
        return m_NumAliveEntities;
    }

    ThSize Size()const
    {
        return m_Entities.Size();
    }

    ThEntityId operator[](ThSize index)const
    {
        return m_Entities[index];
    }

private:
    static constexpr ThSize InvalidEntitySlot {0xFFFFFFFFFFFFFFFF};
    ThVector<ThEntityId> m_Entities;
    ThSize m_LastFreeSlot {InvalidEntitySlot};
    ThSize m_NumAliveEntities {0};
    ThSize m_ReserveEntities {0};
};

}
