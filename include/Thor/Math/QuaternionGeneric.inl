/////////////////////////////////////////////////////

//product of 2 quaternions res=q1%q2
template< class T1, class AllocT1, class T2, class AllocT2, class ResT >
THOR_INLINE void VecModulus( const ThFixedVector<T1,4,quaternion_tag,AllocT1>& lhs, const ThFixedVector<T2,4,quaternion_tag,AllocT2>& rhs, ResT& res )
{	
	res.x() = lhs.w() * rhs.x() + lhs.x() * rhs.w() + lhs.z() * rhs.y() - lhs.y() * rhs.z();
	res.y() = lhs.w() * rhs.y() + lhs.y() * rhs.w() + lhs.x() * rhs.z() - lhs.z() * rhs.x();
	res.z() = lhs.w() * rhs.z() + lhs.z() * rhs.w() + lhs.y() * rhs.x() - lhs.x() * rhs.y();
	res.w() = lhs.w() * rhs.w() - lhs.x() * rhs.x() - lhs.y() * rhs.y() - lhs.z() * rhs.z();
};

//vec3=vec3^q
template< class T1, class VecTagT, class Alloc1T, class T2, class Alloc2T >
THOR_INLINE ThFixedVector<T1,3,VecTagT,Alloc1T>
operator^(const ThFixedVector<T1,3,VecTagT,Alloc1T> &v, const ThFixedVector<T2,4,quaternion_tag,Alloc2T>& q)
{
	ThFixedVector<T1,3,VecTagT,Alloc1T> tmp;
	//VecXor( v, q, tmp );
	QuatMulVec(v, q, tmp);
	return tmp;
};

//vec4=vec4^q
template< class T1, class VecTagT, class Alloc1T, class T2, class Alloc2T >
THOR_INLINE ThFixedVector<T1,4,VecTagT,Alloc1T>
operator^(const ThFixedVector<T1,4,VecTagT,Alloc1T> &v, const ThFixedVector<T2,4,quaternion_tag,Alloc2T>& q)
{
	ThFixedVector<T1, 4, VecTagT, Alloc1T> tmp;
	//VecXor( v, q, tmp );
	QuatMulVec(v, q, tmp);
	tmp.w() = v.w();
	return tmp;
};

//vec4=q^vec4
template< class T1, class VecTagT, class Alloc1T, class T2, class Alloc2T >
THOR_INLINE ThFixedVector<T1,4,VecTagT,Alloc1T>
operator^(const ThFixedVector<T1,4,quaternion_tag,Alloc2T>& q, const ThFixedVector<T2,4,VecTagT,Alloc1T> &v)
{
	ThFixedVector<T1, 4, VecTagT, Alloc1T> tmp;
	//VecXor( v, q, tmp );
	QuatMulVec(v, q, tmp);
	tmp.w() = v.w();
	return tmp;
};

//vec3=q^vec3
template< class T, class VecTagT, class Alloc1T, class Alloc2T >
THOR_INLINE ThFixedVector<T,3,VecTagT,Alloc1T>
operator^(const ThFixedVector<T,4,quaternion_tag,Alloc2T>& q, const ThFixedVector<T,3,VecTagT,Alloc1T> &v)
{	
	ThFixedVector<T,3,VecTagT,Alloc1T> tmp;
	//VecXor( v, q, tmp );
	QuatMulVec(v, q, tmp);
	return tmp;
};

template <class VecT, class QuatT, class ResT>
THOR_INLINE void QuatMulVec(const VecT& v, const QuatT& q, ResT& res)
{
	typedef typename VecT::value_type value_type;
	value_type	wx, wy, wz;
	value_type	xx, yy, yz;
	value_type	xy, xz, zz;
	value_type	x2, y2, z2;

	x2 = q.x() + q.x();
	y2 = q.y() + q.y();
	z2 = q.z() + q.z();

	xx = q.x() * x2;
	xy = q.x() * y2;
	xz = q.x() * z2;

	yy = q.y() * y2;
	yz = q.y() * z2;
	zz = q.z() * z2;

	wx = q.w() * x2;
	wy = q.w() * y2;
	wz = q.w() * z2;

	res.x() = (value_type(1.0) - yy - zz) * v.x()	+ (xy - wz) * v.y()	+ (xz + wy) * v.z();
	res.y() = (xy + wz) * v.x()	+ (value_type(1.0) - xx - zz) * v.y()	+ (yz - wx) * v.z();
	res.z() = (xz - wy) * v.x()	+ (yz + wx) * v.y()	+ (value_type(1.0) - xx - yy) * v.z();
}

