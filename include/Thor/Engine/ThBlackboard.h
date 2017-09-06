#pragma once

#include <Thor/Engine/ThEngineInterface.h>
#include <Thor/Core/Containers/ThHashMap.h>

namespace Thor
{
    class ThBlackboard : public ThiBlackboard
    {
    public:
        ThBlackboard();
        virtual ThiBlackboardItem* GetItem(ThBlackboardType type)override;
        virtual ThiBlackboardItem* GetItem(ThI32 index)override;
        virtual ThI32 GetNumItems()override;
        virtual bool AddItem(ThiBlackboardItem* item)override;
        virtual bool RemoveItem(ThiBlackboardItem* item)override;
    private:
        ThHashMap<ThBlackboardType, ThiBlackboardItem*> m_Items;
    };
}
