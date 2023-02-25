#pragma once

#ifdef __METAL_VERSION__
    using namespace metal;
#else
    #include <simd/simd.h>
    using namespace simd;
#endif

struct InstanceData
{
    float4x4 instanceTransform;
    float4 instanceColor;
};

struct CameraData
{
    float4x4 perspectiveTransform;
    float4x4 worldTransform;
};
