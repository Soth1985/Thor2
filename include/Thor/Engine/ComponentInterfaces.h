#pragma once

#include <Thor/Engine/EngineInterface.h>

namespace Thor
{
    struct eComponentType
    {
        enum Val
        {
            ClusterHash = 1,
            ClusterGrid,
            Transform,
            Mesh,
            Last = 2048
        };
    };
    
    struct eComponentKind
    {
        enum Val
        {
            Clustering = 1,
            Location,
            GeometryShape,
            Light,
            Material,
            Camera,
            Last = 128
        };
    };
    
    struct eEntityFlags
    {
        enum Val
        {
            Active = 1 << 0,
            Dynamic = 1 << 1,
            Prefab = 1 << 2
        };
    };
    
    class ThiClusterHashComponent : public ThiComponentManager
    {
    public:
        
    };
    
    class ThiClusterGridComponent : public ThiComponentManager
    {
    public:
        
    };
    
    class ThiTransformComponent : public ThiComponentManager
    {
    public:
        
    };
    
    class ThiMeshComponent : public ThiComponentManager
    {
    public:
        
    };
    
    typedef ThComponentKey<eComponentType::ClusterHash, eComponentKind::Clustering> ClusterHashComponent;
    typedef ThComponentKey<eComponentType::ClusterGrid, eComponentKind::Clustering> ClusterGridComponent;
    
    typedef ThComponentFKey<eComponentType::ClusterGrid, eComponentKind::Clustering> ClusterGridRef;
}
