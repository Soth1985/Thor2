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
    
    ThWorld();

    ~ThWorld();

    ThEventManager* GetEventManager();

    ThResourceManager* GetResourceManager();

    ThScene* CreateScene();

    ThScene* FindScene(ThU64 sceneUid);

private:
    ThEventManager* m_EventManager {nullptr};
    ThResourceManager* m_ResourceManager {nullptr};
    ThVector<ThScene*> m_Scenes;
};

}
