#pragma once

#include <simd/simd.h>

struct MetalMath
{
    static simd::float3 add( const simd::float3& a, const simd::float3& b );

    static simd_float4x4 makeIdentity();

    static simd::float4x4 makePerspective( float fovRadians, float aspect, float znear, float zfar );

    static simd::float4x4 makeXRotate( float angleRadians );

    static simd::float4x4 makeYRotate( float angleRadians );

    static simd::float4x4 makeZRotate( float angleRadians );

    static simd::float4x4 makeTranslate( const simd::float3& v );

    static simd::float4x4 makeScale( const simd::float3& v );

    static simd::float3x3 discardTranslation( const simd::float4x4& m );

};
