#ifndef THOR_NURBSSURFACE_H
#define THOR_NURBSSURFACE_H

#include <string.h>
#include <Thor/Utility/DynArray.h>
#include <Thor/Core/Math/Math.h>
#include <Thor/Core/Math/Nurbs/NurbsExceptions.h>
#include <Thor/Common.h>

namespace Thor
{
	template <class Nreal, class Nint>
	class NurbsSurfaceTool;
	//////////////////////////////////////////////////////////////////////////
	//																		//
	//								Control Mesh							//
	//																		//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	template <class Nreal, class Nint>
	class ControlMesh
	{
	protected:
		typedef Nreal real_t;
		typedef Nint int_t;
		typedef ThFixedVector< Nreal, 4 > Vector4;
		typedef ThFixedMatrix<Nreal, 4, 4 > Matrix4x4;
		DynArray< Vector4 > Vertices;
	public:


		inline unsigned int r() const
		{
			return Vertices.r();
		};
		inline unsigned int c() const
		{
			return Vertices.c();
		};
		inline ControlMesh& operator=(const ControlMesh &V)
		{
			Vertices=V.Vertices;
			return *this;
		};
		inline void Swap(ControlMesh &V)
		{
			Vertices.Swap(V.Vertices);
		}

		inline void Realloc( int i, int j)
		{
			Vertices.Realloc( i, j );
		};

		inline Vector4 &operator()(Nint i,Nint j)
		{
			return Vertices(i,j);
		}

		void TransformCP(const Matrix4x4& mat)
		{
			for(unsigned int i=0; i<Vertices.r(); ++i)
			{
				for(unsigned int j=0; j<Vertices.c(); ++j)
					Vertices(i,j)=mat*Vertices(i,j);
			}
		};

		inline const Vector4 &operator()(Nint i,Nint j)const
		{
			return Vertices(i,j);
		}

		ControlMesh(){};
		ControlMesh(int r, int c): Vertices(r,c){};
		ControlMesh( const ControlMesh& copy) : Vertices( copy.Vertices ) {};

	};



	//////////////////////////////////////////////////////////////////////////
	//																		//
	//								NurbsSurface							//
	//																		//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	template <class Nreal,class Nint> 
	class NurbsSurface
	{
	public:
		typedef Nreal real_t;
		typedef Nint int_t;
		typedef ThFixedVector< Nreal, 4 > Vector4;
		typedef ThFixedMatrix<Nreal, 4, 4 > Matrix4x4;
		typedef DynArray< Nreal, Shape1d > vector_t;
		typedef DynArray< Nreal > array_t;
		typedef ControlMesh< Nreal,Nint > control_poly_t;

	protected:
		friend class NurbsSurfaceTool<Nreal,Nint>;
		//friend class NurbsCurveTool<Nreal,Nint>;

		vector_t	U;		//knot vector for u direction	dim=n+Uorder+1
		vector_t	V;		//knot vector for v direction	dim=m+Vorder+1
		control_poly_t	P;		//dim=m x n
		Nint			Uorder;	//surface order in u direction
		Nint			Vorder;	//surface order in v direction		
		Nint			DUorder;//maximum derivative order in u direction
		Nint			DVorder;//maximum derivative order in v direction

		Nint			n,m;	//number of control points in u-v direction counted from zero
		array_t	Nu;		//computed span basis functions and their derivs up to DUorder in u direction	dim= DUorder+1 x Uorder+1
		array_t	Nv;		//computed span basis functions and their derivs up to DVorder in v direction	dim= DVorder+1 x Vorder+1
		vector_t  Nipu;	//computed single bf in u direction and its derivatives dim=DUorder+1
		vector_t  Nipv;	//computed single bf in v direction and its derivatives dim=DVorder+1
		vector_t	left,right; //knot differences	dim=max(Uorder,Vorder)+1
		//DynArray< Nreal >	ders;	//basis function derivatives where 1-st dimension is derivative order and second is function index
		array_t	ndu;	// upper triangle stores basis funcs, lower stores knot differences	dim=max(Uorder,Vorder)+1 x max(Uorder,Vorder)+1 
		array_t  a;		//stores coeficients to compute derivatives dim=2 x max(Uorder,Vorder)+1
		vector_t  ND;		//used in DersOneBasisFunc dim=max(DUorder,DVorder)
		DynArray< Vector4 > Aw;//used to calculate curve and surface derivatives dim=DUorder+1 x DVorder+1
		DynArray< Vector4, Shape1d > Temp;//used to calculate a point on a surface dim=Vorder+1

		DynArray< Vector4 > SKL;//stores computed point and its derivatives dim=DUorder+1 x DVorder+1 (3d point)

