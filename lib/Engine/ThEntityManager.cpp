#include <Thor/Engine/ThEntityManager.h>

using namespace Thor;

ThEntityManager::ThEntityManager(ThiEngine* engine)
    :
m_Engine(engine),
m_FreeIndex(-INT32_MAX)
{
    
}

ThEntity ThEntityManager::CreateEntity()
{
    ThU64 uid = m_Engine->GenerateUid();
    if (m_FreeIndex != -INT32_MAX)
    {
        ThI32 index = -m_FreeIndex;
        m_FreeIndex = m_Entities[index].Index();
        m_Entities[index] = ThEntity(uid, index);
        return m_Entities[index];
    }
    else
    {
        ThI32 index = m_Entities.Size();
        ThEntity result(uid, index);
        m_Entities.PushBack(result);
        return result;
    }
}

bool ThEntityManager::DestroyEntity(const ThEntity& ent)
{
    if (IsAlive(ent))
    {
        ThI32 index = ent.Index();
        m_Entities[ent.Index()] = ThEntity(0, m_FreeIndex);
        m_FreeIndex = -index;
        return true;
    }
    
    return false;
}

bool ThEntityManager::IsAlive(const ThEntity& ent)
{
    return ent && m_Entities[ent.Index()].Uid() == ent.Uid();
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
    return m_Entities[index];
}