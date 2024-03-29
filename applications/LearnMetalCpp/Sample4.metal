#include <metal_stdlib>
#include "Shared4.h"

using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

struct VertexData
{
    device float3* positions [[id(0)]];
};

v2f vertex vertexMain4( device const VertexData* vertexData [[buffer(0)]],
                       device const InstanceData* instanceData [[buffer(1)]],
                       uint vertexId [[vertex_id]],
                       uint instanceId [[instance_id]] )
{
    v2f o;
    float4 pos = float4( vertexData->positions[ vertexId ], 1.0 );
    o.position = instanceData[ instanceId ].instanceTransform * pos;
    o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
    return o;
}

half4 fragment fragmentMain4( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}
