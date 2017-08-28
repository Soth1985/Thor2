#pragma once

#include <simd/simd.h>

namespace Thor
{
    float Radians(const float& degrees)
    {
        static const float conv = M_PI / 180.0f;
        return conv * degrees;
    }

    simd::float4x4 Perspective(float fovy, float aspect, float near, float far )
    {
        float angle  = Radians(0.5f * fovy);
        float yScale = 1.0f / std::tan(angle);
        float xScale = yScale / aspect;
        float zScale = far / (far - near);
        
        simd::float4 P;
        simd::float4 Q;
        simd::float4 R;
        simd::float4 S;
        
        P.x = xScale;
        P.y = 0.0f;
        P.z = 0.0f;
        P.w = 0.0f;
        
        Q.x = 0.0f;
        Q.y = yScale;
        Q.z = 0.0f;
        Q.w = 0.0f;
        
        R.x = 0.0f;
        R.y = 0.0f;
        R.z = zScale;
        R.w = 1.0f;
        
        S.x =  0.0f;
        S.y =  0.0f;
        S.z = -near * zScale;
        S.w =  0.0f;
        
        return simd::float4x4(P, Q, R, S);
    };
    
    simd::float4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar )
    {
        float sLength = 1.0f / (right - left);
        float sHeight = 1.0f / (top   - bottom);
        float sDepth  = 1.0f / (far   - near);
        
        simd::float4 P;
        simd::float4 Q;
        simd::float4 R;
        simd::float4 S;
        
        P.x = 2.0f * sLength;
        P.y = 0.0f;
        P.z = 0.0f;
        P.w = 0.0f;
        
        Q.x = 0.0f;
        Q.y = 2.0f * sHeight;
        Q.z = 0.0f;
        Q.w = 0.0f;
        
        R.x = 0.0f;
        R.y = 0.0f;
        R.z = sDepth;
        R.w = 0.0f;
        
        S.x =  0.0f;
        S.y =  0.0f;
        S.z = -near  * sDepth;
        S.w =  1.0f;
        
        return simd::float4x4(P, Q, R, S);
    };
    
    simd::float4x4 LookAt(const simd::float3& eye, const simd::float3& center, const simd::float3& up)
    {
        simd::float3 zAxis = simd::normalize(center - eye);
        simd::float3 xAxis = simd::normalize(simd::cross(up, zAxis));
        simd::float3 yAxis = simd::cross(zAxis, xAxis);

        simd::float4 P;
        simd::float4 Q;
        simd::float4 R;
        simd::float4 S;

        P.x = xAxis.x;
        P.y = yAxis.x;
        P.z = zAxis.x;
        P.w = 0.0f;

        Q.x = xAxis.y;
        Q.y = yAxis.y;
        Q.z = zAxis.y;
        Q.w = 0.0f;

        R.x = xAxis.z;
        R.y = yAxis.z;
        R.z = zAxis.z;
        R.w = 0.0f;

        S.x = -simd::dot(xAxis, eye);
        S.y = -simd::dot(yAxis, eye);
        S.z = -simd::dot(zAxis, eye);
        S.w =  1.0f;

        return simd::float4x4(P, Q, R, S);
    }

    simd::float4x4 Scale(const simd::float3& s)
    {
        simd::float4 v = {s.x, s.y, s.z, 1.0f};
        return simd::float4x4(v);
    }

    simd::float4x4 Translate(const simd::float3& t)
    {
        simd::float4x4 M = matrix_identity_float4x4;
        M.columns[3].xyz = t;
        return M;
    }

    simd::float4x4 Rotate(const float& degrees, const simd::float3& axis)
    {
        float a = Radians(degrees);
        float c = 0.0f;
        float s = 0.0f;

        __sincospif(a, &s, &c);

        float k = 1.0f - c;

        simd::float3 u = simd::normalize(axis);
        simd::float3 v = s * u;
        simd::float3 w = k * u;

        simd::float4 P;
        simd::float4 Q;
        simd::float4 R;
        simd::float4 S;

        P.x = w.x * u.x + c;
        P.y = w.x * u.y + v.z;
        P.z = w.x * u.z - v.y;
        P.w = 0.0f;

        Q.x = w.x * u.y - v.z;
        Q.y = w.y * u.y + c;
        Q.z = w.y * u.z + v.x;
        Q.w = 0.0f;

        R.x = w.x * u.z + v.y;
        R.y = w.y * u.z - v.x;
        R.z = w.z * u.z + c;
        R.w = 0.0f;

        S.x = 0.0f;
        S.y = 0.0f;
        S.z = 0.0f;
        S.w = 1.0f;

        return simd::float4x4(P, Q, R, S);
    }
}