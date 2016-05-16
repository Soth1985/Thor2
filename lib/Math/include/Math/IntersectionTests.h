#pragma once

#include <Thor/ThorMath/Ray.h>

namespace Thor{	
	
	template<class RealT>
	static bool IntersectRays(const ThRayT<RealT>& r1, const ThRayT<RealT>& r2, RealT& t1, RealT& t2)
	{
		static const RealT rayRayEpsilon = RealT(1e-4);

		ThVec3 norm = r1.GetDirection() % r2.GetDirection();
		RealT dot = norm * norm;
		//parallel or coincident rays
		if( Math::Abs(dot) < rayRayEpsilon )
			return false;

		norm /= dot;
		ThVec3 diff = r2.Origin() - r1.Origin();
		t1 = ( diff % r2.GetDirection() ) * norm;
		t2 = ( diff % r1.GetDirection() ) * norm;

		ThVec3 deviation = r1.Point(t1) - r2.Point(t2);
		
		if( deviation * deviation < rayRayEpsilon )
			return true;
		else
			return false;
	}

}//Thor