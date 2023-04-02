#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/Memory/ThMemory.h>
#include <Thor/Core/Hash/ThIntHash.h>

namespace Thor
{

class ThSparseStructuredStorageBuffer
{
public:
    ThSparseStructuredStorageBuffer(ThSize componentDataSize, ThI16 pageSize = Private::PageSize, ThI8 bufferAlignment = Private::PageAlignment)
    {
        m_Buffer = ThMemory::AlignedMalloc(pageSize, bufferAlignment);

        if (!m_Buffer)
        {
            THOR_ERR("Failed to allocate memory", Private::LoggerTag);
            return;
        }

        ThU16 entrySize = componentDataSize + sizeof(ThEntitySparseIndex) + sizeof(ThEntityId);
        m_Capacity = pageSize / entrySize;

        m_EntitiesSparse = m_Buffer;
        m_EntitiesDense = m_EntitiesSparse + m_Capacity * sizeof(ThEntitySparseIndex);
        m_ComponentData = m_EntitiesSparse + m_Capacity * sizeof(ThEntityId);

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

    bool GetComponent(ThEntityId entityId, ThI8* componentData)
    {
        ThEntitySparseIndex entityIndexHash = HashEntityIndex(entityId);
        ThEntitySparseIndex sparseIndex = m_EntitiesSparse[entityIndexHash];

        if (sparseIndex == ThEntitySparseNull)
        {
            return false;
        }

        ThEntityId storedEntity = m_EntitiesDense[sparseIndex];

        if (storedEntity == entityId)
        {
            ThI8* sourceData = m_ComponentData + sparseIndex * m_ComponentDataSize;
            ThMemory::MemoryCopy(componentData, sourceData, m_ComponentDataSize);
            return true;
        }

        return false;
    }

    bool GetComponentNoCopy(ThEntityId entityId, ThI8** componentData)
    {
        ThEntitySparseIndex entityIndexHash = HashEntityIndex(entityId);
        ThEntitySparseIndex sparseIndex = m_EntitiesSparse[entityIndexHash];

        if (sparseIndex == ThEntitySparseNull)
        {
            return false;
        }

        ThEntityId storedEntity = m_EntitiesDense[sparseIndex];

        if (storedEntity == entityId)
        {
            ThI8* sourceData = m_ComponentData + sparseIndex * m_ComponentDataSize;
            *componentData = sourceData;
            return true;
        }

        return false;
    }

    bool SetComponent(ThEntityId entityId, const ThI8* componentData)
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
            ThI8* destinationData = m_ComponentData + m_Size * m_ComponentDataSize;
            ThMemory::MemoryCopy(destinationData, componentData, m_ComponentDataSize);
            ++m_Size;

            return true;
        }

        // See if slot is pointing to the same entityId and overwrite data
        ThEntityId storedEntity = m_EntitiesDense[sparseIndex];

        if (storedEntity == entityId)
        {
            ThI8* destinationData = m_ComponentData + sparseIndex * m_ComponentDataSize;
            ThMemory::MemoryCopy(destinationData, componentData, m_ComponentDataSize);
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
        ThEntityId lastEntityId = m_EntitiesDense[m_Size - 1];
        ThEntitySparseIndex lastEntityIndexHash = HashEntityIndex(lastEntityId);
        ThEntitySparseIndex lastEntitySparseIndex = m_EntitiesSparse[lastEntityIndexHash];
        m_EntitiesSparse[entityIndexHash] = lastEntitySparseIndex;
        m_EntitiesSparse[lastEntityIndexHash] = ThEntitySparseNull;
        m_EntitiesDense[sparseIndex] = lastEntityId;
        ThI8* sourceData = m_ComponentData + (m_Size - 1) * m_ComponentDataSize;
        ThI8* destinationData = m_ComponentData + sparseIndex * m_ComponentDataSize;
        ThMemory::MemoryCopy(destinationData, sourceData, m_ComponentDataSize);

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

    ThI8* ComponentData(ThU16 index)
    {
        return m_ComponentData + index * m_ComponentDataSize;
    }

    const ThI8* ComponentData(ThU16 index)const
    {
        return m_ComponentData + index * m_ComponentDataSize;
    }

    ThEntityId EntityId(ThU16 index)const
    {
        return m_EntitiesDense[index];
    }

private:
    ThEntitySparseIndex*  m_EntitiesSparse {nullptr};
    ThEntityId* m_EntitiesDense {nullptr};
    ThI8* m_ComponentData {nullptr};
    ThI8* m_Buffer {nullptr};
    ThU16 m_Size {0};
    ThU16 m_Capacity {0};
    ThSize m_ComponentDataSize {0};
};

}