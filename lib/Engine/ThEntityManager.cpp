#include <Thor/Engine/ThEntityManager.h>
#include <Thor/Core/Debug/ThAssert.h>

using namespace Thor;

ThEntityManager::ThEntityManager(ThiEngine* engine)
    :
m_Engine(engine)
{
    
}

ThEntity ThEntityManager::CreateEntity()
{
    ThU64 uid = m_Engine->GenerateUid();
    ThEntity result(uid);
    bool inserted = m_Entities.Insert(result);
    THOR_ASSERT(inserted, "Duplicate uid detected");
    return result;
}

bool ThEntityManager::DestroyEntity(const ThEntity& ent)
{
    return m_Entities.Erase(ent);
}

bool ThEntityManager::IsAlive(const ThEntity& ent)
{
    return m_Entities.Find(ent) != m_Entities.End();
}

void ThEntityManager::Reserve(ThI32 capacity)
{
    m_Entities.Reserve(capacity);
}

ThI32 ThEntityManager::GetNumEntities()
{
    return m_Entities.Size();
}

ThEntity ThEntityManager::GetEntity(ThI32 index)
{
    return m_Entities.GetItem(index).Key();
}
