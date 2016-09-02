#pragma once

#include <Thor/Math/Math.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{
    enum class ComponentType : ThI32
    {
        FirstMaterial,
        NormalMapMaterial,
        LastStatelessMaterial,
        LambertMaterial,
        MetalMaterial,
        LastMaterial,
        SphereShape,
        LastShape
    };
    
    struct ComponentRef
    {
        ComponentType type;
        ThI32 index;
    };
    
    struct Object
    {
        ThI32 id;
    };
    
    struct SphereShape
    {
        ThI32 owner;
        ThSpheref shape;
        ComponentRef material;
    };
    
    struct LambertMaterial
    {
        ThI32 owner;
        ThVec3f albedo;
    };
    
    struct MetalMaterial
    {
        ThI32 owner;
        ThVec3f albedo;
    };
    
    struct Scene
    {
        ThVector<SphereShape> spheres;
        ThVector<LambertMaterial> lamberts;
        ThVector<MetalMaterial> metals;
        ThVector<Object> objects;
        
        ComponentRef AddSphereShape(const Object& obj, const ThSpheref& sphere);
        ComponentRef AddLambertMaterial(const Object& obj, const ThVec3f& albedo);
        ComponentRef AddMetalMaterial(const Object& obj, const ThVec3f& albedo);
        bool SetMaterial(const ComponentRef& shape, const ComponentRef& material);
        Object CreateObject();
    };
}
