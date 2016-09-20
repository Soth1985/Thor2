#pragma once

#include <Thor/Engine/EngineTypeInfo.h>

namespace Thor
{
    class ThiSystem;
    class ThiComponent;
    class ThiResource;
    class ThEvent;
    typedef ThTypeID<ThiSystem> ThSystemType;
    typedef ThTypeID<ThiComponent> ThComponentType;
    typedef ThTypeID<ThiResource> ThResourceType;
    typedef ThTypeID<ThEvent> ThEventType;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThComponentKey = ThKey<ThiComponent, TypeID, KindID>;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThResourceKey = ThKey<ThiResource, TypeID, KindID>;
    
    template <ThU32 KindID>
    using ThComponentFKey = ThForeignKey<ThiComponent, KindID>;
    
    template <ThU32 TypeID, ThU32 KindID>
    using ThResourceFKey = ThForeignKey<ThiResource, KindID>;
}