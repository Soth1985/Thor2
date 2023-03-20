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

    T Pop()
    {
        T result = m_Data.Back();
        m_Data.MoveToBackAndRemove(m_Data.Size() - 1);
        return result;
    }

    T* Peek()
    {
        if (m_Data.Size() > 0)
        {
            return &m_Data.Back();
        }

        return nullptr;
    }

    const T* Peek()const
    {
        if (m_Data.Size() > 0)
        {
            return &m_Data.Back();
        }

        return nullptr;
    }
private:
    ThVector<T> m_Data;
};

}