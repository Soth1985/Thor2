#pragma once

#include <Thor/Engine/EngineTypeInfo.h>

namespace Thor
{
    class ThiSystem;
    class ThiComponentManager;
    class ThiResourceManager;
    class ThiBlackboardItem;
    class ThEvent;
    class ThEntity;
    typedef ThTypeID<ThiSystem> ThSystemType;
    typedef ThTypeID<ThiComponentManager> ThComponentType;
    typedef ThTypeID<ThiResourceManager> ThResourceType;
    typedef ThTypeID<ThiBlackboardItem> ThBlackboardType;
    typedef ThTypeID<ThEvent> ThEventType;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThComponentKey = ThKey<ThiComponentManager, TypeID, KindID>;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThResourceKey = ThKey<ThiResourceManager, TypeID, KindID>;
    
    template <ThU32 KindID>
    using ThComponentFKey = ThForeignKey<ThiComponentManager, KindID>;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThResourceFKey = ThForeignKey<ThiResourceManager, KindID>;
}