#pragma once

#include <Thor/ThorMath/FixedVector.h>

namespace Thor{
	/*!
	 * \brief
	 * Plane class.
	 * 
	 * Represents the plane with a point and a unit normal.
	 * 
	 */
	template<class RealT>
	class ThPlaneT
	{
	public:
		typedef ThFixedVector<RealT, 3> Vec3;

		/*!
		 * \brief
		 * Returns plane point.
		 * 
		 * \returns
		 * Returns plane point.
		 *		 
		 */
		THOR_INLINE Vec3& Point() 
		{
			return point;
		}

		/*!
		* \brief
		* Returns plane point.
		* 
		* \returns
		* Returns plane point.
		*		 
		*/
		THOR_INLINE const Vec3& Point() const
		{
			return point;
		}
		
		/*!
		 * \brief
		 * Sets a new plane normal.
		 * 
		 * \param n
		 * New normal vector, normalized automatically.
		 * 		
		 */
		THOR_INLINE void SetNormal(const Vec3& n)
		{
			normal = n;
			normal.Normalize();
		}

		/*!
		 * \brief
		 * Returns plane normal.
		 * 
		 * \returns
		 * Returns plane normal.
		 * 		 
		 */
		THOR_INLINE const Vec3& GetNormal() const
		{
			return normal;
		}

		THOR_INLINE void SetPlane(const Vec3& p, const Vec3& n)
		{
			point = p;
			normal = n;
			normal.Normalize();
		}

		THOR_INLINE void SetPlane(const Vec3& p1, const Vec3& p2, const Vec3& p3)
		{
			Vec3 e1 = p2 - p1;
			Vec3 e2 = p3 - p1;
			point = p1;
			normal = e1 % e2;
			normal.Normalize();
		}

		/*!
		 * \brief
		 * Computes distance from a point to the plane.
		 * 
		 * \param p
		 * Compute distance from the plane to this point.
		 * 
		 * \returns
		 * Signed distance between plane and point.		 
		 * 
		 * Returned value can be negative if the point and the normal lie on the opposite side of the plane.
		 * 
		 */
		RealT Distance(const Vec3& p) const
		{
			Vec3 v = p - point;
			return normal * v;
		}

		/*!
		 * \brief
		 * Projects a point on the plane.
		 * 
		 * \param p
		 * Point to project.
		 * 
		 * \returns
		 * Projected point.
		 * 
		 */
		Vec3 ProjectPoint(const Vec3& p) const
		{
			Vec3 v = p - point;
			RealT d = normal * v;
			return p - d * normal;
		}

		ThPlaneT(const Vec3& point_, const Vec3& normal_)
			:point(point_),normal(normal_)
		{
			normal.Normalize();
		}

		ThPlaneT()
			:point(0.0,0.0,0.0),normal(0.0,1.0,0.0)
		{}
	private:
		/*!
		 * \brief
		 * Plane point.
		 * 
		 */
		Vec3 point;
		/*!
		 * \brief
		 * Plane unit normal.
		 * 
		 */
		Vec3 normal;
	};

	typedef ThPlaneT<ThF32> ThPlane;
}//Thor