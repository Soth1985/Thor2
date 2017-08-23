#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VertexIn
{
	float2 position[[attribute(0)]];
	float4 color[[attribute(1)]];
};

struct VertexOut
{
    float4 position [[position]];
    float4 color;
};

// vertex shader function
vertex VertexOut vertexFunc(VertexIn input [[ stage_in ]])
{
    VertexOut out;
    out.position = float4(input.position, 0.0, 1.0);
    out.color = input.color;
    
    return out;
}

// fragment shader function
fragment float4 fragmentFunc(VertexOut input [[stage_in]])
{
    return input.color;
};

