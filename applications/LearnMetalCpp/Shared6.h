#pragma once

#ifdef __METAL_VERSION__
    using namespace metal;
#else
    #include <simd/simd.h>
    using namespace simd;
#endif

struct VertexData
{
    float3 position;
    float3 normal;
};

struct InstanceData
{
    float4x4 instanceTransform;
    float3x3 instanceNormalTransform;
    float4 instanceColor;
};

struct CameraData
{
    float4x4 perspectiveTransform;
    float4x4 worldTransform;
    float3x3 worldNormalTransform;
};
