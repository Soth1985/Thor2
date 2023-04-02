#pragma once

#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/Containers/ThVector.h>
#include <Thor/ECS/ThEventManager.h>

namespace Thor
{

class ThEventManager;
class ThResourceManager;
class ThScene;

class ThWorld
{
public:
    
    ThWorld()
    {
        m_EventManager = new ThEventManager();
        m_ResourceManager = new ThResourceManager();
    }

    ~ThWorld()
    {
        delete m_ResourceManager;
        delete m_EventManager;
    }

    ThEventManager* GetEventManager()
    {
        return m_EventManager;
    }

    ThResourceManager* GetResourceManager()
    {
        return m_ResourceManager;
    }

    ThScene* CreateScene()
    {

    }

    ThScene* FindScene(ThU64 sceneUid)
    {

    }

private:
    ThEventManager* m_EventManager {nullptr};
    ThResourceManager* m_ResourceManager {nullptr};
    ThVector<ThScene*> m_Scenes;
};

}