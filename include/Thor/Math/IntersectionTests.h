#pragma once

#include <Thor/Math/Ray.h>
#include <Thor/Math/Sphere.h>

namespace Thor{	
	
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
        value_type b = 2.0 * ray.GetDirection() * delta;
        value_type c = delta * delta - sphere.GetRadius() * sphere.GetRadius();
        value_type d = b * b - 4.0 * a * c;
        return d >= 0.0;
    }

}//Thor