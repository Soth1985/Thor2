#ifndef THOR_NURBSSURFACETOOL_H
#define THOR_NURBSSURFACETOOL_H

#include <Thor/Core/Math/Nurbs/NurbsSurface.h>
#include <Thor/Core/Math/Nurbs/NurbsCurveTool.h>
#include <Thor/Core/Renderer/Geometry.h>

namespace Thor
{
	//////////////////////////////////////////////////////////////////////////
	//																		//
	//								NurbsSurfaceTool						//
	//																		//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	template <class Nreal, class Nint>
	class NurbsSurfaceTool
	{
	public:
		
		typedef NurbsSurface<Nreal,Nint> surface_t;
		typedef NurbsCurve<Nreal,Nint> curve_t;
		typedef NurbsCurveTool<Nreal,Nint> curve_tool_t;
		typedef typename surface_t::vector_t vector_t;
		typedef typename surface_t::control_poly_t control_poly_t;
		typedef typename surface_t::real_t real_t;
		typedef typename surface_t::int_t int_t;
		typedef typename surface_t::Vector4 Vector4;
		typedef ThFixedVector< Nreal, 3 > Vector3;
		typedef ThRayT< Nreal > Ray;

		static const unsigned int genP		=0x00000001;
		static const unsigned int gendPdu	=0x00000002;
		static const unsigned int gendPdv	=0x00000004;
		static const unsigned int genN		=0x00000008;
		static const unsigned int genUV		=0x00000010;

		static void KnotInsertU(Nreal u, Nint r, surface_t& surface);//insert knot u into U knot vector 
		static void KnotInsertV(Nreal v, Nint r, surface_t& surface);//insert knot v into V knot vector 
		
		static surface_t SplitU(Nreal u, surface_t& surface);//splits surface at u using U knot vector
		static surface_t SplitV(Nreal v, surface_t& surface);//splits surface at v using V knot vector
		
		static void KnotRefineU( const vector_t& x, surface_t& surface);//surface knot refinement
		static void KnotRefineV( const vector_t& x, surface_t& surface);//surface knot refinement
		
		static std::vector<surface_t> 
			DecomposeToBezierStripsU(const surface_t& surface);
		static std::vector<surface_t> 
			DecomposeToBezierStripsV(const surface_t& surface);
		
		static Nint 
			KnotRemoveU(Nreal u, Nint num, surface_t& surface, Nreal tol=0.001);
		static Nint 
			KnotRemoveV(Nreal v, Nint num, surface_t& surface, Nreal tol=0.001);
		
		static void DegreeElevateU(Nint t, surface_t& surface);
		static void DegreeElevateV(Nint t, surface_t& surface);
		
		static bool DegreeReduceU(surface_t& surface, Nreal tol=0.001);
		static bool DegreeReduceV(surface_t& surface, Nreal tol=0.001);

		static surface_t 
			MakePlane( Nint spansU, Nint spansV, Nreal w, Nreal h, Nint uOrd=1, Nint vOrd=1 );// creates a nurbs plane

		static surface_t
			MakeBilinearSurface(const Vector4& p00, const Vector4& p10, const Vector4& p01, const Vector4& p11);

		static surface_t
			MakeGeneralCylinder(const curve_t& curve, const Vector3& direction, Nreal distance);

		static surface_t
			MakeCylinder(Nreal radius, Nreal start, Nreal end, Nreal height);

		static surface_t
			MakeRuledSurface(const curve_t& curve1, const curve_t& curve2);

		static surface_t
			MakeGeneralCone(const curve_t& base, const Vector3& apex);

		static surface_t
			MakeRevolvedSurface(const Ray& axis, Nreal theta, const curve_t& profile);

		static surface_t
			MakeBox(const Vector3& Pmin, const Vector3& Pmax);


		static GeometryPtr MakeSurfaceGeometry(surface_t& surface, unsigned int stepU, unsigned int stepV, const GpuDevicePtr& device, const EffectPtr& effect=EffectPtr(0), unsigned int bitfield= genP | genN | genUV, eGeometryTopology::Val topology=eGeometryTopology::Triangles, const std::string& name=std::string() );//generate position, tangent vector(normalized), curve normal(unit)
		static GeometryPtr MakeControlPolygonGeometry(const control_poly_t& cp, const GpuDevicePtr& device, const EffectPtr& effect=EffectPtr(0), eGeometryTopology::Val topology=eGeometryTopology::Lines, const std::string& name=std::string() );

	private:
		static Nreal ComputeTolerance(const surface_t& surface);
		static void BezDegreeReduceU(const control_poly_t& bpts,control_poly_t& rbpts,Nreal& maxErr,Nint row);
		static void BezDegreeReduceV(const control_poly_t& bpts,control_poly_t& rbpts,Nreal& maxErr,Nint row);


		//Nint IntersectLines(Vector4 &P1,Vector4 &P2,Vector4 &V1,Vector4 &V2,Nreal &t1,Nreal &t2);//intersects 2 lines returns 0 if no intersection
		//Nint CurvIntLine(Vector4 &P1,Vector4 &V1,Nreal &u1,Nreal &u2,Nreal eps);//intersect 2 curves returns 1 if there is intersection otherwise 0
	};

#include <Thor/Core/Math/Nurbs/NurbsSurfaceTool.inl>

typedef NurbsSurfaceTool<float,int> NurbsSTool;

};

#endif