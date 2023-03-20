#pragma once

#include <Thor/Core/Containers/ThGraph.h>
#include <Thor/Core/Containers/ThStack.h>

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
        m_Visited.Resize(graph.Capacity(), false);

        m_Stack.Reserve(graph.Capacity());
        m_Stack.Clear();
        m_Stack.Push(rootVertex);

        while (m_Stack.Size() > 0)
        {
            VertexId currentVertex = m_Stack.Pop();

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

private:
    ThVector<bool> m_Visited;
    ThStack<VertexId> m_Stack;
};

}