#pragma once

#include <Thor/ThorMath/FixedVector.h>
#include <Thor/ThorMath/IBoundingVolume.h>

namespace Thor{
	
	/*!
	 * \brief
	 * Sphere class.
	 * 
	 * Represents a sphere with a center and a radius.
	 * 
	 */
	class Sphere:public IBoundingVolume
	{		
	public:
		/*!
		 * \brief
		 * Returns sphere radius.
		 * 
		 * \returns
		 * Returns sphere radius.
		 * 
		 */
		THOR_INLINE const ThReal32& Radius() const
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
		THOR_INLINE ThReal32& Radius()
		{
			return radius;
		}

		/*!
		* \brief
		* Returns sphere center.
		* 
		* \returns
		* Returns sphere center.
		* 
		*/
		THOR_INLINE const ThVec3& Center() const
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
		THOR_INLINE ThVec3& Center()
		{
			return center;
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
		ThVec3 ClosestPoint(const ThVec3& p) const
		{
			ThVec3 d = center - p;
			ThReal32 dl = d.Length();

			if(dl <= radius)
				return p;
			else
				return p + d * (dl - radius) / dl;
		}

		Sphere(const ThVec3& center_, ThReal32 radius_)
			: center(center_),radius(radius_)
		{}
	private:
		/*!
		 * \brief
		 * Sphere radius.
		 * 
		 */
		ThReal32 radius;
		/*!
		 * \brief
		 * Sphere center.
		 *		 
		 */
		ThVec3 center;
	};
}//Thor