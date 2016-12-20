#pragma once

#include <Thor/Engine/ComponentInterfaces.h>
#include <Thor/Core/Containers/ThHashMap.h>

namespace Thor
{
    struct ThClusterHashComponentData
    {
        ThClusterHashComponentData();
        
        ThI32 m_Size;
        ThI32 m_Capacity;
        
        ThEntity* m_Owner;
        ThU64* m_Uid;
        ThClusterHash* m_Cluster;
        ClusterGridRef* m_Grid;
    };
    
    class ThClusterHashComponent: public ThiClusterHashComponent
    {
    public:
        ThClusterHashComponent();
        
        THOR_INLINE ClusterHashComponent GetComponent(const ThEntity& ent)
        {
            auto entry = m_EntityIndex.Find(ent);
            
            if (entry == m_EntityIndex.End())
                return ClusterHashComponent();
            else
                return ClusterHashComponent(entry->Value());
        }
        
        THOR_INLINE ClusterHashComponent GetComponent(const ThU64& uid)
        {
            auto entry = m_UidIndex.Find(uid);
            
            if (entry == m_UidIndex.End())
                return ClusterHashComponent();
            else
                return ClusterHashComponent(entry->Value());
        }
        
        THOR_INLINE const ThEntity& GetOwner(const ClusterHashComponent& com)const
        {
            return m_Data.m_Owner[com.Index()];
        }
        
        THOR_INLINE const ThU64& GetUid(const ClusterHashComponent& com)const
        {
            return m_Data.m_Uid[com.Index()];
        }
        
        THOR_INLINE const ThClusterHash& GetCluster(const ClusterHashComponent& com)const
        {
            return m_Data.m_Cluster[com.Index()];
        }
        
        THOR_INLINE const ClusterGridRef& GetGrid(const ClusterHashComponent& com)const
        {
            return m_Data.m_Grid[com.Index()];
        }
    private:
        ThClusterHashComponentData m_Data;
        
        
        ThHashMap<ThEntity, ThI32> m_EntityIndex;
        ThHashMap<ThU64, ThI32> m_UidIndex;
    };
}
