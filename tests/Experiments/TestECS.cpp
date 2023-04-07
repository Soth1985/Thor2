#include <Thor/SimdMath/Simd.h>
#include <Thor/ECS/ThECS.h>

using namespace Thor;

struct ComponentIds
{
    enum
    {
        Transform = 1,
        Velocity,
        StaticMesh,
        PhongMaterial
    };
};


struct TransformComponent
{
    static constexpr ThComponentId ComponentId = ComponentIds::Transform;
    
    simd::float4 position;
    simd::float3 scale;
    simd::quatf rotation;
};

int main()
{
    ThCore::Initialize();
    simd::float4 vec1 = {1.0, 2.0, 3.0, 4.0};
    simd::float4 vec2 = {5.0, 6.0, 7.0, 8.0};
    
    simd::float4 vec3 = vec1 + vec2;
    float dot = simd::dot(vec1, vec2);
    ThWorld* world = new ThWorld();
    ThScene* scene = world->CreateScene();
    scene->RegisterComponents<TransformComponent>();
    ThEntityManager* entityManager = scene->GetEntityManager();
    ThEntityId e1 = entityManager->CreateEntity();
    entityManager->DestroyEntity(e1);
    e1 = entityManager->CreateEntity();
    ThEntityId e2 = entityManager->CreateEntity();
    auto index = GetEntityIndex(e2);
    auto generation = GetEntityGeneration(e2);
	return 0;
}
