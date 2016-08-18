#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Math/FixedVectorFwd.h>
#include <Thor/Math/FixedMatrixFwd.h>
#include <Thor/Math/EulerAnglesFwd.h>
#include <Thor/Math/ThorMath.h>

namespace Thor{

//////////////////////////
//Quaternion decl
template <
			class DataT
		 >
class ThFixedVecContainer<DataT,4,quaternion_tag>
{
	typedef ThFixedVector<DataT,4,quaternion_tag> quat_t;

	template <class MatT>
	void MakeMatrix( MatT& mat ) const
	{
		DataT	wx, wy, wz;
		DataT	xx, yy, yz;
		DataT	xy, xz, zz;
		DataT	x2, y2, z2;

		x2 = mx + mx;
		y2 = my + my;
		z2 = mz + mz;

		xx = mx * x2;
		xy = mx * y2;
		xz = mx * z2;

		yy = my * y2;
		yz = my * z2;
		zz = mz * z2;

		wx = mw * x2;
		wy = mw * y2;
		wz = mw * z2;

		mat(0,0) = DataT(1.0) - ( yy + zz );	mat(0,1) = xy - wz;						mat(0,2) = xz + wy;
		mat(1,0) = xy + wz;						mat(1,1) = DataT(1.0) - ( xx + zz );	mat(1,2) = yz - wx;
		mat(2,0) = xz - wy;						mat(2,1) = yz + wx;						mat(2,2) = DataT(1.0) - ( xx + yy );
	};
public:
	
	THOR_INLINE ThFixedVecContainer():mx( DataT(0.0) ),my( DataT(0.0) ),mz( DataT(0.0) ),mw( DataT(1.0) )
	{};

	THOR_INLINE ThFixedVecContainer( const ThFixedVecContainer& copy ):mx( copy.mx ),my( copy.my ),mz( copy.mz ),mw( copy.mw )
	{};

	THOR_INLINE ThFixedVecContainer( const DataT& x_, const DataT& y_, const DataT& z_, const DataT& w_ )
	{
		Rotate(w_, x_, y_, z_);
	};

	template< class T, class TagT >
	THOR_INLINE ThFixedVecContainer( const DataT& angle, const ThFixedVector<T,3,TagT>& vec )
	{
		Rotate( angle, vec );
	};
	
	THOR_INLINE  quat_t&
	Identity(void)
	{ 
		mw = DataT(1.0); 
		mx = my = mz = DataT(0.0); 
		return *(quat_t*)(this);
	}
	
	THOR_INLINE quat_t&
	RotateX(DataT theta) 
	{
		theta = Math::DegToRad(theta) * DataT(0.5);
		DataT s,c;
		Math::SinCos(theta,s,c);

		mw = c;
		mx = s;
		my = 0.0f;
		mz = 0.0f;

		return *(quat_t*)(this);
	};
	
	THOR_INLINE quat_t&
	RotateY(DataT theta) 
	{

		theta = Math::DegToRad(theta) * DataT(0.5);
		DataT s,c;
		Math::SinCos(theta,s,c);

		mw = c;
		mx = 0.0f;
		my = s;
		mz = 0.0f;

		return *(quat_t*)(this);
	};
	
	THOR_INLINE quat_t& 
	RotateZ(DataT theta) 
	{
		theta = Math::DegToRad(theta) * DataT(0.5);
		DataT s,c;
		Math::SinCos(theta,s,c);

		mw = c;
		mx = 0.0f;
		my = 0.0f;
		mz = s;

		return *(quat_t*)(this);
	};
	
	template < class VdT,class TagT >
	THOR_INLINE quat_t& 
	Rotate(DataT theta, ThFixedVector<VdT,3,TagT> axis)
	{
		axis.Normalize();
		theta = Math::DegToRad(theta) * DataT(0.5);
		DataT s,c;
		Math::SinCos(theta,s,c);		
		
		mw = c;
		mx = axis.x() * s;
		my = axis.y() * s;
		mz = axis.z() * s;

		return *(quat_t*)(this);
	};
	
	THOR_INLINE quat_t& 
	Rotate(DataT theta, DataT _x, DataT _y, DataT _z)
	{
		theta = Math::DegToRad(theta) * DataT(0.5);
		DataT s,c;
		Math::SinCos(theta,s,c);	

		ThFixedVector<DataT,3> axis(_x,_y,_z);
		axis.Normalize();
		
		mw = c;
		mx = axis.x() * s;
		my = axis.y() * s;
		mz = axis.z() * s;

		return *(quat_t*)(this);
	};

