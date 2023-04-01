#pragma once

#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/ECS/ThEventManager.h>

namespace Thor
{

class ThWorld
{
public:
    
    ThWorld()
    {
        m_EventManager = new ThEventManager();
    }

    ~ThWorld()
    {
        delete m_EventManager;
    }

    ThEventManager* GetEventManager()
    {
        return m_EventManager;
    }

private:
    ThEventManager* m_EventManager;
};

}