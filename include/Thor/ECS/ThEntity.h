#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{

using ThEntityIndex = ThU32;
using ThEntityGeneration = ThU16;
using ThEntityId = ThU64;
using ThEntitySparseIndex = ThU16;
using ThEventId = ThI32;
using ThComponentId = ThI32;
using ThSystemId = ThI32;
constexpr ThU32 ThEntityNull {0xFFFFFFFF};
constexpr ThU16 ThEntitySparseNull {0xFFFF};

namespace Private
{
    constexpr ThI16 PageSize {8192};
    constexpr ThI8 PageAlignment {16};
    constexpr ThChar* LoggerTag {"ECS"};
    constexpr ThU64 EnityGenerationMask {0x000000000000FFFF};
    constexpr ThI32 EntityIndexBits {32};
    constexpr ThI32 EntityGenerationBits {16};
}

constexpr ThEntityIndex GetEntityIndex(ThEntityId entityId)
{
    return entityId >> Private::EntityIndexBits;
}

constexpr ThEntityGeneration GetEntityGeneration(ThEntityId entityId)
{
    return entityId & Private::EnityGenerationMask;
}

constexpr ThEntityId SetEntityIndex(ThEntityId entityId, ThEntityIndex index)
{
    ThEntityId result = index;
    result = (result << Private::EntityIndexBits) | (entityId & Private::EnityGenerationMask);
    return result;
}

constexpr ThEntityId SetEntityGeneration(ThEntityId entityId, ThEntityGeneration generation)
{
    ThEntityId result = (entityId & (~Private::EnityGenerationMask)) | generation;
    return result;
}

constexpr ThEntityId CreateEntityId(ThEntityIndex index, ThEntityGeneration generation)
{
    ThEntityId result = 0;
    result = SetEntityIndex(result, index);
    result = SetEntityGeneration(result, generation);
    return result;
}

}
