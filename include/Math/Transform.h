#pragma once

#include <Thor/ThorMath/FixedVector.h>
#include <Thor/ThorMath/FixedMatrix.h>
#include <Thor/ThorMath/Quaternion.h>
#include <Thor/ThorMath/ThorMathExport.h>

namespace Thor{
	class THOR_MATH_DLL Transform
	{
	public:

		const ThVec3 GetUp()const
		{
			return rotate ^ yAxis;
		}

		const ThVec3 GetForward()const
		{
			return rotate ^ (-zAxis);
		}

		const ThVec3 GetRight()const
		{
			return rotate ^ xAxis;
		}		

		void Move(const ThVec3& direction, ThReal32 distance)
		{
			translate += direction * distance;
		}

		void Rotate(const ThQuat& q)
		{			
			rotate %= q;
		}

		ThQuat& Rotate()
		{
			return rotate;
		};

		const ThQuat& Rotate() const
		{
			return rotate;
		};

		ThVec3& Translate()
		{
			return translate;
		};

		const ThVec3& Translate() const
		{
			return translate;
		};

		ThVec3& Scale()
		{
			return scale;
		};

		const ThVec3& Scale() const
		{
			return scale;
		};

		ThMat4x4 ToMat4x4() const
		{			
			ThMat4x4 t;
			t.Translate(translate);
			
			return ThMat4x4( t * rotate.ToMat4x4()  );
		};

		void Join(const Transform& t)
		{
			translate += t.Translate();
			scale = ThVec3(scale.x() * t.Scale().x(), scale.y() * t.Scale().y(), scale.z() * t.Scale().z() );
			rotate %= t.Rotate();
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
			r(0,0) /= sx;	r(0,1) /= sx; r(0,2) /= sx;
			r(1,0) /= sy;	r(1,1) /= sy; r(1,2) /= sy;
			r(2,0) /= sz;	r(2,1) /= sz; r(2,2) /= sz;

			r(0,0) /= sx;	r(0,1) /= sy; r(0,2) /= sz;
			r(1,0) /= sx;	r(1,1) /= sy; r(1,2) /= sz;
			r(2,0) /= sx;	r(2,1) /= sy; r(2,2) /= sz;

			rotate = r.ToQuat();
			return true;
		}

		void operator=(const Transform& rhs)
		{
			rotate = rhs.rotate;
			translate = rhs.translate;
			scale = rhs.scale;
		}

		Transform()
			:scale(1.0f,1.0f,1.0f)
		{};
	private:
		ThQuat rotate;
		ThVec3 translate;
		ThVec3 scale;		
	};
}//Thor