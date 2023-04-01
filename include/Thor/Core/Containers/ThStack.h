#pragma once

#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

template <class T>
class ThStack
{
public:
    explicit ThStack(ThiMemoryAllocator* allocator = nullptr)
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
        m_Data.PushBack(item);
    }

    T& Top()
    {
        return m_Data.Back();
    }

    const T& Top()const
    {
        return m_Data.Back();
    }

    void Pop()
    {
        m_Data.MoveToBackAndRemove(m_Data.Size() - 1);
    }

    bool Contains(const T& item)
    {
        for (ThSize i = 0; i < m_Data.Size(); ++i)
        {
            if (m_Data[i] == item)
            {
                return true;
            }
        }

        return false;
    }

    const ThVector<T>& Data()const
    {
        return m_Data;
    }
    
private:
    ThVector<T> m_Data;
};

}