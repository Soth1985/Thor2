#pragma once

#include <Thor/Engine/ComponentInterfaces.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Math/FixedVector.h>

namespace Thor
{
    struct ThClusterGridComponentData
    {
        ThClusterGridComponentData();
        ThI32 m_Size;
        ThI32 m_Capacity;
        ThEntity* m_Owner;
        ThU64* m_Uid;
        ThVec3f* m_GridSize;
        ThVec3f* m_CellSize;
    };
    
    class ThClusterGridComponent: public ThiClusterGridComponent
    {
    public:
        ThClusterGridComponent();
        
        THOR_INLINE ClusterGridComponent GetComponent(const ThEntity& ent)
        {
            auto entry = m_EntityIndex.Find(ent);
            
            if (entry == m_EntityIndex.End())
                return ClusterGridComponent();
            else
                return ClusterGridComponent(entry->Value());
        }
        
        THOR_INLINE ClusterGridComponent GetComponent(const ThU64& uid)
        {
            auto entry = m_UidIndex.Find(uid);
            
            if (entry == m_UidIndex.End())
                return ClusterGridComponent();
            else
                return ClusterGridComponent(entry->Value());
        }
        
        THOR_INLINE const ThEntity& GetOwner(const ClusterGridComponent& com)const
        {
            return m_Data.m_Owner[com.Index()];
        }
        
        THOR_INLINE const ThU64& GetUid(const ClusterGridComponent& com)const
        {
            return m_Data.m_Uid[com.Index()];
        }
        
        THOR_INLINE const ThVec3f& GetGridSize(const ClusterGridComponent& com)const
        {
            return m_Data.m_GridSize[com.Index()];
        }
        
        THOR_INLINE const ThVec3f& GetCellSize(const ClusterGridComponent& com)const
        {
            return m_Data.m_CellSize[com.Index()];
        }
    private:
        ThClusterGridComponentData m_Data;
        
        ThHashMap<ThEntity, ThI32> m_EntityIndex;
        ThHashMap<ThU64, ThI32> m_UidIndex;
    };
}