	THOR_INLINE DataT GetRotationAngle(void) const 
	{
		DataT thetaOver2 = Math::Acos(mw);
		// Return the rotation angle
		return Math::RadToDeg(thetaOver2 * DataT(2.0) );
	}
	
	THOR_INLINE ThFixedVector<DataT,3> GetRotationAxis(void) const 
	{
		ThFixedVector<DataT,3> tmp;

		DataT SinHalfTheta = DataT(1.0) - mw*mw;
		// Protect against numerical imprecision
		if (SinHalfTheta <= DataT(0.0) ) 
		{
			tmp = DataT(1.0), DataT(0.0), DataT(0.0);
			return tmp;
		}
		// Compute 1 / sin(theta/2)
		SinHalfTheta = DataT(1.0) / Math::Sqrt(SinHalfTheta);

		tmp(0) = mx * SinHalfTheta;
		tmp(1) = my * SinHalfTheta;
		tmp(2) = mz * SinHalfTheta;
		
		return tmp;
	}

	THOR_INLINE quat_t Conjugate() 
	{
		quat_t result;
		// Same rotation amount
		result.w() = mw;
		// Opposite axis of rotation
		result.x() = -mx;
		result.y() = -my;
		result.z() = -mz;

		return result;
	}

	ThFixedMatrix<DataT,3,3,generic_mat_tag> ToMat3x3() const
	{
		ThFixedMatrix<DataT,3,3,generic_mat_tag>	mat;
		MakeMatrix(mat);
		return mat;		
	}
	/////////////////////////////////////////////////////////////////////////////
	ThFixedMatrix<DataT,4,4,generic_mat_tag> ToMat4x4() const
	{
		ThFixedMatrix<DataT,4,4,generic_mat_tag>	mat;
		MakeMatrix(mat);
		return mat;		
	}

	ThEulerAngles<DataT> ToAngles() const
	{
		// Extract sin(pitch)
		DataT sp = DataT(-2.0) * (my*mz - mw*mx), heading, pitch, bank;
		// Check for Gimbal lock
		if (Math::Abs(sp) > DataT(0.9999) ) 
		{
			// Looking straight up or down
			pitch = Math::HalfPi * sp;	
			heading = Math::Atan2(-mx * mz + mw * my, DataT(0.5) - my * my - mz * mz);
			bank = 0.0f;
		} 
		else 
		{
			pitch	= Math::Asin(sp);
			heading = Math::Atan2(mx * mz + mw * my, DataT(0.5) - mx * mx - my * my);
			bank	= Math::Atan2(mx * my + mw * mz, DataT(0.5) - mx * mx - mz * mz);
		}

		return ThEulerAngles<DataT>(Math::RadToDeg(heading), Math::RadToDeg(pitch), Math::RadToDeg(bank));
	}
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedVecContainer tmp;
		
		tmp.Vec[0] = e.tree.at<DataT>(0);
		tmp.Vec[1] = e.tree.at<DataT>(1);
		tmp.Vec[2] = e.tree.at<DataT>(2);
		tmp.Vec[3] = e.tree.at<DataT>(3);

		Vec[0] = tmp.Vec[0];
		Vec[1] = tmp.Vec[1];
		Vec[2] = tmp.Vec[2];
		Vec[3] = tmp.Vec[3];
		
	};	
#endif
	THOR_INLINE DataT& x()
	{
		return mx;
	};

	THOR_INLINE const DataT& x() const
	{
		return mx;
	};

	THOR_INLINE DataT& y()
	{
		return my;
	};

	THOR_INLINE const DataT& y() const
	{
		return my;
	};

	THOR_INLINE DataT& z()
	{
		return mz;
	};

	THOR_INLINE const DataT& z() const
	{
		return mz;
	};

	THOR_INLINE DataT& w()
	{
		return mw;
	};

	THOR_INLINE const DataT& w() const
	{
		return mw;
	};

protected:
	union
	{
		DataT Vec[4];
	
		struct
		{
			DataT mx,my,mz,mw;
		};
	};
};


#include <Thor/Math/QuaternionGeneric.inl>

};//Thor