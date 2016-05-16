#pragma once

#include <Thor/ThorMath/FixedVector.h>
#include <Thor/ThorMath/IBoundingVolume.h>
#include <Thor/ThorMath/Transform.h>
#include <limits>
#undef max

namespace Thor{
	/*!
	 * \brief
	 * Axis Aligned box.
	 * 
	 * Represents an AABB with it`s maximum and minimum extents.
	 * 
	 */
	template<class RealT>
	class ThAxisAlignedBoxT:public IBoundingVolume
	{
	public:
		typedef ThFixedVector<RealT, 3> Vec3;
		typedef std::pair<Vec3,Vec3> pn_vert_t;
		/*!
		 * \brief
		 * Returns center of the box.
		 * 
		 */
		THOR_INLINE Vec3 GetCenter() const
		{
			return 0.5 * (pmin + pmax);
		};

		/*!
		* \brief
		* Returns circumscribed radius of the box.
		* 
		*/
		THOR_INLINE Vec3 GetRadius() const
		{
			return 0.5 * (pmax - pmin);
		}

		/*!
		 * \brief
		 * Extends the box if needed.
		 * 
		 * \param point
		 * The point this box must bound.
		 * 
		 */
		THOR_INLINE void Extend(const Vec3& point)
		{
			if (point.x() < pmin.x() ) 
				pmin.x() = point.x()

			if (point.x() > pmax.x() )
				pmax.x() = point.x();

			if (point.y() < pmin.y() ) 
				pmin.y() = point.y();

			if (point.y() > pmax.y() )
				pmax.y() = point.y();

			if (point.z() < pmin.z() )
				pmin.z() = point.z();

			if (point.z() > pmax.z() )
				pmax.z() = point.z();
		}

		/*
		Transforming Axis-Aligned Bounding Boxes
		by Jim Arvo
		from "Graphics Gems", Academic Press, 1990
		*/
		/*!
		 * \brief
		 * Transforms the original AABB and computes it`s axis aligned bounding box.
		 * 
		 * \param tr
		 * Box transform.
		 * 		 
		 * 
		 * Write detailed description for TransformBox here.
		 * 
		 */
		THOR_INLINE void TransformBox(const Transform& tr)
		{
			Vec3 bmin( tr.Translate() ), bmax(bmin);
			
			ThMat4x4& m = tr.ToMat4x4();
			
			RealT a,b;
			
			int i,j;
			
			/* Now find the extreme points by considering the product of the */
			/* min and max with each component of M.  */

			for( i = 0; i < 3; i++ )
			{
				for( j = 0; j < 3; j++ )
				{
					a = m(i,j) * pminOriginal(j);
					b = m(i,j) * pminOriginal(j);
					if( a < b ) 
					{ 
						bmin(i) += a; 
						bmax(i) += b;
					}
					else 
					{ 
						bmin(i) += b; 
						bmax(i) += a;
					}
				}
			}

			/* Copy the result into the new box. */
			pmin = bmin;
			pmax = bmax;
		}

		/*!
		 * \brief
		 * Sets minimum vertex.
		 * 
		 * \param p
		 * New minimum vertex.
		 * 
		 */
		THOR_INLINE void SetPmin(const Vec3& p)
		{
			pmin = p;
		}

		/*!
		 * \brief
		 * Returns box minimum vertex.
		 * 
		 */
		THOR_INLINE const Vec3& GetPmin() const
		{
			return pmin;
		}

		/*!
		* \brief
		* Sets maximum vertex.
		* 
		* \param p
		* New maximum vertex.
		* 
		*/
		THOR_INLINE void SetPmax(const Vec3& p)
		{
			pmax = p;
		}

		/*!
		* \brief
		* Returns box maximum vertex.
		* 
		*/
		THOR_INLINE const Vec3& GetPmax()const
		{
			return pmax;
		}		

		/*!
		 * \brief
		 * Returns box dimensions (width,height,depth).
		 * 
		 */
		THOR_INLINE Vec3 Size() const
		{
			return pmax - pmin;
		}

		Vec3 ClosestPoint(const Vec3& p)const
		{
			Vec3 r;

			for (int i = 0; i < 3; ++i)
			{
				if ( p(i) < pmin(i) )
				{
					r(i) = pmin(i);
				}
				else if ( p(i) > pmax(i) )
				{
					r(i) = pmax(i);
				}
				else
				{
					r(i) = p(i);
				}
			}

			return r;
		}

		/*!
		 * \brief
		 * Determine the nearest and the furthest diagonal vertices of the box along the normal direction.
		 * 
		 * \param normal
		 * Plane normal.
		 * 
		 * \returns
		 * pn_vert_t.first is the n vertex, pn_vert_t.second is p vertex.
		 * 
		 */
		pn_vert_t GetPNVertices(const Vec3& normal) const
		{
			Vec3 p(pmin);
			Vec3 n(pmax);

			for(int i = 0; i < 3; ++i)
			{
				if( normal(i) >= 0 )
				{
					p(i) = pmax(i);
					n(i) = pmin(i);
				}
			}

			return pn_vert_t(n,p);
		}

		/*!
		* \brief
		* Determine the furthest diagonal vertex of the box along the normal direction.
		* 
		* \param normal
		* Plane normal.
		* 
		* \returns
		* p vertex.
		* 
		*/
		Vec3 GetPVertex(const Vec3& normal) const
		{
			Vec3 p(pmin);			

			for(int i = 0; i < 3; ++i)
			{
				if( normal(i) >= 0 )
				{
					p(i) = pmax(i);
				}
			}

			return p;
		}

		/*!
		* \brief
		* Determine the nearest diagonal vertices of the box along the normal direction.
		* 
		* \param normal
		* Plane normal.
		* 
		* \returns
		* n vertex.
		* 
		*/
		Vec3 GetNVertex(const Vec3& normal) const
		{
			Vec3 n(pmax);

			for(int i = 0; i < 3; ++i)
			{
				if( normal(i) >= 0 )
				{					
					n(i) = pmin(i);
				}
			}

			return n;
		}

		ThAxisAlignedBoxT()
			//:pmax( -std::numeric_limits<Real>::max(), -std::numeric_limits<Real>::max(), -std::numeric_limits<Real>::max() ),
			//pmin( std::numeric_limits<Real>::max(), std::numeric_limits<Real>::max(), std::numeric_limits<Real>::max() )
		{
			RealT maxv = std::numeric_limits<RealT>::max();

			for(int i = 0; i < 3; ++i)
			{
				pmax(i) = -maxv;
				pmin(i) = maxv;
			}
		}

		ThAxisAlignedBoxT(const Vec3& pmin_, const Vec3& pmax_)
			:pmin(pmin_),pmax(pmax_)
		{};
	private:
		/*!
		 * \brief
		 * Actual box min vertex.
		 * 
		 */
		Vec3 pmin;
		/*!
		* \brief
		* Actual box max vertex.
		* 
		*/
		Vec3 pmax;
	};

	typedef ThAxisAlignedBoxT<ThF32> ThAxisAlignedBox;
}//Thor

#endif