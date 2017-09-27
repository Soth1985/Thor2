#pragma once

#include <Thor/Engine/ThEngineTypeInfo.h>
#include <Thor/Engine/ThComponentTypes.h>
#include <Thor/Engine/ThResourceTypes.h>

namespace Thor
{
    class ThiSystem;
    class ThiComponentManager;
    class ThiResourceManager;
    class ThiBlackboardItem;
    class ThiWorld;
    class ThEvent;
    class ThEntity;
    
    typedef ThTypeID<ThiSystem> ThSystemType;
    typedef ThTypeID<ThiComponentManager> ThComponentType;
    typedef ThTypeID<ThiResourceManager> ThResourceType;
    typedef ThTypeID<ThiBlackboardItem> ThBlackboardType;
    typedef ThTypeID<ThEvent> ThEventType;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThComponentInstance = ThInstance<ThiComponentManager, TypeID, KindID>;
    
    template <ThU32 KindID>
    using ThComponentKey = ThKey<ThiComponentManager, KindID>;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThResourceInstance = ThInstance<ThiResourceManager, TypeID, KindID>;
    
    template <ThU32 KindID>
    using ThResourceKey = ThKey<ThiResourceManager, KindID>;
}
