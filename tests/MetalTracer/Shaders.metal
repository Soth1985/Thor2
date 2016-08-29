#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VertexIn
{
	float2 position[[attribute(0)]];
	float2 uv[[attribute(1)]];
};

struct VertexOut
{
    float4 position [[position]];
    float2 uv;
};

// vertex shader function
vertex VertexOut vertexFunc(VertexIn input [[ stage_in ]])
{
    VertexOut out;
    out.position = float4(input.position, 0.0, 1.0);
    out.uv = input.uv;
    
    return out;
}

// fragment shader function
fragment float4 fragmentFunc(VertexOut input [[stage_in]], texture2d<float> tex[[texture(0)]])
{
    constexpr sampler sampler;
    float4 color = tex.sample(sampler, input.uv);
    return color;
};

