#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/Memory/ThMemory.h>
#include <Thor/Core/Hash/ThIntHash.h>

namespace Thor
{

template <class TItem, ThI16 PageSize = Private::PageSize, ThI8 BufferAlignment = Private::PageAlignment>
class ThSparseStructuredStorageBuffer
{
public:
    ThSparseStructuredStorageBuffer()
    {
        m_Buffer = ThMemory::AlignedMalloc(PageSize, BufferAlignment);

        if (!m_Buffer)
        {
            THOR_ERR("Failed to allocate memory", Private::LoggerTag);
            return;
        }

        ThU16 EntrySize = sizeof(TItem) + sizeof(ThEntitySparseIndex) + sizeof(ThEntityId);
        m_Capacity = PageSize / EntrySize;

        m_EntitiesSparse = m_Buffer;
        m_EntitiesDense = m_EntitiesSparse + m_Capacity * sizeof(ThEntitySparseIndex);
        m_Items = m_EntitiesSparse + m_Capacity * sizeof(ThEntityId);

        memset(m_EntitiesSparse, ThEntitySparseNull, sizeof(ThEntitySparseIndex) * m_Capacity);
    }

    ~ThSparseStructuredStorageBuffer()
    {
        if (m_Buffer)
        {
            ThMemory::AlignedFree(m_Buffer);
            m_Buffer = nullptr;
        }
    }

    ThEntitySparseIndex HashEntityIndex(ThEntityId entityId)
    {
        //return GetEntityIndex(entityId) % m_Capacity;
        return Thor::Hash::HashInt32(GetEntityIndex(entityId)) % m_Capacity;
    }

    bool HasEntity(ThEntityId entityId)const
    {
        ThEntitySparseIndex entityIndexHash = HashEntityIndex(entityId);
        ThEntitySparseIndex sparseIndex = m_EntitiesSparse[entityIndexHash];

        if (sparseIndex == ThEntitySparseNull)
        {
            return false;
        }

        return m_EntitiesDense[sparseIndex] == entityId;
    }

    bool AddComponent(ThEntityId entityId, const TItem& component)
    {
        // Out of space
        if (m_Size >= m_Capacity)
        {
            return false;
        }

        ThEntitySparseIndex entityIndexHash = HashEntityIndex(entityId);
        ThEntitySparseIndex sparseIndex = m_EntitiesSparse[entityIndexHash];

        // Slot is empty, add new component data
        if (sparseIndex == ThEntitySparseNull)
        {
            m_EntitiesSparse[sparseIndex] = m_Size;
            m_EntitiesDense[m_Size] = entityId;
            m_Items[m_Size] = component;
            ++m_Size;

            return true;
        }

        // See if slot is pointing to the same entityId and overwrite data
        ThEntityId storedEntity = m_EntitiesDense[sparseIndex];

        if (storedEntity == entityId)
        {
            m_Items[sparseIndex] = component;
            return true;
        }

        return false;
    }

    bool RemoveComponent(ThEntityId entityId)
    {
        if (m_Size == 0)
        {
            return false;
        }

        ThEntitySparseIndex entityIndexHash = HashEntityIndex(entityId);
        ThEntitySparseIndex sparseIndex = m_EntitiesSparse[entityIndexHash];

        // Entity is not in buffer
        if (sparseIndex == ThEntitySparseNull)
        {
            return false;
        }

        // A different entity occupies the slot
        if (m_EntitiesDense[sparseIndex] != entityId)
        {
            return false;
        }

        // Swap with last
        ThEntitySparseIndex lastEntityIndexHash = HashEntityIndex(m_EntitiesDense[m_Size - 1]);
        ThEntitySparseIndex lastEntitySparseIndex = m_EntitiesSparse[lastEntityIndexHash];
        m_EntitiesSparse[entityIndexHash] = lastEntitySparseIndex;
        m_EntitiesSparse[lastEntityIndexHash] = ThEntitySparseNull;
        m_Items[sparseIndex] = m_Items[m_Size - 1];

        --m_Size;
        return true;
    }

    ThU16 Size()const
    {
        return m_Size;
    }

    ThU16 Capacity()const
    {
        return m_Capacity;
    }

    TItem& operator[](ThU16 index)
    {
        return m_Items[index];
    }

    const TItem& operator[](ThU16 index)const
    {
        return m_Items[index];
    }

private:
    ThEntitySparseIndex*  m_EntitiesSparse = nullptr;
    ThEntityId* m_EntitiesDense = nullptr;
    TItem* m_Items = nullptr;
    ThI8* m_Buffer = nullptr;
    ThU16 m_Size = 0;
    ThU16 m_Capacity = 0;
};

}