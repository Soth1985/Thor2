#pragma once

#include <Thor/ThorMath/FixedVector.h>

namespace Thor{
	/*!
	 * \brief
	 * Ray class.
	 * 
	 * Represents a ray with an origin and unit direction.
	 * 
	 */
	template<class RealT>
	class ThRayT
	{
	public:
		/*!
		 * \brief
		 * Returns ray origin.
		 * 
		 * \returns
		 * Returns ray origin.
		 * 
		 */
		THOR_INLINE ThVec3& Origin() 
		{
			return origin;
		}
		/*!
		* \brief
		* Returns ray origin.
		* 
		* \returns
		* Returns ray origin.
		* 
		*/
		THOR_INLINE const ThVec3& Origin() const
		{
			return origin;
		}

		/*!
		 * \brief
		 * Sets a new ray direction.
		 * 
		 * \param d
		 * New ray direction.
		 * 
		 */
		THOR_INLINE void SetDirection(const ThVec3& d)
		{
			direction = d;
			direction.Normalize();
		}

		/*!
		 * \brief
		 * Returns ray direction.
		 * 
		 * \returns
		 * Returns ray direction.		
		 */
		THOR_INLINE const ThVec3& GetDirection() const
		{
			return direction;
		}

		/*!
		 * \brief
		 * Projects a point on the ray.
		 * 
		 * \param p
		 * Point to project.
		 * 
		 * \returns
		 * Point projection.
		 * 
		 * 
		 */
		ThVec3 ProjectPoint(const ThVec3& p) const
		{
			ThVec3 v = p - origin;
			RealT d = direction * v;
			return origin + d * direction;
		}

		ThVec3 Point(RealT t)const
		{
			return origin + t * direction;
		}

		ThVec3 operator()(RealT t)const
		{
			return origin + t * direction;
		}

		ThRayT(const ThVec3& origin_, const ThVec3& direction_)
			:origin(origin_),direction(direction_)
		{
			direction.Normalize();
		}
	private:
		/*!
		 * \brief
		 * Ray origin.
		 * 
		 */
		ThVec3 origin;
		/*!
		 * \brief
		 * Ray direction.
		 * 
		 */
		ThVec3 direction;
	};

	typedef ThRayT<ThF32> ThRay;
}//Thor