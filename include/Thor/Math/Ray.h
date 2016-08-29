#pragma once

#include <Thor/Math/FixedVector.h>

namespace Thor{
	/*!
	 * \brief
	 * Ray class.
	 * 
	 * Represents a ray with an origin and unit direction.
	 * 
	 */
	template<class Vec3>
	class ThRay
	{
	public:
        typedef typename Vec3::value_type value_type;
		/*!
		 * \brief
		 * Returns ray origin.
		 * 
		 * \returns
		 * Returns ray origin.
		 * 
		 */
		THOR_INLINE const Vec3& GetOrigin()const
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
		THOR_INLINE void SetOrigin(const Vec3& o)
		{
            origin = o;
		}

		/*!
		 * \brief
		 * Sets a new ray direction.
		 * 
		 * \param d
		 * New ray direction.
		 * 
		 */
		THOR_INLINE void SetDirection(const Vec3& d)
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
		THOR_INLINE const Vec3& GetDirection() const
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
		Vec3 ProjectPoint(const Vec3& p) const
		{
			Vec3 v = p - origin;
			value_type d = direction * v;
			return origin + d * direction;
		}

		Vec3 Point(value_type t)const
		{
			return origin + t * direction;
		}

		Vec3 operator()(value_type t)const
		{
			return origin + t * direction;
		}

		ThRay(const Vec3& origin_, const Vec3& direction_)
			:
        origin(origin_),
        direction(direction_)
		{
			//direction.Normalize();
		}
        
        ThRay()
        {
            
        }
	private:
		/*!
		 * \brief
		 * Ray origin.
		 * 
		 */
		Vec3 origin;
		/*!
		 * \brief
		 * Ray direction.
		 * 
		 */
		Vec3 direction;
	};

	typedef ThRay<ThVec3f> ThRayf;
}//Thor