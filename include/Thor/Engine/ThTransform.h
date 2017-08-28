#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Engine/ThSimdMatrixUtil.h>

namespace Thor
{
    class ThTransform
    {
    public:
        static const simd::float3 xAxis(1.0f, 0.0f, 0.0f);
        static const simd::float3 yAxis(0.0f, 1.0f, 0.0f);
        static const simd::float3 zAxis(0.0f, 0.0f, 1.0f);
        
        ThTransform()
            :
        m_Scale(1.0f,1.0f,1.0f)
        {}
        
        const simd::float3 GetUp()const
        {
            return m_Rotation(yAxis);
        }
        
        const simd::float3 GetForward()const
        {
            return m_Rotation(-zAxis);
        }
        
        const simd::float3 GetRight()const
        {
            return m_Rotation(xAxis);
        }
        
        void Move(const simd::float3& direction, ThF32 distance)
        {
            m_Translation += direction * distance;
        }
        
        void Rotate(const simd::quatf& q)
        {
            m_Rotation *= q;
        }
        
        simd::quatf& GetRotation()
        {
            return m_Rotation;
        };
        
        const simd::quatf& GetRotation() const
        {
            return m_Rotation;
        };
        
        simd::float3& GetTranslation()
        {
            return m_Translation;
        };
        
        const simd::float3& GetTranslation() const
        {
            return m_Translation;
        };
        
        simd::float3& GetScale()
        {
            return m_Scale;
        };
        
        const simd::float3& GetScale() const
        {
            return m_Scale;
        };
        
        simd::float4x4 ToMat4x4() const
        {
            simd::float4x4 translation;
            translation = Translate(m_Translation);
            simd::float4x4 rotation(m_Rotation);
            simd::float4x4 scale;
            scale = Scale(m_Scale);
            
            return translation * rotation * scale;
        };
        
        const ThTransform& operator()(const Transform& t)
        {
            m_Translation += t.m_Translation;
            m_Scale.x *= t.m_Scale.x;
            m_Scale.y *= t.m_Scale.y;
            m_Scale.z *= t.m_Scale.z;
            rotate *= t.m_Rotation;
            return *this;
        }
        
        bool FromMatrix(const ThMat4x4& m)
        {
            translate = ThVec3( m(0,3), m(1,3), m(2,3) );
            
            ThReal32 sx = Math::Sqrt( m(0,0) * m(0,0) + m(1,0) * m(1,0) + m(2,0) * m(2,0) );
            ThReal32 sy = Math::Sqrt( m(0,1) * m(0,1) + m(1,1) * m(1,1) + m(2,1) * m(2,1) );
            ThReal32 sz = Math::Sqrt( m(0,2) * m(0,2) + m(1,2) * m(1,2) + m(2,2) * m(2,2) );
            
            if( sx == ThReal32(0.0) || sy == ThReal32(0.0) || sz == ThReal32(0.0) )
                return false;
            
            scale = ThVec3(sx,sy,sz);
            
            ThMat3x3 r = m.ToMat3x3();
            r(0,0) /= sx;    r(0,1) /= sx; r(0,2) /= sx;
            r(1,0) /= sy;    r(1,1) /= sy; r(1,2) /= sy;
            r(2,0) /= sz;    r(2,1) /= sz; r(2,2) /= sz;
            
            r(0,0) /= sx;    r(0,1) /= sy; r(0,2) /= sz;
            r(1,0) /= sx;    r(1,1) /= sy; r(1,2) /= sz;
            r(2,0) /= sx;    r(2,1) /= sy; r(2,2) /= sz;
            
            rotate = r.ToQuat();
            return true;
        }
        
    private:
        simd::float4 m_Translation;
        simd::quatf m_Rotation;
        simd::float3 m_Scale;
    };
}
