#ifndef THOR_NURBSCURVE_H
#define THOR_NURBSCURVE_H

#include <string.h>
#include <Thor/Utility/DynArray.h>
#include <Thor/Core/Math/Math.h>
#include <Thor/Core/Math/Nurbs/NurbsExceptions.h>
#include <Thor/Common.h>

namespace Thor{

	template <class Nreal, class Nint>
	class NurbsCurveTool;

	template <class Nreal, class Nint>
	class NurbsSurfaceTool;
	//////////////////////////////////////////////////////////////////////////
	//																		//
	//								Control Polygon							//
	//																		//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	template <class Nreal, class Nint>
	class ControlPolygon
	{
	protected:
		typedef Nreal real_t;
		typedef Nint int_t;
		typedef ThFixedVector< Nreal, 4 > Vector4;
		typedef ThFixedMatrix<Nreal, 4, 4 > Matrix4x4;
		DynArray< Vector4, Shape1d > Vertices;
	public:	

		inline unsigned int c() const
		{
			return Vertices.c();
		};
		inline ControlPolygon& operator=(const ControlPolygon &V)
		{
			Vertices=V.Vertices;
			return *this;
		}
		inline void Swap(ControlPolygon &V)
		{
			Vertices.Swap(V.Vertices);
		}

		inline unsigned int Size() const
		{
			return Vertices.Size();
		};

		/*inline void Copy( const ControlPolygon &V, int num)
		{
		Vertices.Copy( V.Vertices, num);
		};*/

		inline void Realloc( int i)
		{
			Vertices.Realloc( i );
		};

		inline Vector4& operator()(Nint i)
		{
			return Vertices(i);
		}

		inline const Vector4& operator()(Nint i) const
		{
			return Vertices(i);
		}

		void TransformCP(const Matrix4x4& mat)
		{
			for(unsigned int i=0; i<Vertices.Size(); ++i)
				Vertices(i)=mat*Vertices(i);
		};

		ControlPolygon()
		{};
		ControlPolygon(int c)
			: Vertices(c)
		{};
		ControlPolygon( const ControlPolygon& copy) : Vertices( copy.Vertices ) {};

	};
	//////////////////////////////////////////////////////////////////////////
	//																		//
	//								NurbsCurve								//
	//																		//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	template <class Nreal,class Nint> 
	class NurbsCurve
	{
	public:
		typedef Nreal real_t;
		typedef Nint int_t;
		typedef ThFixedVector< Nreal, 4 > Vector4;
		typedef ThFixedMatrix<Nreal, 4, 4 > Matrix4x4;
		typedef DynArray< Nreal, Shape1d > vector_t;
		typedef DynArray< Vector4, Shape1d > vector4_t;
		typedef DynArray< Nreal > array_t;
		typedef ControlPolygon< Nreal,Nint > control_poly_t;

	protected:

		vector_t	U;		//knot vector for u direction	dim=n+Uorder+1		
		control_poly_t	P;		//dim=m x n
		Nint			Uorder;	//surface order in u direction		
		Nint			DUorder;//maximum derivative order in u direction

		Nint			n;	//number of control points in u-v direction counted from zero
		array_t	Nu;		//computed span basis functions and their derivs up to DUorder in u direction	dim= DUorder+1 x Uorder+1
		vector_t  Nipu;	//computed single bf in u direction and its derivatives dim=DUorder+1
		vector_t	left,right; //knot differences	dim=(Uorder)+1
		//DynArray< Nreal >	ders;	//basis function derivatives where 1-st dimension is derivative order and second is function index
		array_t	ndu;	// upper triangle stores basis funcs, lower stores knot differences	dim=max(Uorder,Vorder)+1 x max(Uorder,Vorder)+1 
		array_t  a;		//stores coefficients to compute derivatives dim=2 x (Uorder)+1
		vector_t  ND;		//used in DersOneBasisFunc dim=(DUorder)
		vector4_t Aw;//used to calculate curve and surface derivatives dim=DUorder+1 

		vector4_t SKL;//stores computed point and its derivatives dim=DUorder+1 x DVorder+1 (3d point)


		Nint FindSpan(Nreal u);//returns knot span index in u direction
		Nint FindSpanMult(Nreal u,Nint &s);//returns knot span index and multiplicity in u direction
		void BasisFunc(Nint i,Nreal u);//compute nonzero basis funcs in u direction
		void DersBasisFunc(Nint i,Nreal u);//compute nonzero basis funcs and their derivatives ordered from 0 to DUorder in u direction
		void OneBasisFunc(Nint i,Nreal u);// computes single basis function in u direction
		void DersOneBasisFunc(Nint i,Nreal u);//computes single basis function and it`s derivatives ordered from 0 to DUorder in u direction

		void Refresh(	Nint uOrder, Nint duOrder );
	public:

		friend class NurbsCurveTool<Nreal,Nint>;
		friend class NurbsSurfaceTool<Nreal,Nint>;


		const Vector4& Result(unsigned int du=0) const
		{
			return SKL(du);
		}


		NurbsCurve(	const control_poly_t& cp, const vector_t& uVec,
			Nint uOrder, Nint duOrder );

		NurbsCurve( const NurbsCurve& copy ) : U( copy.U), P( copy.P ), Uorder( copy.Uorder ),	
			DUorder(copy.DUorder), n(copy.n),
			Nu(copy.Nu), Nipu(copy.Nipu), left(copy.left),
			right(copy.right), ndu(copy.ndu), a(copy.a), ND(copy.ND), Aw(copy.Aw), SKL(copy.SKL)
		{};
		~NurbsCurve();
		Nreal Umin(void)const;//returns min value of U knot vector
		Nreal Umax(void)const;//returns max value of U knot vector

		void SetDUorder(Nint iDUorder)
		{
			Refresh( Uorder,iDUorder );
		}

		const control_poly_t& GetControlPoly()const //gets control polygon
		{
			return P;
		}
		const vector_t& GetKnotVec()const//gets  u knot vector
		{
			return U;
		}
		Nint GetDUorder(void)const//gets differentiation order in u direction
		{
			return DUorder;
		}
		Nint GetUorder(void)const//gets order in u direction
		{
			return Uorder;
		}

		void TransformCP(const Matrix4x4& mat)
		{
			P.TransformCP(mat);
		};	

		void Point(Nreal u);//compute curve point on a rational b-spline
		void PointDerivs(Nreal u);//compute curve point with its derivatives

		inline Vector4 &operator()(Nint i)
		{
			return P(i);
		};

		Nint GetN()const
		{
			return n;
		}

		inline const Vector4 &operator()(Nint i)const
		{
			return P(i);
		};

		void Reparametrize(Nreal c, Nreal d);
		//w0=wn
		void RationalReparametrize(Nreal c, Nreal d);
		void Reverse();


		inline NurbsCurve& operator=( const NurbsCurve& rhs )
		{
			U		=	rhs.U;			
			P		=	rhs.P;
			Uorder	=	rhs.Uorder;			
			DUorder	=	rhs.DUorder;			
			n		=	rhs.n;			
			Nu		=	rhs.Nu;			
			Nipu	=	rhs.Nipu;			
			left	=	rhs.left;
			right	=	rhs.right;
			ndu		=	rhs.ndu;
			a		=	rhs.a;
			ND		=	rhs.ND;
			Aw		=	rhs.Aw;			
			SKL		=	rhs.SKL;

			return (*this);
		};
	};


#include <Thor/Core/Math/Nurbs/NurbsCurve.inl>

typedef NurbsCurve<float,int> NurbsC;

}//Thor

#endif