#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/ECS/ThSparseStructuredStorageBuffer.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

class ThSparseStructuredStorage
{
public:

    ThSparseStructuredStorage(ThComponentId componentId, ThSize componentDataSize, ThI16 pageSize = Private::PageSize, ThI8 bufferAlignment = Private::PageAlignment)
        :
    m_ComponentId(componentId),
    m_ComponentDataSize(componentDataSize),
    m_PageSize(pageSize),
    m_BufferAlignment(bufferAlignment)
    {

    }

    ThSize PageCount()const
    {
        return m_Pages.Size();
    }

    ThSparseStructuredStorageBuffer* Page(ThSize index)
    {
        return &m_Pages[index];
    }

    const ThSparseStructuredStorageBuffer* Page(ThSize index)const
    {
        return &m_Pages[index];
    }

    ThSize ComponentsCount()const
    {
        ThSize result = 0;

        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            result += m_Pages[i].Size();
        }

        return result;
    }

    ThSize Capacity()const
    {
        if (m_Pages.Size() > 0)
        {
            return m_Pages.Size() * m_Pages[0].Capacity();
        }

        return 0;
    }

    bool RemoveComponent(ThEntityId entityId)
    {
        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].RemoveComponent(entityId))
            {
                return true;
            }
        }

        return false;
    }

    void SetComponent(ThEntityId entityId, const ThI8* componentData)
    {
        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].SetComponent(entityId, componentData))
            {
                return;
            }
        }
         
        m_Pages.EmplaceBack(m_ComponentDataSize, m_PageSize, m_BufferAlignment);
        m_Pages.Back().SetComponent(entityId, componentData);
    }

    bool GetComponent(ThEntityId entityId, ThI8* componentData)
    {
        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].GetComponent(entityId, componentData))
            {
                return true;
            }
        }

        return false;
    }

    bool HasEntity(ThEntityId entityId)const
    {
        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].HasEntity(entityId))
            {
                return true;
            }
        }

        return false;
    }

private:
    ThVector<ThSparseStructuredStorageBuffer> m_Pages;
    ThComponentId m_ComponentId {0}; 
    ThSize m_ComponentDataSize {0};
    ThI16 m_PageSize {0};
    ThI8 m_BufferAlignment {0};
};

}
