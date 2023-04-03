#include <Thor/ECS/ThWorld.h>
#include <Thor/ECS/ThScene.h>
#include <Thor/ECS/ThEventManager.h>
#include <Thor/ECS/ThResourceManager.h>

using namespace Thor;

ThWorld::ThWorld()
{
    m_EventManager = new ThEventManager();
    m_ResourceManager = new ThResourceManager();
}

ThWorld::~ThWorld()
{
    for (ThSize i = 0; i < m_Scenes.Size(); ++i)
    {
        delete m_Scenes[i];
    }
    
    delete m_ResourceManager;
    delete m_EventManager;
}

ThEventManager* ThWorld::GetEventManager()
{
    return m_EventManager;
}

ThResourceManager* ThWorld::GetResourceManager()
{
    return m_ResourceManager;
}

ThScene* ThWorld::CreateScene()
{
    ThScene* newScene = new ThScene(this, m_ResourceManager->GenerateUid());
    m_Scenes.PushBack(newScene);
}

ThScene* ThWorld::FindScene(ThU64 sceneUid)
{
    for (ThSize i = 0; i < m_Scenes.Size(); ++i)
    {
        if (m_Scenes[i]->GetUid() == sceneUid)
        {
            return m_Scenes[i];
        }
    }
    
    return nullptr;
}
