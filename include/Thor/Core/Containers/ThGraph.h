#pragma once

#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

template <class TVertexPayload, class TEdgePayload>
class ThGraph
{
public:
    using VertexId = ThU64;
    using EdgeId = ThU64;
    static constexpr VertexId NullVertex = 0xFFFFFFFFFFFFFFFF;
    static constexpr VertexId NullEdge = 0xFFFFFFFFFFFFFFFF;

public:
    struct Edge
    {
    public:
        Edge()
        {

        }

        Edge(EdgeId id, VertexId from, VertexId to, const TEdgePayload& payload)
            :
        m_From(from),
        m_To(to),
        m_Id(id),
        m_Payload(payload)
        {

        }

        VertexId From()const
        {
            return m_From;
        }

        VertexId To()const
        {
            return m_To;
        }

        EdgeId Id()const
        {
            return m_Id;
        }

        const TEdgePayload& Payload()const
        {
            return m_Payload;
        }

        TEdgePayload& Payload()
        {
            return m_Payload;
        }
    private:
        VertexId m_From = NullVertex;
        VertexId m_To = NullVertex;
        EdgeId m_Id = NullEdge;
        TEdgePayload m_Payload = default(TEdgePayload);

        friend class ThGraph;
    };

public:
    struct Vertex
    {
    public:
        using AdjacencyList = ThVector<Edge>;
        using EdgeIterator = AdjacencyList::Iterator;
        using ConstEdgeIterator = AdjacencyList::ConstIterator;

        Vertex(ThiMemoryAllocator* allocator = nullptr)
            :
        m_Adjacency(allocator)
        {

        }

        Vertex(const TVertexPayload& payload, ThiMemoryAllocator* allocator = nullptr)
            :
        m_Payload(payload),
        m_Adjacency(allocator)
        {

        }

        const ThVector<Edge>& Adjacency()const
        {
            return m_Adjacency;
        }

        ThSize Size()const
        {
            return m_Adjacency.Size();
        }

        Edge& operator[](ThSize index)
        {
            return m_Adjacency[index];
        }

        const Edge& operator[](ThSize index)const
        {
            return m_Adjacency[index];
        }

        EdgeIterator Begin()
        {
            return m_Adjacency.Begin();
        }

        ConstEdgeIterator Begin()const
        {
            return m_Adjacency.Begin();
        }

        EdgeIterator End()
        {
            return m_Adjacency.End();
        }

        ConstEdgeIterator End()const
        {
            return m_Adjacency.End();
        }

        TVertexPayload& Payload()
        {
            return m_Payload;
        }

        const TVertexPayload& Payload()const
        {
            return m_Payload;
        }
    private:
        AdjacencyList m_Adjacency;
        TVertexPayload m_Payload = default(TVertexPayload);
        VertexId m_NextFreeVertex = NullVertex;
        bool m_Deallocated = false;

        friend class ThGraph;
    };

public:
    friend struct VertexIterator
    {
    public:
        using ValueType = Vertex;
        using DifferenceType = ThSize;
        using Reference = Vertex&;
        using Pointer = Vertex*;

        VertexIterator() 
        {

        }

        explicit VertexIterator(VertexId vertexId, ThGraph* graph) 
            : 
        m_VertexId(vertexId),
        m_Graph(graph)
        {

        }

        // Forward iterator requirements
        Reference operator*() const 
        { 
            return m_Graph->m_Vertices[m_VertexId]; 
        }

        Pointer operator->() const 
        { 
            return &(m_Graph->m_Vertices[m_VertexId]); 
        }

        Iterator& operator++() 
        { 
            m_VertexId = m_Graph->NextVertex(m_VertexId);
            return *this; 
        }

        VertexIterator operator++(int) 
        {
            VertexId nextVertex = m_Graph->NextVertex(m_VertexId);
            return VertexIterator(nextVertex, m_Graph); 
        }

        // Bidirectional iterator requirements
        VertexIterator& operator--() 
        { 
            m_VertexId = m_Graph->PrevVertex(m_VertexId);
            return *this;
        }

        VertexIterator operator--(int) 
        {
            VertexId prevVertex = m_Graph->PrevVertex(m_VertexId);
            return VertexIterator(prevVertex, m_Graph); 
        }

        bool operator==(const VertexIterator& rhs)const
        {
            return m_VertexId == rhs.m_VertexId && m_Graph == rhs.m_Graph;
        }

        bool operator!=(const VertexIterator& rhs)const
        {
            return !(*this == rhs);
        }

        VertexId Vertex()const
        {
            return m_VertexId;
        }

    private:
        VertexId m_VertexId = NullVertex;
        ThGraph* m_Graph = nullptr;
    };

    using ConstVertexIterator = const VertexIterator;

public:

    ThGraph(ThiMemoryAllocator* allocator = nullptr)
        :
    m_Vertices(allocator)
    {

    }

    ThSize Size()const
    {
        return m_Size;
    }

