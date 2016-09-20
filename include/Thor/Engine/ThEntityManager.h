#pragma once

#include <Thor/Engine/Interface.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{
    class ThEntityManager: public ThiEntityManager
    {
    public:
        ThEntityManager(ThiEngine* engine);
        virtual ThEntity CreateEntity()override;
        virtual bool DestroyEntity(const ThEntity& ent)override;
        virtual bool IsAlive(const ThEntity& ent)override;
        virtual void Reserve(ThI32 capacity)override;
        virtual ThI32 GetNumEntities()override;
        virtual ThEntity GetEntity(ThI32 index)override;
    private:
        ThVector<ThEntity> m_Entities;
        ThI32 m_FreeIndex;
        ThiEngine* m_Engine;
    };
}