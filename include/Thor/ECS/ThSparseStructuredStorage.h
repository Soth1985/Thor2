#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/ECS/ThSparseStructuredStorage.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

class ThComponentStorageBase
{
public:
    virtual ~ThComponentStorageBase()
    {

    }

    virtual bool HasEntity(ThEntityId entityId)const = 0;
};

template <class TItem, ThI16 PageSize = Private::PageSize, ThI8 BufferAlignment = Private::PageAlignment>
class ThSparseStructuredStorage: public ThComponentStorageBase
{
public:
    using TComponent = TItem; 
    using TBuffer = ThSparseStructuredStorageBuffer<TItem, PageSize, BufferAlignment>;

    ThSize PageCount()const
    {
        return m_Pages.Size();
    }

    TBuffer* Page(ThSize index)
    {
        return &m_Pages[index];
    }

    const TBuffer* Page(ThSize index)const
    {
        return &m_Pages[index];
    }

    ThSize NumComponents()const
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

    void SetComponent(ThEntityId entityId, const TItem& component)
    {
        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].SetComponent(entityId, component))
            {
                return;
            }
        }
         
        m_Pages.EmplaceBack(TBuffer);
        m_Pages.Back().SetComponent(entityId, component);
    }

    bool GetComponent(ThEntityId entityId, TItem& component)
    {
        for(ThI64 i = m_Pages.Size() - 1; i >= 0; --i)
        {
            if (m_Pages[i].GetComponent(entityId, component))
            {
                return true;
            }
        }

        return false;
    }

    virtual bool HasEntity(ThEntityId entityId)const override
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
    ThVector<TBuffer> m_Pages;
};

}