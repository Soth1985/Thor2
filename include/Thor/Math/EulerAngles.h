#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Math/FixedVectorFwd.h>
#include <Thor/Math/FixedMatrixFwd.h>
#include <Thor/Math/EulerAnglesFwd.h>
#include <Thor/Math/ThorMath.h>

namespace Thor{

template <class RealT>
struct ThEulerAngles
{
	RealT heading, pitch, bank;//y-x-z

	ThEulerAngles():
	heading(RealT(0.0)), pitch(RealT(0.0)), bank(RealT(0.0))
	{};

	ThEulerAngles(RealT heading_, RealT pitch_, RealT bank_):
	heading(heading_), pitch(pitch_), bank(bank_)
	{};

	ThEulerAngles(const ThEulerAngles& copy):
	heading(copy.heading), pitch(copy.pitch), bank(copy.bank)
	{};

	THOR_INLINE void Canonize(void) 
	{
		// First, wrap pitch in range –pi ... pi
		pitch = WrapAngle(pitch);
		// Now, check for "the back side" of the matrix pitch outside
		// the canonical range of –pi/2 ... pi/2
		if (pitch < RealT(-90.0) ) 
		{
			pitch = RealT(-180.0) - pitch;
			heading += RealT(180.0);
			bank += RealT(180.0);
		} 
		else if (pitch > RealT(90.0) ) 
		{
			pitch = RealT(180.0) - pitch;
			heading += RealT(180.0);
			bank += RealT(180.0);
		}
		// Now check for the gimbal lock case (within a slight tolerance)
		if( Math::Abs(pitch) > ( RealT(90.0) - RealT(5.73e-03)  ) ) 
		{		
			heading += bank;
			bank = RealT(0.0);
		}
		else 
		{
			// Not in gimbal lock. Wrap the bank angle in
			// canonical range
			bank = WrapAngle(bank);
		}
		// Wrap heading in canonical range
		heading = WrapAngle(heading);
	}

	
	ThFixedVector<RealT,4,quaternion_tag>
	ToQuaternion() const
	{
		RealT sh,ch;
		RealT sp,cp;
		RealT sb,cb;
		RealT chsp, cpsb, cpcb, shsp;

		Math::SinCos( Math::DegToRad(heading) * RealT(0.5)	, sh, ch);
		Math::SinCos( Math::DegToRad(pitch)   * RealT(0.5)	, sp, cp);
		Math::SinCos( Math::DegToRad(bank)    * RealT(0.5)	, sb, cb);

		chsp = ch * sp;
		cpsb = cp * sb;
		cpcb = cp * cb;
		shsp = sh * sp;

		ThFixedVector<RealT,4,quaternion_tag> quat;
		quat.x = chsp * cb + sh * cpsb;
		quat.y = sh * cpcb - chsp * sb;
		quat.z = ch * cpsb - shsp * cb;
		quat.w = ch * cpcb + shsp * sb;

		return quat;
	}
	
	ThFixedMatrix<RealT,4,4,generic_mat_tag>
	ToMat4(void) const
	{
		ThFixedMatrix<RealT,4,4,generic_mat_tag> mat;
		//mal.LoadIdentity()
		MakeMatrix( mat );
		return mat;
		
	}

	ThFixedMatrix<RealT,3,3,generic_mat_tag>
	ToMat3(void) const
	{
		ThFixedMatrix<RealT,3,3,generic_mat_tag> mat;
		//mal.LoadIdentity()
		MakeMatrix( mat );
		return mat;
		
	}
private:
	template <class MatT>
	void MakeMatrix( MatT& m ) const
	{
		RealT sh,ch;
		RealT sp,cp;
		RealT sb,cb;
		RealT chcb,shsp;

		Math::SinCos( Math::DegToRad(heading), sh, ch);
		Math::SinCos( Math::DegToRad(pitch)	, sp, cp);
		Math::SinCos( Math::DegToRad(bank)	, sb, cb);

		chcb = ch * cb;
		shsp = sh * sp;

		m(0,0) =	chcb + shsp * sb;		m(0,1) = -ch * sb + shsp * cb;	m(0,2) = sh * cp;
		m(1,0) = sb * cp;				    m(1,1) = cb * cp;				m(1,2) = -sp;
        m(2,0) = -sh * cb + ch * sp * sb;	m(2,1) = sb * sh + chcb * sp;	m(2,2) = ch * cp;
	};
};
// "Wrap" an angle in range –pi...pi by adding the correct multiple
// of 2 pi
template <class RealT>
THOR_INLINE RealT WrapAngle(RealT angle)
{
	angle += RealT(180.0);
	angle -= Math::Floor(angle * RealT(90.0) ) * RealT(360.0);
	angle -= RealT(180.0);
	return angle;
}

//product of a vector and euler angles
template< class T1, unsigned int sz,class TagT, class T2, class ResT >
THOR_INLINE void VecMulAngles( const ThFixedVector<T1,sz,TagT>& v, const ThEulerAngles<T2>& ea, ResT& res )
{
	typedef typename ResT::value_type value_type;
	value_type sh,ch;
	value_type sp,cp;
	value_type sb,cb;
	value_type chcb,shsp;

	Math::SinCos( Math::DegToRad(ea.heading) , sh, ch);
	Math::SinCos( Math::DegToRad(ea.pitch)	, sp, cp);
	Math::SinCos( Math::DegToRad(ea.bank)	, sb, cb);

	chcb = ch * cb;
	shsp = sh * sp;

	res(0) = (chcb + shsp * sb) * v.x + (-ch * sb + shsp * cb) * v.y + (sh * cp) * v.z;
	res(1) = (sb * cp) * v.x + (cb * cp) * v.y + (-sp) * v.z;
	res(2) = (-sh * cb + ch * sp * sb) * v.x + (sb * sh + chcb * sp) * v.y + (ch * cp) * v.z;
}

//vec3=vec3*angles
template< class T1, class VecTagT, class T2 >
THOR_INLINE ThFixedVector<T1,3,VecTagT>
operator*(const ThFixedVector<T1,3,VecTagT> &v, const ThEulerAngles<T2>& a)
{
	ThFixedVector<T1,3,VecTagT> tmp;
	VecMulAngles( v,a,tmp );
	return tmp;
}

//vec4=vec4*angles
template< class T1, class VecTagT, class T2 >
THOR_INLINE ThFixedVector<T1,4,VecTagT>
operator*(const ThFixedVector<T1,4,VecTagT> &v, const ThEulerAngles<T2>& a)
{
	ThFixedVector<T1,4,VecTagT> tmp;
	VecMulAngles( v,a,tmp );
	tmp.w = v.w;
	return tmp;
}

//vec3=angles*vec3
template< class T1, class VecTagT, class T2 >
THOR_INLINE ThFixedVector<T1,3,VecTagT>
operator*(const ThEulerAngles<T2>& a, const ThFixedVector<T1,3,VecTagT> &v)
{
	ThFixedVector<T1,3,VecTagT> tmp;
	VecMulAngles( v,a,tmp );
	return tmp;
}

//vec4=angles*vec4
template< class T1, class VecTagT, class T2 >
THOR_INLINE ThFixedVector<T1,4,VecTagT>
operator*(const ThEulerAngles<T2>& a, const ThFixedVector<T1,4,VecTagT> &v)
{
	ThFixedVector<T1,4,VecTagT> tmp;
	VecMulAngles( v,a,tmp );
	tmp.w = v.w;
	return tmp;
}

}