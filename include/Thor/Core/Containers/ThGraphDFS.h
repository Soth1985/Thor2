#pragma once

#include <Thor/Core/Containers/ThGraph.h>
#include <Thor/Core/Containers/ThStack.h>
#include <Thor/Core/Containers/ThQueue.h>

namespace Thor
{

template <class TVertexPayload, class TEdgePayload>
class ThGraphDFS
{
public:
    using TGraph = ThGraph<TVertexPayload, TEdgePayload>;
    using VertexId = TGraph::VertexId;

    template <class TCallback>
    void Visit(VertexId rootVertex, const TGraph& graph, TCallback&& callback)
    {
        InitDFS(graph);
        VisitInternal(rootVertex, graph, std::forward(callback));
    }

    template <class TCallback>
    void Visit(const TGraph& graph, TCallback&& callback)
    {
        InitDFS(graph);

        for (auto vertex = graph.Begin(); vertex != graph.End(); ++vertex)
        {
            VertexId vertexId = vertex.Vertex();
            if (!m_Visited[vertexId] && graph[vertexId].Size() == 0)
            {
                VisitInternal(vertexId, graph, std::forward(callback));
            }
        }
    }

    bool HasCycle(VertexId rootVertex, const TGraph& graph)
    {
        InitDFS(graph);
        return HasCycleInternal(rootVertex, graph);
    }

    bool HasCycle(const TGraph& graph)
    {
        InitDFS(graph);

        for (auto vertex = graph.Begin(); vertex != graph.End(); ++vertex)
        {
            VertexId vertexId = vertex.Vertex();
            if (!m_Visited[vertexId] && graph[vertexId].Size() == 0)
            {
                bool hasCycle = HasCycleInternal(vertexId, graph, std::forward(callback));

                if (hasCycle)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void Order(VertexId rootVertex, const TGraph& graph)
    {
        InitDFS(graph);
        InitOrder(graph);
        OrderInternal(rootVertex, graph);
    }

    void Order(const TGraph& graph)
    {
        InitDFS(graph);
        InitOrder(graph);

        for (auto vertex = graph.Begin(); vertex != graph.End(); ++vertex)
        {
            VertexId vertexId = vertex.Vertex();
            if (!m_Visited[vertexId] && graph[vertexId].Size() == 0)
            {
                OrderInternal(vertexId, graph);
            }
        }
    }

    bool TopologicalSort(const TGraph& graph)
    {
        if (HasCycle(graph))
        {
            return false;
        }

        // m_ReversePostOrder is the topological sort
        Order(graph);
        return true;
    }

    const ThQueue<VertexId>& PreOrder()const
    {
        return m_PreOrder;
    }

    const ThQueue<VertexId>& PostOrder()const
    {
        return m_PostOrder;
    }

    const ThStack<VertexId>& ReversePostOrder()const
    {
        return m_ReversePostOrder;
    }

    void Clear()
    {
        m_Visited.Clear();
        m_Stack.Clear();
        m_PreOrder.Clear();
        m_PostOrder.Clear();
        m_ReversePostOrder.Clear();
    }

private:

    void OrderInternal(VertexId rootVertex, const TGraph& graph)
    {
        m_Stack.Push(rootVertex);

        while (m_Stack.Size() > 0)
        {
            VertexId currentVertex = m_Stack.Top();
            m_Stack.Pop();

            if (!m_Visited[currentVertex])
            {
                m_PreOrder.Push(currentVertex);
                m_Visited[currentVertex] = true;
                auto vertexData = graph[VertexId];

                for (ThI64 e = vertexData.Size(); e >= 0; --e)
                {
                    VertexId targetVertex = vertexData[e].To();

                    if (!m_Visited[targetVertex])
                    {
                        m_Stack.Push(targetVertex);
                    }
                }

                m_PostOrder.Push(currentVertex);
                m_ReversePostOrder.Push(currentVertex);
            }
        }
    }

    bool HasCycleInternal(VertexId rootVertex, const TGraph& graph)
    {
        m_Stack.Push(rootVertex);

        while (m_Stack.Size() > 0)
        {
            VertexId currentVertex = m_Stack.Top();
            m_Stack.Pop();

            if (!m_Visited[currentVertex])
            {
                m_Visited[currentVertex] = true;
                auto vertexData = graph[VertexId];

                for (ThI64 e = vertexData.Size(); e >= 0; --e)
                {
                    VertexId targetVertex = vertexData[e].To();

                    if (m_Stack.Contains(targetVertex))
                    {
                        return true;
                    }

                    if (!m_Visited[targetVertex])
                    {
                        m_Stack.Push(targetVertex);
                    }
                }
            }
        }

        return false;
    }

    template <class TCallback>
    void VisitInternal(VertexId rootVertex, const TGraph& graph, TCallback&& callback)
    {
        m_Stack.Push(rootVertex);

        while (m_Stack.Size() > 0)
        {
            VertexId currentVertex = m_Stack.Top();
            m_Stack.Pop();

            if (!m_Visited[currentVertex])
            {
                callback(currentVertex, graph);
                m_Visited[currentVertex] = true;
                auto vertexData = graph[VertexId];

                for (ThI64 e = vertexData.Size(); e >= 0; --e)
                {
                    VertexId targetVertex = vertexData[e].To();

                    if (!m_Visited[targetVertex])
                    {
                        m_Stack.Push(targetVertex);
                    }
                }
            }
        }
    }

    void InitDFS(const TGraph& graph)
    {
        m_Visited.Resize(graph.Capacity(), false);

        m_Stack.Reserve(graph.Capacity());
        m_Stack.Clear();
    }

    void InitOrder(const TGraph& graph)
    {
        m_PreOrder.Reserve(graph.Capacity());
        m_PreOrder.Clear();

        m_PostOrder.Reserve(graph.Capacity());
        m_PostOrder.Clear();

        m_ReversePostOrder.Reserve(graph.Capacity());
        m_ReversePostOrder.Clear();
    }

    ThVector<bool> m_Visited;
    ThStack<VertexId> m_Stack;
    ThQueue<VertexId> m_PreOrder;
    ThQueue<VertexId> m_PostOrder;
    ThStack<VertexId> m_ReversePostOrder;
};

}