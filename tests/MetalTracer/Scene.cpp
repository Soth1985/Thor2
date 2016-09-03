#include "Scene.h"

using namespace Thor;

ComponentRef Scene::AddSphereShape(const Object& obj, const ThSpheref& sphere)
{
    spheres.PushBack(SphereShape());
    SphereShape* res = &spheres.Back();
    res->owner = obj.id;
    res->shape = sphere;
    return ComponentRef{ComponentType::SphereShape, static_cast<ThI32>(spheres.Size() - 1)};
}

ComponentRef Scene::AddLambertMaterial(const Object& obj, const ThVec3f& albedo)
{
    lamberts.PushBack(LambertMaterial{obj.id, albedo});
    return ComponentRef{ComponentType::LambertMaterial, static_cast<ThI32>(lamberts.Size() - 1)};
}

ComponentRef Scene::AddMetalMaterial(const Object& obj, const ThVec3f& albedo, float fuzziness)
{
    if (fuzziness < 0.0 || fuzziness > 1.0)
        fuzziness = 1.0;
    
    metals.PushBack(MetalMaterial{obj.id, albedo, fuzziness});
    return ComponentRef{ComponentType::MetalMaterial, static_cast<ThI32>(metals.Size() - 1)};
}

ComponentRef Scene::AddDielectricMaterial(const Object& obj, float n)
{
    dielectrics.PushBack(DielectricMaterial{n});
    return ComponentRef{ComponentType::DielectricMaterial, static_cast<ThI32>(dielectrics.Size() - 1)};
}

bool Scene::SetMaterial(const ComponentRef& shape, const ComponentRef& material)
{
    if (shape.type != ComponentType::SphereShape)
        return false;
    
    if (material.type < ComponentType::FirstMaterial)
        return false;
    
    if (material.type > ComponentType::LastMaterial)
        return false;
    
    spheres[shape.index].material = material;
    return true;
}

Object Scene::CreateObject()
{
    ThI32 idx = NextID<Scene>();
    return Object{idx};
}
