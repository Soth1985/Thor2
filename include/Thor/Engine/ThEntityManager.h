#pragma once

#include <Thor/Engine/ThEngineInterface.h>
#include <Thor/Core/Containers/ThHashSet.h>

namespace Thor
{
    class ThEntityManager: public ThiEntityManager
    {
    public:
        ThEntityManager(ThiEngine* engine);
        virtual ThEntity CreateEntity()override;
        virtual bool DestroyEntity(const ThEntity& ent)override;
        virtual bool IsAlive(const ThEntity& ent)override;
        virtual bool IsActive(const ThEntity& ent)override;
        virtual void Reserve(ThI32 capacity)override;
        virtual ThI32 GetNumEntities()override;
        virtual ThEntity GetEntity(ThI32 index)override;
    private:
        ThHashSet<ThEntity> m_Entities;
        ThiEngine* m_Engine;
    };
}
