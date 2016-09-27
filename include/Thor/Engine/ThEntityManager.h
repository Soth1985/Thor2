#pragma once

#include <Thor/Engine/EngineInterface.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/Containers/ThHashSet.h>
#include <Thor/Core/Containers/ThFlags.h>

namespace Thor
{
    struct eEntityFlags
    {
        enum Val
        {
            Active = 1 << 0
        };
    };
    
    struct ThEntityEntry
    {
        ThClusterHash m_Cluster;
        ThFlags32 m_Flags;
        ThComponentKey<m_Cluster, system> m_ClusterKey;
    };
    
    class ThEntityManager: public ThiEntityManager
    {
    public:
        ThEntityManager(ThiEngine* engine);
        virtual ThEntity CreateEntity(const ThClusterHash& cluster)override;
        virtual ThEntity CreateEntity()override;
        virtual bool DestroyEntity(const ThEntity& ent)override;
        virtual bool IsAlive(const ThEntity& ent)override;
        virtual bool IsActive(const ThEntity& ent)override;
        virtual bool IsClusterActive(const ThClusterHash& cluster)override;
        virtual void SetClusterActive(const ThClusterHash& cluster, bool active)override;
        virtual void Reserve(ThI32 capacity)override;
        virtual ThI32 GetNumEntities()override;
        virtual ThEntity GetEntity(ThI32 index)override;
    private:
        ThHashMap<ThEntity, ThEntityEntry> m_Entities;
        ThHashSet<ThClusterHash> m_ActiveClusters;
        ThiEngine* m_Engine;
    };
}