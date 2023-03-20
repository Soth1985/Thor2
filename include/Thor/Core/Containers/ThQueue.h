#pragma once

#include <Thor/Core/Containers/ThList.h>

namespace Thor
{

template <class T>
class ThQueue
{
public:
    explicit ThQueue(ThiMemoryAllocator* allocator = nullptr)
        :
    m_Data(allocator)
    {

    }

    ThSize Size()const
    {
        return m_Data.Size();
    }

    ThSize Capacity()const
    {
        return m_Data.Capacity();
    }

    void Clear()
    {
        m_Data.Clear();
    }

    void Reserve(ThSize size)
    {
        m_Data.Reserve(size);
    }

    void Push(const T& item)
    {
        m_Data.PushLast(item);
    }

    void Pop()
    {
        m_Data.PopFront();
    }
    
    T& Front()
    {
        return *m_Data.Begin();
    }

    const T& Front()const
    {
        return *m_Data.Begin();
    }

    T& Last()
    {
        return *m_Data.Last();
    }

    const T& Last()const
    {
        return *m_Data.Back();
    }

    bool Contains(const T& item)
    {
        for (auto i = m_Data.Begin(); i != m_Data.End(); ++i)
        {
            if (*i == item)
            {
                return true;
            }
        }

        return false;
    }
private:
    ThList<T> m_Data;
};

}