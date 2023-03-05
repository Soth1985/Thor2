#include "stdafx.h"
#include <Thor/SimdMath/ThGraphics.h>

using namespace Thor;

simd_float4x4 ThGraphics::MakeIdentity()
{
    using simd::float4;
    return (simd_float4x4){ (float4){ 1.f, 0.f, 0.f, 0.f },
                            (float4){ 0.f, 1.f, 0.f, 0.f },
                            (float4){ 0.f, 0.f, 1.f, 0.f },
                            (float4){ 0.f, 0.f, 0.f, 1.f } };
}

simd::float4x4 ThGraphics::MakePerspective( float fovRadians, float aspect, float znear, float zfar )
{
    using simd::float4;
    float ys = 1.f / tanf(fovRadians * 0.5f);
    float xs = ys / aspect;
    float zs = zfar / ( znear - zfar );
    return simd_matrix_from_rows((float4){ xs, 0.0f, 0.0f, 0.0f },
                                 (float4){ 0.0f, ys, 0.0f, 0.0f },
                                 (float4){ 0.0f, 0.0f, zs, znear * zs },
                                 (float4){ 0, 0, -1, 0 });
    
    /*float sy = 1 / tan(fovRadians * 0.5);
    float sx = sy / aspect;
    float zRange = zfar - znear;
    float sz = -(zfar + znear) / zRange;
    float tz = -2 * zfar * znear / zRange;
    
    return simd_matrix_from_rows
    (
        (float4){sx, 0.0, 0.0, 0.0},
        (float4){0, sy,  0,  0},
        (float4){0,  0, sz, -1},
        (float4){0,  0, tz,  0}
    );*/
}

simd::float4x4 ThGraphics::MakeXRotate( float angleRadians )
{
    using simd::float4;
    const float a = angleRadians;
    return simd_matrix_from_rows((float4){ 1.0f, 0.0f, 0.0f, 0.0f },
                                 (float4){ 0.0f, cosf( a ), sinf( a ), 0.0f },
                                 (float4){ 0.0f, -sinf( a ), cosf( a ), 0.0f },
                                 (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd::float4x4 ThGraphics::MakeYRotate( float angleRadians )
{
    using simd::float4;
    const float a = angleRadians;
    return simd_matrix_from_rows((float4){ cosf( a ), 0.0f, sinf( a ), 0.0f },
                                 (float4){ 0.0f, 1.0f, 0.0f, 0.0f },
                                 (float4){ -sinf( a ), 0.0f, cosf( a ), 0.0f },
                                 (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd::float4x4 ThGraphics::MakeZRotate( float angleRadians )
{
    using simd::float4;
    const float a = angleRadians;
    return simd_matrix_from_rows((float4){ cosf( a ), sinf( a ), 0.0f, 0.0f },
                                 (float4){ -sinf( a ), cosf( a ), 0.0f, 0.0f },
                                 (float4){ 0.0f, 0.0f, 1.0f, 0.0f },
                                 (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd::float4x4 ThGraphics::MakeTranslate( const simd::float3& v )
{
    using simd::float4;
    const float4 col0 = { 1.0f, 0.0f, 0.0f, 0.0f };
    const float4 col1 = { 0.0f, 1.0f, 0.0f, 0.0f };
    const float4 col2 = { 0.0f, 0.0f, 1.0f, 0.0f };
    const float4 col3 = { v.x, v.y, v.z, 1.0f };
    return simd_matrix( col0, col1, col2, col3 );
}

simd::float4x4 ThGraphics::MakeScale( const simd::float3& v )
{
    using simd::float4;
    return simd_matrix((float4){ v.x, 0, 0, 0 },
                       (float4){ 0, v.y, 0, 0 },
                       (float4){ 0, 0, v.z, 0 },
                       (float4){ 0, 0, 0, 1.0 });
}

simd::float3x3 ThGraphics::DiscardTranslation( const simd::float4x4& m )
{
    return simd_matrix( m.columns[0].xyz, m.columns[1].xyz, m.columns[2].xyz );
}
