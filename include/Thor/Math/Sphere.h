#pragma once

#include <Thor/Math/FixedVector.h>

namespace Thor{
	
	/*!
	 * \brief
	 * Sphere class.
	 * 
	 * Represents a sphere with a center and a radius.
	 * 
	 */
    template <class Vec3>
	class ThSphere
	{		
	public:
        typedef typename Vec3::value_type value_type;
		/*!
		 * \brief
		 * Returns sphere radius.
		 * 
		 * \returns
		 * Returns sphere radius.
		 * 
		 */
		THOR_INLINE value_type GetRadius() const
		{
			return radius;
		}

		/*!
		* \brief
		* Returns sphere radius.
		* 
		* \returns
		* Returns sphere radius.
		* 
		*/
		THOR_INLINE void SetRadius(value_type r)
		{
			return radius = r;
		}

		/*!
		* \brief
		* Returns sphere center.
		* 
		* \returns
		* Returns sphere center.
		* 
		*/
		THOR_INLINE const Vec3& GetCenter() const
		{
			return center;
		}

		/*!
		* \brief
		* Returns sphere center.
		* 
		* \returns
		* Returns sphere center.
		* 
		*/
		THOR_INLINE void SetCenter(const Vec3& c)
		{
            center = c;
		}

		/*!
		 * \brief
		 * Computes a closest point on a sphere.
		 * 
		 * \param p
		 * Point to compute the closest point to.
		 * 		 
		 * 
		 * If the point is inside the sphere, it is considered the closest.
		 * 
		 */
		Vec3 ClosestPoint(const Vec3& p) const
		{
			Vec3 d = center - p;
			value_type dl = d.Length();

			if(dl <= radius)
				return p;
			else
				return p + d * (dl - radius) / dl;
		}

		ThSphere(const Vec3& center_, value_type radius_)
			:
        center(center_),
        radius(radius_)
		{}
        
        ThSphere()
            :
        radius(value_type(0.0))
        {
            
        }
	private:

		value_type radius;
		Vec3 center;
	};
    
    typedef ThSphere<ThVec3f> ThSpheref;
}//Thor