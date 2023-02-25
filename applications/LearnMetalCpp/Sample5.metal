#include <metal_stdlib>
#include "Shared5.h"

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

v2f vertex vertexMain5(device const VertexData* vertexData [[buffer(0)]],
                       device const InstanceData* instanceData [[buffer(1)]],
                       device const CameraData& cameraData [[buffer(2)]],
                       uint vertexId [[vertex_id]],
                       uint instanceId [[instance_id]] )
{
    v2f o;
    float4 pos = float4(vertexData->positions[vertexId], 1.0 );
    pos = instanceData[instanceId].instanceTransform * pos;
    pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
    o.position = pos;
    o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
    return o;
}

half4 fragment fragmentMain5( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}
