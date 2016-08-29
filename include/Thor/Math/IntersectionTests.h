#pragma once

#include <Thor/Math/Ray.h>
#include <Thor/Math/Sphere.h>
#include <Thor/Math/ThorMath.h>

namespace Thor
{
    template<class Vec3>
    struct ThRayHit
    {
        ValueType<Vec3> t;
        Vec3 pos;
        Vec3 norm;
    };
    
    typedef ThRayHit<ThVec3f> ThRayHitf;
	
	template<class Vec3, class RealT>
	static bool IntersectRays(const ThRay<Vec3>& r1, const ThRay<Vec3>& r2, RealT& t1, RealT& t2)
	{
		static const RealT rayRayEpsilon = RealT(1e-4);

		Vec3 norm = r1.GetDirection() % r2.GetDirection();
		RealT dot = norm * norm;
		//parallel or coincident rays
		if( Math::Abs(dot) < rayRayEpsilon )
			return false;

		norm /= dot;
		Vec3 diff = r2.Origin() - r1.Origin();
		t1 = ( diff % r2.GetDirection() ) * norm;
		t2 = ( diff % r1.GetDirection() ) * norm;

		Vec3 deviation = r1.Point(t1) - r2.Point(t2);
		
		if( deviation * deviation < rayRayEpsilon )
			return true;
		else
			return false;
	}
    
    template<class Vec3>
    bool RayHitSphere(const ThRay<Vec3>& ray, const ThSphere<Vec3>& sphere)
    {
        //dot(p(t) - c, p(t) - c) = R * R; p(t) = o + t * d
        typedef ValueType<Vec3> value_type;
        Vec3 delta = ray.GetOrigin() - sphere.GetCenter();
        value_type a = ray.GetDirection() * ray.GetDirection();
        value_type b = ray.GetDirection() * delta;
        value_type c = delta * delta - sphere.GetRadius() * sphere.GetRadius();
        value_type d = b * b - a * c;
        return d >= 0.0;
    }
    
    template<class Vec3>
    bool RayIntersectSphere(const ThRay<Vec3>& ray, const ThSphere<Vec3>& sphere, ValueType<Vec3> tMin, ValueType<Vec3> tMax, ThRayHit<Vec3>& hit)
    {
        //dot(p(t) - c, p(t) - c) = R * R; p(t) = o + t * d
        typedef ValueType<Vec3> value_type;
        Vec3 delta = ray.GetOrigin() - sphere.GetCenter();
        value_type a = ray.GetDirection() * ray.GetDirection();
        value_type b = ray.GetDirection() * delta;
        value_type c = delta * delta - sphere.GetRadius() * sphere.GetRadius();
        value_type d = b * b - a * c;
        
        if (d >= 0.0f)
        {
            value_type sqrtD = Math::Sqrt(d);
            value_type oneOverA = 1.0 / a;
            value_type oneOverR = 1.0 / sphere.GetRadius();
            value_type t = (-b - sqrtD) * oneOverA;
            
            if (t > tMin && t < tMax)
            {
                hit.t = t;
                hit.pos = ray(t);
                hit.norm = (hit.pos - sphere.GetCenter()) * oneOverR;
                return true;
            }
            
            t = (-b + sqrtD) * oneOverA;
            
            if (t > tMin && t < tMax)
            {
                hit.t = t;
                hit.pos = ray(t);
                hit.norm = (hit.pos - sphere.GetCenter()) * oneOverR;
                return true;
            }
        }
        
        return false;
    }

}//Thor