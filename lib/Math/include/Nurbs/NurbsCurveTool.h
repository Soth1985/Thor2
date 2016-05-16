#ifndef THOR_NURBSCURVETOOL_H
#define THOR_NURBSCURVETOOL_H

#include <Thor/Core/Math/Nurbs/NurbsCurve.h>
#include <Thor/Core/Renderer/Geometry.h>
#include <Thor/Core/Math/Ray.h>
#include <vector>
#include <limits>

namespace Thor
{
	//////////////////////////////////////////////////////////////////////////
	//																		//
	//								NurbsCurveTool							//
	//																		//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	template <class Nreal, class Nint>
	class NurbsCurveTool
	{
	public:
		typedef NurbsCurve<Nreal,Nint> curve_t;
		typedef typename curve_t::control_poly_t control_poly_t;
		typedef typename curve_t::vector_t vector_t;
		typedef typename curve_t::real_t real_t;
		typedef typename curve_t::int_t int_t;
		typedef typename curve_t::Vector4 Vector4;
		typedef ThFixedVector< Nreal, 3 > Vector3;
		typedef ThRayT<Nreal> Ray;
		static const unsigned int genP=0x00000001;
		static const unsigned int gendPdu=0x00000002;
		//static const unsigned int genN=0x00000004;

		static void KnotInsert(Nreal u, Nint r, curve_t& curve);//insert knot u into curve knot vector
		static curve_t Split(Nreal u, curve_t& curve);//splits the curve at u left part goes to this - right to Curve
		static void KnotRefine( const vector_t& x, curve_t& curve);//curve knot refinement
		static std::vector<curve_t> DecomposeToBezier(const curve_t& curve);
		static Nint KnotRemove(Nreal u, Nint num, curve_t& curve, Nreal tol=0.001);
		static void DegreeElevate(Nint t, curve_t& curve);
		static bool DegreeReduce(curve_t& curve, Nreal tol=0.001);

		static vector_t MakeBezierKnotVec(Nint order);

		static curve_t MakeLine(const Vector3& P0, const Vector3& P1);
		static curve_t MakeCircle(Nreal radius, Nreal start, Nreal end);
		static curve_t MakeEllipse(Nreal a, Nreal b);
		static curve_t MakeOpenConic(const Vector3& P0, const Vector3& T0, const Vector3& P2, const Vector3& T2, const Vector3& P);

		static GeometryPtr MakeCurveGeometry(curve_t& curve, unsigned int step, const GpuDevicePtr& device, const EffectPtr& effect=EffectPtr(0), unsigned int bitfield= genP, const std::string& name=std::string() );//generate position, tangent vector(normalized), curve normal(unit)
		static GeometryPtr MakeControlPolygonGeometry(const control_poly_t& cp, const GpuDevicePtr& device, const EffectPtr& effect=EffectPtr(0), const std::string& name=std::string() );

	private:
		static void SplitArc(const Vector3& P0, const Vector3& P1, Nreal w1, const Vector3& P2, Vector3& Q1, Vector3& S, Vector3& R1, Nreal& wqr );
		static Nreal Angle(const Vector3& P, const Vector3& Q, const Vector3& R);
		static Nreal MakeConicArc(const Vector3& P0, const Vector3& T0, const Vector3& P2, const Vector3& T2, const Vector3& P , Vector3& P1);
		static Nreal ComputeTolerance(const curve_t& curve);
		static void BezDegreeReduce(const control_poly_t& bpts,control_poly_t& rbpts,Nreal& maxErr);
		//Nint IntersectLines(Vector4 &P1,Vector4 &P2,Vector4 &V1,Vector4 &V2,Nreal &t1,Nreal &t2);//intersects 2 lines returns 0 if no intersection	
		//Nint CurvIntLine(Vector4 &P1,Vector4 &V1,Nreal &u1,Nreal &u2,Nreal eps);//intersect 2 curves returns 1 if there is intersection otherwise 0

	};

#include <Thor/Core/Math/Nurbs/NurbsCurveTool.inl>

typedef NurbsCurveTool<float,int> NurbsCTool;

}//Thor

#endif