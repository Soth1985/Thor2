#pragma once

#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

template <class T>
class ThList
{
private:
    
    static constexpr ThU64 NullNode = 0xFFFFFFFFFFFFFFFF;
    
    struct ListNode
    {
        T m_Payload;
        ThU64 m_Prev = NullNode;
        ThU64 m_Next = NullNode;

        ListNode()
        {

        }

        ListNode(const T& payload)
            :
        m_Payload(payload)
        {

        }
    };

public:

    friend struct Iterator
    {
    public:
        using ValueType = typename T;
        using DifferenceType = ThSize;
        using Reference = T&;
        using Pointer = T*;

        Iterator() 
        {

        }

        explicit Iterator(ThU64 node, ThList<T>* list) 
            : 
        m_Node(node),
        m_List(list)
        {

        }

        // Forward iterator requirements
        Reference operator*() const 
        { 
            return m_List->m_Nodes[m_Node].m_Payload; 
        }

        Pointer operator->() const 
        { 
            return &(m_List->m_Nodes[m_Node].m_Payload); 
        }

        Iterator& operator++() 
        { 
            m_Node = m_List->NextNode(m_Node);
            return *this; 
        }

        Iterator operator++(int) 
        {
            ThU64 nextNode = m_List->NextNode(m_Node);
            return Iterator(nextNode, m_List); 
        }

        // Bidirectional iterator requirements
        Iterator& operator--() 
        { 
            m_Node = m_List->PrevNode(m_Node);
            return *this;
        }

        Iterator operator--(int) 
        {
            ThU64 prevNode = m_List->PrevNode(m_Node);
            return Iterator(prevNode, m_List); 
        }

        bool operator==(const Iterator& rhs)const
        {
            return m_Node == rhs.m_Node && m_List == rhs.m_List;
        }

        bool operator!=(const Iterator& rhs)const
        {
            return !(*this == rhs);
        }

        ThU64 NodeIndex()const
        {
            return m_Node;
        }

    private:
        ThU64 m_Node = NullNode;
        ThList<T>* m_List = nullptr;
    };

    using ConstIterator = const Iterator;

public:

    explicit ThList(ThiMemoryAllocator* allocator = nullptr)
        :
    m_Nodes(allocator)
    {

    }

    ThSize Size()const
    {
        return m_Size;
    }

    ThSize Capacity()const
    {
        return m_Nodes.Capacity();
    }

    void Reserve(ThSize size)
    {
        m_Nodes.Reserve(size);
    }

    void PushFront(const T& item)
    {
        InsertBefore(Begin(), item);
    }

    void PushBack(const T& item)
    {
        InsertAfter(Last(), item);
    }

    T PopFront()
    {
        Iterator begin = Begin();
        T result = *begin;
        Remove(begin);
        return result;
    }

    T PopBack()
    {
        Iterator last = Last();
        T result = *last;
        Remove(last);
        return result;
    }

    Iterator Begin()
    {
        if (m_Size > 0)
        {
            return Iterator(m_ListHead, this);
        }
        else
        {
            return End();
        }
    }

    ConstIterator Begin()const
    {
        if (m_Size > 0)
        {
            return ConstIterator(m_ListHead, this);
        }
        else
        {
            return End();
        }
    }

    Iterator Last()
    {
        if (m_Size > 0)
        {
            return Iterator(m_ListTail, this);
        }
        else
        {
            return End();
        }
    }

    ConstIterator Last()const
    {
        if (m_Size > 0)
        {
            return ConstIterator(m_ListTail, this);
        }
        else
        {
            return End();
        }
    }

    Iterator End()
    {
        return Iterator(NullNode, this);
    }

    ConstIterator End()const
    {
        return ConstIterator(NullNode, this);
    }

    Iterator InsertBefore(Iterator position, const T& item)
    {
        ThU64 newNodeIndex = AddNewNode(item);

        if (position != End())
        {
            ThU64 positionIndex = position.NodeIndex();
            m_Nodes[newNodeIndex].m_Next = positionIndex;
            m_Nodes[positionIndex].m_Prev = newNodeIndex;

            if (positionIndex == m_ListHead)
            {
                m_ListHead = newNodeIndex;
            }
        }
        
        return Iterator(newNodeIndex, this);
    }

    Iterator InsertAfter(Iterator position, const T& item)
    {
        ThU64 newNodeIndex = AddNewNode(item);

        if (position != End())
        {
            ThU64 positionIndex = position.NodeIndex();
            m_Nodes[newNodeIndex].m_Prev = positionIndex;
            m_Nodes[positionIndex].m_Next = newNodeIndex;

            if (positionIndex == m_ListTail)
            {
                m_ListTail = newNodeIndex;
            }
        }
        
        return Iterator(newNodeIndex, this);
    }

    void Remove(Iterator position)
    {
        ThU64 positionIndex = position.GetIndex();

        if (positionIndex >= 0 && positionIndex < m_Nodes.Size())
        {
            if (positionIndex == m_ListHead)
            {
                m_ListHead = m_Nodes[positionIndex].m_Next;
            }

            if (positionIndex == m_ListTail)
            {
                m_ListTail = m_Nodes[positionIndex].m_Prev;
            }

            m_Nodes[positionIndex].m_Next = m_FirstFreeNode;
            m_FirstFreeNode = positionIndex;
            DestroyObjects(&m_Nodes[positionIndex].m_Payload, 1);
            --m_Size;
        }
    }

private:

    ThU64 NextNode(ThU64 node)
    {
        if (node != NullNode)
        {
            return m_Nodes[node].m_Next;
        }

        return NullNode;
    }

    ThU64 PrevNode(ListNode* node)
    {
        if (node != NullNode)
        {
            return m_Nodes[node].m_Prev;
        }

        return NullNode;
    }

    ThU64 AddNewNode(const T& item)
    {
        ThU64 resultIndex = NullNode;
        if (m_FirstFreeNode != NullNode)
        {
            resultIndex = m_FirstFreeNode;
            m_Nodes[resultIndex].m_Payload = item;
            m_FirstFreeNode = m_Nodes[resultIndex].m_Next;
            m_Nodes[resultIndex].m_Next = NullNode;
            m_Nodes[resultIndex].m_Prev = NullNode;
        }
        else
        {
            m_Nodes.PushBack(ListNode(item));
            resultIndex = m_Nodes.Size() - 1;
        }

        if (m_ListHead == NullNode)
        {
            m_ListHead = resultIndex;
        }

        if (m_ListTail == NullNode)
        {
            m_ListTail = resultIndex;
        }

        ++m_Size;
        return resultIndex;
    }

    ThVector<ListNode> m_Nodes;
    ThU64 m_FirstFreeNode = NullNode;
    ThU64 m_ListHead = NullNode;
    ThU64 m_ListTail = NullNode;
    ThSize m_Size = 0;
};

}