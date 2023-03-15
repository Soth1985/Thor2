#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/ECS/ThSparseStructuredStorage.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

template <class TItem, ThI16 PageSize = Private::PageSize, ThI8 BufferAlignment = Private::PageAlignment>
class ThSparseStructuredStorage
{
public:
    using TBuffer = ThSparseStructuredStorageBuffer<TItem, PageSize, BufferAlignment>;

    ThSize PageCount()const
    {
        return m_Pages.Size();
    }

    TBuffer& operator[](ThSize index)
    {
        return m_Pages[index];
    }

    const TBuffer& operator[](ThSize index)const
    {
        return m_Pages[index];
    }

    ThSize Size()const
    {
        ThSize result = 0;

        for(auto i = m_Pages.Size() - 1; i >= 0; --i)
        {
            result += m_Pages[i].Size();
        }

        return result;
    }

    ThSize Capacity()const
    {
        if (m_Pages.Size() > 0)
        {
            return m_Pages.Size() * m_Pages[0].Capacity;
        }

        return 0;
    }

    bool RemoveComponent(ThEntityId entityId)
    {
        for(auto i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].RemoveComponent(entityId))
            {
                return true;
            }
        }

        return false;
    }

    void SetComponent(ThEntityId entityId, const TItem& component)
    {
        for(auto i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].SetComponent(entityId, component))
            {
                return;
            }
        }
         
        m_Pages.EmplaceBack(TBuffer);
        m_Pages.Back().SetComponent(entityId, component);
    }

    bool HasEntity(ThEntityId entityId)const
    {
        for(auto i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].HasEntity(entityId))
            {
                return true;
            }
        }

        return false;
    }

private:
    ThVector<TBuffer> m_Pages;
};

}