    ThSize Capacity()const
    {
        return m_Vertices.Capacity();
    }

    VertexIterator Begin()
    {
        if (m_Size > 0)
        {
            return VertexIterator(0, this);
        }
        else
        {
            return End();
        }
    }

    ConstVertexIterator Begin()const
    {
        if (m_Size > 0)
        {
            return ConstVertexIterator(0, this);
        }
        else
        {
            return End();
        }
    }

    VertexIterator End()
    {
        return VertexIterator(NullVertex, this);
    }

    ConstVertexIterator End()const
    {
        return ConstVertexIterator(NullVertex, this);
    }

    void ReserveVertices(ThSize capacity)
    {
        m_Vertices.Reserve(capacity);
    }

    void ReserveEdges(ThSize capacity)
    {
        m_ReserveEdges = capacity;

        for (ThSize i = 0; i < m_Vertices.Size(); ++i)
        {
            m_Vertices[i].m_Adjacency.Reserve(m_ReserveEdges);
        }
    }

    VertexId AddVertex(const TVertexPayload& payload)
    {
        VertexId result = NullVertex;

        if (m_FirstFreeVertex == NullVertex)
        {
            m_Vertices.PushBack(Vertex(payload, m_Vertices.GetAllocator()));
            result = m_Vertices.Size() - 1;
        }
        else
        {
            result = m_FirstFreeVertex;
            m_Vertices[result].m_Payload = payload;
            m_FirstFreeVertex = m_Vertices[result].m_NextFreeVertex;
            m_Vertices[result].m_NextFreeVertex = NullVertex;
        }

        m_Vertices[result].m_Adjacency.Reserve(m_ReserveEdges);
        ++m_Size;
        return result;
    }

    void RemoveVertex(VertexId vertex)
    {
        if (vertex >= m_Vertices.Size())
        {
            return;
        }

        DestroyObject(&m_Vertices[vertex].m_Payload);
        m_Vertices[vertex].m_Deallocated = true;
        m_Vertices[vertex].m_NextFreeVertex = m_FirstFreeVertex;
        m_Vertices[vertex].m_Adjacency.Clear();
        m_FirstFreeVertex = vertex;
        --m_Size;
    }

    EdgeId AddEdge(VertexId from, VertexId to, const TEdgePayload& payload)
    {
        EdgeId result = m_NextEdgeId;
        ++m_NextEdgeId;
        m_Vertices[from].m_Adjacency.PushBack(Edge(result, from, to, payload));

        return result;
    }

    bool RemoveEdge(EdgeId edge)
    {
        VertexId sourceVertex;
        ThSize edgeIndex;

        if (FindEdge(edge, sourceVertex, edgeIndex))
        {
            m_Vertices[sourceVertex].m_Adjacency.MoveToBackAndRemove(edgeIndex);
        }
        
        return false;
    }

    Edge* FindEdge(EdgeId edge)
    {
        VertexId sourceVertex;
        ThSize edgeIndex;

        if (FindEdge(edge, sourceVertex, edgeIndex))
        {
            return &m_Vertices[sourceVertex].m_Adjacency[edgeIndex];
        }

        return nullptr;
    }

    const Edge* FindEdge(EdgeId edge)const
    {
        ThGraph* self = const_cast<ThGraph*>(this);
        return self->FindEdge(edge);
    }

    Vertex& operator[](VertexId vertex)
    {
        return m_Vertices[vertex];
    }

    const Vertex& operator[](VertexId vertex)const
    {
        return m_Vertices[vertex];
    }
private:

    bool FindEdge(EdgeId edge, VertexId& sourceVertex, ThSize& edgeIndex)const
    {
        sourceVertex = NullVertex;
        edgeIndex = NullEdge;

        for (ThSize v = 0; v < m_Vertices.Size(); ++v)
        {
            for (ThSize e = 0; e < m_Vertices[v].Size(); ++e)
            {
                if (m_Vertices[v][e].m_Id == edge)
                {
                    sourceVertex = v;
                    edgeIndex = e;
                    return true;
                }
            }
        }
        
        return false;
    }

    VertexId NextVertex(VertexId vertex)const
    {
        VertexId result = NullVertex;

        for (VertexId v = vertex + 1; v < m_Vertices.Size(); ++v)
        {
            if (!m_Vertices[v].m_Deallocated)
            {
                result = v;
                break;
            }
        }

        return result;
    }

    VertexId PrevVertex(VertexId vertex)const
    {
        VertexId result = NullVertex;

        if (vertex == 0)
        {
            return result;
        }

        for (VertexId v = vertex - 1; v >= 0; --v)
        {
            if (!m_Vertices[v].m_Deallocated)
            {
                result = v;
                break;
            }
        }

        return result;
    }

    ThVector<Vertex> m_Vertices;
    VertexId m_FirstFreeVertex = NullVertex;
    ThSize m_ReserveEdges = 0;
    ThSize m_Size = 0;
    EdgeId m_NextEdgeId = 0;
};

}