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
    
    ThI32 pre;
    simd::float3 position;
    simd::float3 scale;
    simd::quatf rotation;
    ThI32 post;
};

void ExpectEntity(ThEntityId entityId, ThEntityIndex index, ThEntityGeneration generation)
{
    auto eIndex = GetEntityIndex(entityId);
    auto eGeneration = GetEntityGeneration(entityId);
    
    THOR_ASSERT(eIndex == index, "Invalid entity index");
    THOR_ASSERT(eGeneration == generation, "Invalid entity generation");
}

void ExpectAlive(ThEntityManager* entityManager, ThEntityId entityId, bool alive)
{
    bool eAlive = entityManager->IsAlive(entityId);
    
    THOR_ASSERT(eAlive == alive, "Invalid life");
}

void TestEntityManager()
{
    ThWorld* world = new ThWorld();
    ThScene* scene = world->CreateScene();

    ThEntityManager* entityManager = scene->GetEntityManager();
    ThEntityId e1 = entityManager->CreateEntity();
    ExpectEntity(e1, 0, 0);
    ExpectAlive(entityManager, e1, true);
    
    entityManager->DestroyEntity(e1);
    ExpectAlive(entityManager, e1, false);
    
    e1 = entityManager->CreateEntity();
    ExpectEntity(e1, 0, 1);
    
    ThEntityId e2 = entityManager->CreateEntity();
    ExpectEntity(e2, 1, 0);

    entityManager->DestroyEntity(e1);
    entityManager->DestroyEntity(e2);
    ExpectAlive(entityManager, e1, false);
    ExpectAlive(entityManager, e2, false);
    
    e1 = entityManager->CreateEntity();
    e2 = entityManager->CreateEntity();
    ExpectAlive(entityManager, e1, true);
    ExpectAlive(entityManager, e2, true);
    ExpectEntity(e1, 1, 1);
    ExpectEntity(e2, 0, 2);
    
    delete world;
}

void TestAddingComponents()
{
    ThWorld* world = new ThWorld();
    ThScene* scene = world->CreateScene();
    ThEntityManager* entityManager = scene->GetEntityManager();
    ThEntityId e1 = entityManager->CreateEntity();
    
    TransformComponent tr1
    {
        .pre {1},
        .position {1, 1, 1},
        .scale {1, 1, 1},
        .rotation {1, 1, 1, 1},
        .post {1}
    };
    
    TransformComponent tr2
    {
        .pre {2},
        .position {2, 2, 2},
        .scale {2, 2, 2},
        .rotation {2, 2, 2, 2},
        .post {2}
    };
    
    bool componentSet = scene->SetComponent(e1, tr1);
    THOR_ASSERT(componentSet == false, "Cannot set an unregistered component");
    
    bool componentRegistered = scene->RegisterComponents<TransformComponent>();
    THOR_ASSERT(componentRegistered == true, "Should be able to add new component");
    
    componentRegistered = scene->RegisterComponents<TransformComponent>();
    THOR_ASSERT(componentRegistered == false, "Should not be able to add component twice");
    
    componentSet = scene->SetComponent(e1, tr1);
    THOR_ASSERT(componentSet == true, "Can set an registered component");
    componentSet = scene->SetComponent(e1, tr2);
    THOR_ASSERT(componentSet == true, "Can set an registered component");
    
    scene->GetComponent(e1, tr1);
    
    bool pre = (tr1.pre == tr2.pre);
    bool post = (tr1.post == tr2.post);
    THOR_ASSERT(pre && post, "SetComponent properly blitted data");
    
    delete world;
}

int main()
{
    ThCore::Initialize();
    
    TestEntityManager();
    TestAddingComponents();
    
	return 0;
}
