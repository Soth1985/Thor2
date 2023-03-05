#pragma once

#include <Thor/SimdMath/Simd.h>

namespace Thor
{

struct ThGraphics
{
    static simd_float4x4 MakeIdentity();

    static simd::float4x4 MakePerspective( float fovRadians, float aspect, float znear, float zfar );

    static simd::float4x4 MakeXRotate( float angleRadians );

    static simd::float4x4 MakeYRotate( float angleRadians );

    static simd::float4x4 MakeZRotate( float angleRadians );

    static simd::float4x4 MakeTranslate( const simd::float3& v );

    static simd::float4x4 MakeScale( const simd::float3& v );

    static simd::float3x3 DiscardTranslation( const simd::float4x4& m );
};

}