//vec=vec*q
template< class T1, class TagT, class AllocT1, class T2, class AllocT2, class ResT >
THOR_INLINE void VecXor( const ThFixedVector<T1,4,TagT,AllocT1>& v, const ThFixedVector<T2,4,quaternion_tag,AllocT2>& q, ResT& res )
{	
	//res = q.ToMat4x4() * v;
};

template< class T1, class TagT, class AllocT1, class T2, class AllocT2, class ResT >
THOR_INLINE void VecXor( const ThFixedVector<T1,3,TagT,AllocT1>& v, const ThFixedVector<T2,4,quaternion_tag,AllocT2>& q, ResT& res )
{
	//res = q.ToMat3x3() * v;
}


template< class T, class AllocT, class RealT >
THOR_INLINE ThFixedVector<T,4,quaternion_tag,AllocT>
Slerp(const ThFixedVector<T,4,quaternion_tag,AllocT> &q0, const ThFixedVector<T,4,quaternion_tag,AllocT> &q1,const RealT t) 
{
	typedef typename ThFixedVector<T, 4, quaternion_tag,AllocT>::value_type value_type;
	// Check for out-of range parameter and return edge points if so
	if (t <= value_type(0.0) ) 
		return q0;

	if (t >= value_type(1.0) )
		return q1;
	// Compute "cosine of angle between Quaternions" using dot product
	
	value_type CosOmega = q0 * q1;
	// If negative dot, use –q1. Two Quaternionfs q and –q
	// represent the same rotation, but may produce
	// different slerp.Chose q or –q to rotate using
	// the acute angle.
	value_type q1w = q1.w();
	value_type q1x = q1.x();
	value_type q1y = q1.y();
	value_type q1z = q1.z();
	if (CosOmega < value_type(0.0) ) 
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		CosOmega = -CosOmega;
	}	
	
	value_type k0, k1;
	if (CosOmega > value_type(0.9999) )
	{
		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero
		k0 = value_type(1.0)-t;
		k1 = t;
	} 
	else 
	{
		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1
		float SinOmega = Math::Sqrt(value_type(1.0) - CosOmega * CosOmega);
		// Compute the angle from its sin and cosine
		float omega = Math::Atan2(SinOmega, CosOmega);
		// Compute inverse of denominator
		SinOmega = value_type(1.0) / SinOmega;
		// Compute interpolation parameters
		k0 = Math::Sin(( value_type(1.0) - t) * omega) * SinOmega;
		k1 = Math::Sin(t * omega) * SinOmega;
	}
	// Interpolate
	ThFixedVector<T, 4, quaternion_tag, AllocT> result;
	
	result.x() = k0 * q0.x() + k1 * q1x;
	result.y() = k0 * q0.y() + k1 * q1y;
	result.z() = k0 * q0.z() + k1 * q1z;
	result.w() = k0 * q0.w() + k1 * q1w;
	
	return result;
}

/////////////////////////////////////////////////////////////////////////////

template< class T, class AllocT, class RealT >
THOR_INLINE ThFixedVector<T,4,quaternion_tag,AllocT>
Pow(const ThFixedVector<T,4,quaternion_tag,AllocT> &q,const RealT exponent) 
{
	typedef typename ThFixedVector<T, 4, quaternion_tag, AllocT>::value_type value_type;
	// Check for the case of an identity Quaternion.
	// This will protect against divide by zero
	if (Math::Abs( q.w() ) > value_type(0.9999) ) 
	{
		return q;
	}
	// Extract the half angle alpha (alpha = theta/2)
	value_type alpha = Math::Acos( q.w() );
	
	value_type NewAlpha = alpha * exponent;	

	ThFixedVector<T, 4, quaternion_tag, AllocT> result;
	result.w() = Math::Cos(NewAlpha);
	
	value_type mult = Math::Sin(NewAlpha) / Math::Sin(alpha);
	result.x() = q.x() * mult;
	result.y() = q.y() * mult;
	result.z() = q.z() * mult;

	return result;
}