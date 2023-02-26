#include <metal_stdlib>
#include "Shared7.h"
using namespace metal;

struct v2f
{
    float4 position [[position]];
    float3 normal;
    half3 color;
    float2 texcoord;
};

v2f vertex vertexMain8( device const VertexData* vertexData [[buffer(0)]],
                       device const InstanceData* instanceData [[buffer(1)]],
                       device const CameraData& cameraData [[buffer(2)]],
                       uint vertexId [[vertex_id]],
                       uint instanceId [[instance_id]] )
{
    v2f o;

    const device VertexData& vd = vertexData[ vertexId ];
    float4 pos = float4( vd.position, 1.0 );
    pos = instanceData[ instanceId ].instanceTransform * pos;
    pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
    o.position = pos;

    float3 normal = instanceData[ instanceId ].instanceNormalTransform * vd.normal;
    normal = cameraData.worldNormalTransform * normal;
    o.normal = normal;

    o.texcoord = vd.texcoord.xy;

    o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
    return o;
}

half4 fragment fragmentMain8( v2f in [[stage_in]], texture2d< half, access::sample > tex [[texture(0)]] )
{
    constexpr sampler s( address::repeat, filter::linear );
    half3 texel = tex.sample( s, in.texcoord ).rgb;

    // assume light coming from (front-top-right)
    float3 l = normalize(float3( 1.0, 1.0, 0.8 ));
    float3 n = normalize( in.normal );

    half ndotl = half( saturate( dot( n, l ) ) );

    half3 illum = (in.color * texel * 0.1) + (in.color * texel * ndotl);
    return half4( illum, 1.0 );
}

 kernel void mandelbrot_set(
                            texture2d< half, access::write > tex [[texture(0)]],
                            uint2 index [[thread_position_in_grid]],
                            uint2 gridSize [[threads_per_grid]]
                            )
{
    // Scale
    float x0 = 2.0 * index.x / gridSize.x - 1.5;
    float y0 = 2.0 * index.y / gridSize.y - 1.0;

    // Implement Mandelbrot set
    float x = 0.0;
    float y = 0.0;
    uint iteration = 0;
    uint max_iteration = 1000;
    float xtmp = 0.0;
    while(x * x + y * y <= 4 && iteration < max_iteration)
    {
        xtmp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtmp;
        iteration += 1;
    }

    // Convert iteration result to colors
    half color = (0.5 + 0.5 * cos(3.0 + iteration * 0.15));
    tex.write(half4(color, color, color, 1.0), index, 0);
}

kernel void mandelbrot_set_animated(
                                    texture2d< half, access::write > tex [[texture(0)]],
                                   uint2 index [[thread_position_in_grid]],
                                   uint2 gridSize [[threads_per_grid]],
                                   device const uint* frame [[buffer(0)]]
                                    )
{
    constexpr float kAnimationFrequency = 0.01;
    constexpr float kAnimationSpeed = 4;
    constexpr float kAnimationScaleLow = 0.62;
    constexpr float kAnimationScale = 0.38;

    constexpr float2 kMandelbrotPixelOffset = {-0.2, -0.35};
    constexpr float2 kMandelbrotOrigin = {-1.2, -0.32};
    constexpr float2 kMandelbrotScale = {2.2, 2.0};

    // Map time to zoom value in [kAnimationScaleLow, 1]
    float zoom = kAnimationScaleLow + kAnimationScale * cos(kAnimationFrequency * *frame);
    // Speed up zooming
    zoom = pow(zoom, kAnimationSpeed);

    //Scale
    float x0 = zoom * kMandelbrotScale.x * ((float)index.x / gridSize.x + kMandelbrotPixelOffset.x) + kMandelbrotOrigin.x;
    float y0 = zoom * kMandelbrotScale.y * ((float)index.y / gridSize.y + kMandelbrotPixelOffset.y) + kMandelbrotOrigin.y;

    // Implement Mandelbrot set
    float x = 0.0;
    float y = 0.0;
    uint iteration = 0;
    uint max_iteration = 1000;
    float xtmp = 0.0;
    while(x * x + y * y <= 4 && iteration < max_iteration)
    {
        xtmp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtmp;
        iteration += 1;
    }

    // Convert iteration result to colors
    half color = (0.5 + 0.5 * cos(3.0 + iteration * 0.15));
    tex.write(half4(color, color, color, 1.0), index, 0);
}