		Nint FindSpanU(Nreal u);//returns knot span index in u direction
		Nint FindSpanV(Nreal v);//returns knot span index in v direction
		Nint FindSpanMultV(Nreal v,Nint &s);//returns knot span index and multiplicity in v direction
		Nint FindSpanMultU(Nreal u,Nint &s);//returns knot span index and multiplicity in u direction
		void BasisFuncU(Nint i,Nreal u);//compute nonzero basis funcs in u direction
		void BasisFuncV(Nint i,Nreal v);//compute nonzero basis funcs in v direction
		void DersBasisFuncU(Nint i,Nreal u);//compute nonzero basis funcs and their derivatives ordered from 0 to DUorder in u direction
		void DersBasisFuncV(Nint i,Nreal v);//compute nonzero basis funcs and their derivatives ordered from 0 to DVorder in v direction
		void OneBasisFuncU(Nint i,Nreal u);// computes single basis function in u direction
		void OneBasisFuncV(Nint i,Nreal v);// computes single basis function in v direction
		void DersOneBasisFuncU(Nint i,Nreal u);//computes single basis function and it`s derivatives ordered from 0 to DUorder in u direction
		void DersOneBasisFuncV(Nint i,Nreal v);//computes single basis function and it`s derivatives ordered from 0 to DVorder in v direction		

		void Refresh(	Nint uOrder, Nint vOrder, Nint duOrder, Nint dvOrder );
	public:



		const Vector4& Result(unsigned int du=0,unsigned int dv=0) const
		{
			return SKL(du,dv);
		}

		NurbsSurface(	const control_poly_t& cp, const vector_t& uVec, const vector_t& vVec,
			Nint uOrder, Nint vOrder, Nint duOrder,	Nint dvOrder );

		NurbsSurface( const NurbsSurface& copy ) : U( copy.U), V(copy.V), P( copy.P ), Uorder( copy.Uorder ),	Vorder(copy.Vorder),	
			DUorder(copy.DUorder), DVorder(copy.DVorder), n(copy.n), m(copy.m),
			Nu(copy.Nu), Nv(copy.Nv), Nipu(copy.Nipu), Nipv(copy.Nipv), left(copy.left),
			right(copy.right), ndu(copy.ndu), a(copy.a), ND(copy.ND), Aw(copy.Aw), Temp(copy.Temp),SKL(copy.SKL)
		{};
		~NurbsSurface(){};
		Nreal Umin(void)const;//returns min value of U knot vector
		Nreal Umax(void)const;//returns max value of U knot vector
		Nreal Vmin(void)const;//returns min value of V knot vector
		Nreal Vmax(void)const;//returns max value of V knot vector


		void SetDUorder(Nint iDUorder)
		{
			Refresh(Uorder,Vorder,Max(Uorder,iDUorder),DVorder);
		}
		void SetDVorder(Nint iDVorder)
		{
			Refresh(Uorder,Vorder,DUorder,Max(Vorder,iDVorder));
		}

		const control_poly_t& GetControlPoly()const//gets control polygon
		{
			return P;
		}
		const vector_t& GetKnotVecU()const//gets  u knot vector
		{
			return U;
		}
		const vector_t& GetKnotVecV()const//gets  v knot vector
		{
			return V;
		}
		Nint GetDUorder(void)const//gets differentiation order in u direction
		{
			return DUorder;
		}
		Nint GetDVorder(void)const//gets differentiation order in v direction
		{
			return DVorder;
		}
		Nint GetUorder(void)const//gets order in u direction
		{
			return Uorder;
		}
		Nint GetVorder(void)const//gets order in v direction		
		{
			return Vorder;
		}

		void TransformCP(const Matrix4x4& mat)
		{
			P.TransformCP(mat);
		};

		Nint GetN()const
		{
			return n;
		}

		Nint GetM()const
		{
			return m;
		}

		void Point(Nreal u,Nreal v);//compute a point on a rational surface
		void PointDerivs(Nreal u,Nreal v);//compute a point on a rational surface with its derivatives

		inline Vector4 &operator()(Nint i,Nint j)
		{
			return P(i,j);
		};

		inline const Vector4 &operator()(Nint i,Nint j) const
		{
			return P(i,j);
		};

		void ReparametrizeU(Nreal c, Nreal d);
		void ReparametrizeV(Nreal c, Nreal d);
		void ReverseU();
		void ReverseV();


		inline NurbsSurface& operator=( const NurbsSurface& rhs )
		{
			U		=	rhs.U;
			V		=	rhs.V;
			P		=	rhs.P;
			Uorder	=	rhs.Uorder;
			Vorder	=	rhs.Vorder;
			DUorder	=	rhs.DUorder;
			DVorder	=	rhs.DVorder;
			n		=	rhs.n;
			m		=	rhs.m;
			Nu		=	rhs.Nu;
			Nv		=	rhs.Nv;
			Nipu	=	rhs.Nipu;
			Nipv	=	rhs.Nipv;
			left	=	rhs.left;
			right	=	rhs.right;
			ndu		=	rhs.ndu;
			a		=	rhs.a;
			ND		=	rhs.ND;
			Aw		=	rhs.Aw;
			Temp	=	rhs.Temp;
			SKL		=	rhs.SKL;

			return (*this);
		};
	};

#include <Thor/Core/Math/Nurbs/NurbsSurface.inl>

typedef NurbsSurface<float,int> NurbsS;

};

#endif