template <class Nreal, class Nint>
void NurbsCurveTool< Nreal,Nint >::KnotInsert(Nreal u, Nint r, curve_t& curve)
{

	if(r<=0)
		return;

	if(r>curve.Uorder)
		return;

	if( u<curve.U(0) || u>curve.U(curve.n+curve.Uorder) )
	{
		throw NurbsParameterOutOfRange();
	}

	Nint mp,nq,i,j,L;
	Nreal alpha;

	Nint s,k=curve.FindSpanMult(u,s);
	L=k-s;

	if( (r+s)>curve.Uorder ) r=curve.Uorder-s;

	mp=curve.n+curve.Uorder+1;//high index of U vec
	nq=curve.n+r;//updated number of CV

	control_poly_t Qw;
	Qw.Realloc(nq+1);

	control_poly_t Rw;
	Rw.Realloc(curve.Uorder+1);

	vector_t UQ;
	UQ.Realloc(mp+r+1);

	//Load knot vector
	for(i=0;i<=k;i++) UQ(i)=curve.U(i);//knots before insertion
	for(i=1;i<=r;i++) UQ(k+i)=u;//inserted knot
	for(i=k+1;i<=mp;i++) UQ(i+r)=curve.U(i);//knots after insertion
	//save unmodified CV
	for(i=0;i<=k-curve.Uorder;i++) Qw(i)=curve.P(i);
	for(i=k-s;i<=curve.n;i++) Qw(i+r)=curve.P(i);
	for(i=0;i<=curve.Uorder-s;i++) Rw(i)=curve.P(k-curve.Uorder+i);//load CVs needed in knot insertion process

	for(j=1;j<=r;j++)
	{
		L=k-curve.Uorder+j;
		for(i=0;i<=curve.Uorder-s-j;i++)
		{
			alpha=( u - curve.U(L+i) )/( curve.U(i+k+1)-curve.U(L+i) );
			Rw(i)=Rw(i+1)*alpha + Rw(i)*(1.0f-alpha);
		}

		Qw(L)=Rw(0);
		Qw(k+r-j-s)=Rw(curve.Uorder-j-s);
	}

	for(i=L+1;i<k-s;i++)
		Qw(i)=Rw(i-L);

	curve.P.Swap(Qw);
	curve.U.Swap(UQ);
	curve.n=nq;
}

//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
typename NurbsCurveTool< Nreal,Nint >::curve_t
NurbsCurveTool< Nreal,Nint >::Split(Nreal u, curve_t& curve)
{
	if( u<curve.U(0) || u>curve.U(curve.n+curve.Uorder) )
	{
		throw NurbsParameterOutOfRange();
	}

	Nint s,k=curve.FindSpanMult(u,s),i;
	KnotInsert(u,curve.Uorder-s,curve);

	vector_t resU;
	//load knot vector
	resU.Realloc(curve.U.c()-k+s);

	resU(0)=u;

	for(i=1;i<resU.c();i++)
	{
		resU(i)=curve.U(i+k-s);
	}


	//DynArray<Nreal,Shape1d> tU;//temp knot vector


	//tU=curve.U;
	curve.U.Realloc(k+1+curve.Uorder);
	//tU.Copy(left.U,k+1);

	for(i=1;i<=curve.Uorder;i++)
	{
		curve.U(k+i)=u;
	}
	//U.Swap(tU);


	//load CV
	control_poly_t resP;

	resP.Realloc(curve.n-k+1+s);
	//right.n=curve.n-k+s;
	resP(0)=curve.P(k-s);

	for(i=1;i<resP.c();i++)
	{
		resP(i)=curve.P(i+k-s);
	}
	//ControlPolygon tP;//temp knot vector


	//tP=curve.P;
	//tP.Copy(P,k+1-s);
	curve.P.Realloc(k+1-s);

	//P.Swap(tP);
	curve.n=k-s;

	return curve_t(resP,resU,curve.Uorder,curve.DUorder);
}
//////////////////////////////////////////////////////////////////////////
/*template <class Nreal, class Nint>
Nint NurbsCurveTool< Nreal,Nint >::CurvIntLine(Vector4 &P1,Vector4 &V1,Nreal &u1,Nreal &u2,Nreal eps)
{
if( (DUorder<2) )
{
NurbsErrors.Push(ERR_INTERSECT_CANNOT_DIFF);
return 0;
}

Nreal du1,w1,alpha=PI/20.0;
Nreal cross,dp;
Vector4 dP=P(n)-P(0);
dp=dP.Length3D();



u1=Umin();
Nbool done=false;


while ( u1<=Umax() )
{
CurvePointDerivs(u1);
IntersectLines( SKL(0,0), P1, SKL(1,0), V1, w1, u2);
dP=SKL(1,0)%SKL(2,0);
cross=dP.Length3D();
if( cross>0.01 )
{
du1=alpha*SKL(1,0).Length3D() / cross;
}
else
{            			
du1=Umax()/dp;
}

if ( abs(w1)<=du1 )
break;
else 
{
u1+=du1;

}
}

Nreal dt,a,b,c,d,f,g,det;
du1=eps*2.0f;
dt=du1;
dP=SKL(0,0) - (P1+V1*u2);
dp=dP.Length3D();
if(u1>Umax()) u1=Umax();
if(u1<Umin()) u1=Umin();
int i=0;

while( (du1>eps) || (dt>eps) || (dp>eps) )
{
CurvePointDerivs(u1);
dP=SKL(0,0) - (P1+V1*u2);
dp=dP.Length3D();
f=( SKL(0,0) - (P1+V1*u2) )*SKL(1,0);
g=( (P1+V1*u2) - SKL(0,0) )*V1;
a=SKL(1,0)*SKL(1,0)+SKL(2,0)*( SKL(0,0) - (P1+V1*u2) );
b=SKL(1,0)*V1;
b=-b;
c=b;
d=V1*V1;
det=a*d-b*c;
du1=(b*g-d*f)/det;
dt=(f*c-a*g)/det;

u1=u1+du1;
u2=u2+dt;
if( ( u1>Umax() ) || ( u1<Umin() ) )
{
return 0;
}
i++;

}


return 1;

}*/
//////////////////////////////////////////////////////////////////////////
/*template <class Vector4,class Nreal, class Nint>
Nint NurbsCurveTool< Vector4,Nreal,Nint >::IntersectLines(Vector4 &P1,Vector4 &P2,Vector4 &V1,Vector4 &V2,Nreal &t1,Nreal &t2)
{
Vector4 N1,N2,P12;
Nreal temp;

N1=V1;
SWAP(N1.x,N1.y,temp);
N1.x=-N1.x;

N2=V2;
SWAP(N2.x,N2.y,temp);
N2.x=-N2.x;

N1.Normalize3D();
N2.Normalize3D();

if( abs(N1*N2)==1 ) return 0;

P12=P2-P1;

t1=N2*P12 / (N2*V1);
t2=N1*P12*(-1) / (N1*V2);

return 1;


}*/
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsCurveTool< Nreal,Nint >::KnotRefine( const vector_t& x, curve_t& curve)
{

	if(x.Size()<1) return;
	Nint mm=curve.n+curve.Uorder+1;
	Nint r=x.c()-1;
	Nint a=curve.FindSpan( x(0) );
	Nint b=curve.FindSpan( x(r) ) +1;
	Nint i,j,k,l,ind;
	Nreal alfa;

	control_poly_t Qw(curve.n+r+2);	
	vector_t Ubar(mm+r+2);

	for(j=0;j<=a-curve.Uorder;j++)
		Qw(j)=curve.P(j);

	for(j=b-1;j<=curve.n;j++)
		Qw(j+r+1)=curve.P(j);

	for(j=0;j<=a;j++)
		Ubar(j)=curve.U(j);

	for(j=b+curve.Uorder;j<=mm;j++)
		Ubar(j+r+1)=curve.U(j);

	i=b+curve.Uorder-1;
	k=b+curve.Uorder+r;

	for(j=r;j>=0;j--)
	{
		while( ( x(j) <= curve.U(i) ) && ( i>a ) )
		{
			Qw(k-curve.Uorder-1)=curve.P(i-curve.Uorder-1);
			Ubar(k)=curve.U(i);
			k=k-1;
			i=i-1;
		}

		Qw(k-curve.Uorder-1)=Qw(k-curve.Uorder);

		for( l=1; l<=curve.Uorder; l++ )
		{
			ind=k-curve.Uorder+l;
			alfa=Ubar(k+l)-x(j);
			if( abs(alfa)==0.0 )
				Qw(ind-1)=Qw(ind);
			else
			{
				alfa=alfa/( Ubar(k+l) - curve.U(i-curve.Uorder+l) );
				Qw(ind-1)=alfa*Qw(ind-1) + (1.0-alfa)*Qw(ind);
			}
		}
		Ubar(k) = x(j);
		k=k-1;
	}

	curve.U.Swap( Ubar );
	curve.P.Swap( Qw );
	curve.n=curve.n+r+1;
}

template <class Nreal, class Nint>
std::vector<typename  NurbsCurveTool< Nreal,Nint >::curve_t> 
NurbsCurveTool< Nreal,Nint >::DecomposeToBezier(const curve_t& curve)
{
	Nint m=curve.n+curve.Uorder+1;
	Nint a=curve.Uorder;
	Nint b=curve.Uorder+1;
	Nint nb=0;
	vector_t alphas(curve.Uorder);
	vector_t resU( 2*(curve.Uorder+1) );
	control_poly_t resP(curve.Uorder+1);
	control_poly_t resP1(curve.Uorder+1);
	std::vector<curve_t> result;

	if( b<m )
	{
		for(Nint i=0; i<=curve.Uorder; ++i)
			resP(i)=curve.P(i);

		for(Nint i=0; i<=curve.Uorder; ++i)
			resU(i)=0.0;//a

		for(Nint i=curve.Uorder+1; i< 2*(curve.Uorder+1); ++i)
			resU(i)=1.0;//b
	}
	

	while( b<m )
	{
		Nint i=b;

		while( ( b<m ) && ( curve.U(b+1)==curve.U(b) ) )
			++b;

		Nint mult=b-i+1;

		if(mult<curve.Uorder)
		{
			Nreal numer=curve.U(b)-curve.U(a);

			for(Nint j=curve.Uorder; j>mult; --j)
				alphas(j-mult-1)=numer/( curve.U(a+j)-curve.U(a) );

			Nint r=curve.Uorder-mult;

			for(Nint j=1; j<=r; ++j)
			{
				Nint save=r-j;
				Nint s=mult+j;

				for(Nint k=curve.Uorder; k>=s; --k)
				{
					Nreal alpha=alphas(k-s);
					resP(k)=alpha*resP(k)+(1.0-alpha)*resP(k-1);
				}

				if(b<m)
				{
					resP1(save)=resP(curve.Uorder);
				}
			}
		}

		result.push_back( curve_t(resP,resU,curve.Uorder,curve.DUorder) );
		nb=nb+1;
		resP.Swap(resP1);

		if(b<m)
		{
			for(Nint i=curve.Uorder-mult; i<=curve.Uorder; ++i)
				resP(i)=curve.P(b-curve.Uorder+i);
			a=b;
			b=b+1;
		}
	}

	return result;
}

template <class Nreal, class Nint>
Nreal NurbsCurveTool< Nreal,Nint >::ComputeTolerance(const curve_t& curve)
{	
	Nreal wMin=std::numeric_limits<Nreal>::max();
	Nreal pMax=0.0;

	for(Nint i=0; i<=curve.n; ++i)
	{
		if( curve.P(i).w()<wMin )
			wMin=curve.P(i).w();

		Nreal temp=curve.P(i)*curve.P(i);

		if( temp>pMax )
			pMax=temp;
	}

	return wMin / ( 1+Math::Sqrt(pMax) );

}

template <class Nreal, class Nint>
Nint NurbsCurveTool< Nreal,Nint >::KnotRemove(Nreal u, Nint num, curve_t& curve, Nreal tol)
{
	Nint s,r=curve.FindSpanMult(u,s);
	Nint t=0;
	Nint m=curve.n+curve.Uorder+1;
	Nint ord=curve.Uorder+1;
	Nint fout=(2*r-s-curve.Uorder)/2;
	Nint last=r-s;
	Nint first=r-curve.Uorder;
	DynArray<Vector4,Shape1d> temp(2*curve.Uorder+1);
	Nreal TOL=tol*ComputeTolerance(curve);

	for(t=0; t<num; ++t)
	{
		Nint off=first-1;
		temp(0)=curve.P(off);
		temp(last+1-off)=curve.P(last+1);

		Nint i=first;
		Nint j=last;
		Nint ii=1;
		Nint jj=last-off;
		Nint remflag=0;

		while( (j-i)>t )
		{
			Nreal alfi=( u-curve.U(i) ) / ( curve.U(i+ord+t)-curve.U(i) );
			Nreal alfj=( u-curve.U(j-t) ) / ( curve.U(j+ord)-curve.U(j-t) );
			temp(ii)=( curve.P(i)-(1.0-alfi)*temp(ii-1) ) / alfi;
			temp(jj)=( curve.P(j)-alfj*temp(jj+1) ) / ( 1.0-alfj );
			i=i+1; ii=ii+1;
			j=j-1; jj=jj-1;
		}

		
		if( (j-i)<t )
		{
			if( temp(ii-1).Distance( temp(jj+1) ) <= TOL )
				remflag=1;
		}
		else
		{
			Nreal alfi=( u-curve.U(i) ) / ( curve.U(i+ord+t)-curve.U(i) );

			if( curve.P(i).Distance( alfi*temp(ii+t+1)+(1.0-alfi)*temp(ii-1) ) <=TOL )
				remflag=1;
		}

		if(remflag==0)
			break;
		else
		{
			i=first; j=last;

			while( (j-i)>t )
			{
				curve.P(i)=temp(i-off);
				curve.P(j)=temp(j-off);
				i=i+1; j=j-1;
			}
		}
		first=first-1; last=last+1;
	}

	if(t==0)
		return t;

	for(Nint k=r+1; k<=m; ++k)
		curve.U(k-t)=curve.U(k);

	curve.U.Realloc(curve.U.c()-t);

	Nint j=fout;
	Nint i=j;

	for(Nint k=1; k<t; ++k)
	{
		if( (k%2)==1 )
			i=i+1;
		else
			j=j-1;
	}

	for(Nint k=i+1; k<=curve.n; k++)
	{
		curve.P(j)=curve.P(k);
		j=j+1;
	}

	curve.P.Realloc(curve.P.c()-t);
	curve.n=curve.P.c()-1;
	return t;
}

template <class Nreal, class Nint>
void NurbsCurveTool< Nreal,Nint >::DegreeElevate(Nint t, curve_t& curve)
{
	Nint mm=curve.n+curve.Uorder+1;
	Nint ph=curve.Uorder+t;
	Nint ph2=ph/2;
	DynArray<Nreal> bezalfs(curve.Uorder+t+1,curve.Uorder+1);
	bezalfs(0,0)=1.0;
	bezalfs(ph,curve.Uorder)=1.0;
	
	Nint newN=curve.n*(t+1)+t+1;//new number of control points
	Nint newM=newN+curve.Uorder+t+1;//curve.U.c()+2*t;//new high index
	//

	for(Nint i=1; i<=ph2; ++i)
	{
		Nreal inv=Nreal(1.0) / Math::Bin<Nreal>(ph,i);
		Nint mpi=Min(curve.Uorder,i);

		for(Nint j=Max(0,i-t); j<=mpi; ++j)
			bezalfs(i,j)=inv*Math::Bin<Nreal>(curve.Uorder,j)*Math::Bin<Nreal>(t,i-j);
	}

	for(Nint i=ph2+1; i<=(ph-1); ++i)
	{
		Nint mpi=Min(curve.Uorder,i);

		for(Nint j=Max(0,i-t); j<=mpi; ++j)
			bezalfs(i,j)=bezalfs(ph-i,curve.Uorder-j);
	}

	Nint mh=ph;
	Nint kind=ph+1;
	Nint r =-1;
	Nint a=curve.Uorder;
	Nint b=curve.Uorder+1;
	Nint cind=1;
	Nreal ua=curve.U(0);
	control_poly_t Qw(newN);
	vector_t Uh(newM+1);
	
	Qw(0)=curve.P(0);

	for(Nint i=0; i<=ph; ++i)
		Uh(i)=ua;

	control_poly_t bpts(curve.Uorder+1);
	control_poly_t Nextbpts(curve.Uorder-1);
	vector_t alfs(curve.Uorder-1);
	control_poly_t ebpts(curve.Uorder+t+1);


	//first bezier segment
	for(Nint i=0; i<=curve.Uorder; ++i)
		bpts(i)=curve.P(i);

	while( b<mm )
	{
		Nint i=b;

		while( (b<mm) && ( curve.U(b)==curve.U(b+1) ) )
			b=b+1;

		Nint mul=b-i+1;
		mh=mh+mul+t;
		Nreal ub=curve.U(b);
		Nint oldr=r;
		r=curve.Uorder-mul;
		//insert u(b) r times
		Nint lbz=0;
		if(oldr>0)
			lbz=(oldr+2)*0.5;
		else
			lbz=1;

		Nint rbz=0;
		if(r>0)
			rbz=ph-(r+1)*0.5;
		else
			rbz=ph;

		if(r>0)
		{
			Nreal numer=ub-ua;		

			for(Nint k=curve.Uorder; k>mul; --k)
				alfs(k-mul-1)=numer / (curve.U(a+k)-ua);

			for(Nint j=1; j<=r; ++j)
			{
				Nint save=r-j;
				Nint s=mul+j;
				for(Nint k=curve.Uorder; k>=s; --k)
				{
					bpts(k)=alfs(k-s)*bpts(k)+(1.0-alfs(k-s))*bpts(k-1);
				}

				Nextbpts(save)=bpts(curve.Uorder);
			}
		}//end of insert knot		

		for(int i=lbz; i<=ph; ++i)
		{
			ebpts(i)=Vector4(0.0,0.0,0.0,0.0);
			Nint mpi=Min(curve.Uorder,i);

			for(Nint j=Max(0,i-t); j<=mpi; ++j)
				ebpts(i)=ebpts(i)+bezalfs(i,j)*bpts(j);
		}

		if(oldr>1)
		{
			Nint first=kind-2; Nint last=kind;
			Nreal den=ub-ua;
			Nreal bet=( ub-Uh(kind-1) ) / den;

			for(Nint tr=1; tr<oldr; ++tr)
			{//knot removal loop
				Nint i=first; Nint j=last; Nint kj=j-kind+1;
				while( (j-i)>tr )
				{
					if(i<cind)
					{
						Nreal alf=( ub-Uh(i) ) / ( ua-Uh(i) );
						Qw(i)=alf*Qw(i) + (1.0-alf)*Qw(i-1);
					}
					if(j>=lbz)
					{
						if( (j-tr)<=(kind-ph+oldr) )
						{
							Nreal gam=( ub-Uh(j-tr) ) / den;
							ebpts(kj)=gam*ebpts(kj)+(1.0-gam)*ebpts(kj+1);
						}
						else
							ebpts(kj)=bet*ebpts(kj)+(1.0-bet)*ebpts(kj+1);
					}

					i=i+1; j=j-1; kj=kj-1;
				}

				first=first-1; last=last+1;
			}
		}

		if( a!=curve.Uorder )
		{
			for( Nint i=0; i<(ph-oldr); ++i )
			{
				Uh(kind)=ua;
				kind=kind+1;
			}
		}

		for(Nint j=lbz; j<=rbz; ++j)
		{
			Qw(cind)=ebpts(j);
			cind=cind+1;
		}		

		if(b<mm)
		{
			for(Nint j=0; j<r; ++j)
				bpts(j)=Nextbpts(j);

			for(Nint j=r; j<=curve.Uorder; ++j)
				bpts(j)=curve.P(b-curve.Uorder+j);

			a=b; b=b+1; ua=ub;
		}
		else
		{
			for(Nint i=0; i<=ph; ++i)
				Uh(kind+i)=ub;
		}	
	}

	Qw.Realloc(mh-ph);
	Uh.Realloc(mh-ph+curve.Uorder+t+1);
	curve.n=mh-ph-1;
	//curve.Uorder+=t;
	curve.P.Swap(Qw);
	curve.U.Swap(Uh);
	curve.Refresh(curve.Uorder+t, curve.DUorder);
}

template <class Nreal, class Nint>
void NurbsCurveTool< Nreal,Nint >::BezDegreeReduce(const control_poly_t& bpts,control_poly_t& rbpts,Nreal& maxErr)
{
	Nint p=rbpts.c();
	Nint r=(p-1)/2;

	rbpts(0)=bpts(0);
	rbpts(p-1)=bpts(p);

	if( (p%2)==0 )//even
	{
		for(int i=1; i<=r; ++i)
		{
			Nreal alfa=Nreal(i)/Nreal(p);
			rbpts(i)= ( bpts(i)-alfa*rbpts(i-1) ) / (1-alfa);
		}

		for(int i=r+1; i<=(p-2); ++i)
		{
			Nreal alfa=Nreal( (i+1) )/ Nreal( p );
			rbpts(i)= ( bpts(i+1)-(1.0-alfa)*rbpts(i+1) ) / alfa;
		}

		Vector4 len=bpts(r+1)-0.5*( rbpts(r)+rbpts(r+1) );
		maxErr=len.Length();

	}
	else//odd
	{
		for(int i=1; i<=(r-1); ++i)
		{
			Nreal alfa=Nreal(i)/Nreal(p);
			rbpts(i)= ( bpts(i)-alfa*rbpts(i-1) ) / (1-alfa);
		}

		for(int i=r+1; i<=(p-2); ++i)
		{
			Nreal alfa=Nreal( (i+1) )/ Nreal( p );
			rbpts(i)= ( bpts(i+1)-(1.0-alfa)*rbpts(i+1) ) / alfa;
		}

		Nreal alfar=Nreal(r)/Nreal(p);
		Vector4 Pl=( bpts(r)-alfar*rbpts(r-1) ) / (1-alfar);
		alfar=Nreal( (r+1) )/ Nreal( p );
		Vector4 Pr=( bpts(r+1)-(1.0-alfar)*rbpts(r+1) ) / alfar;
		rbpts(r)=0.5*(Pl+Pr);

		Vector4 len=Pl-Pr;
		maxErr=len.Length();
	}	
}

template <class Nreal, class Nint>
bool NurbsCurveTool< Nreal,Nint >::DegreeReduce(curve_t& curve, Nreal tol)
{
	Nint ph=curve.Uorder-1;
	Nint mh=ph;
	Nint kind=ph+1;
	Nint r=-1;
	Nint a=curve.Uorder;
	Nint b=curve.Uorder+1;
	Nint cind=1;
	Nint mult=curve.Uorder;
	Nint mm=curve.n+curve.Uorder+1;
	control_poly_t Pw(curve.n);
	vector_t Uh( curve.U.c()-2 );
	control_poly_t bpts(curve.Uorder+1);
	control_poly_t Nextbpts(curve.Uorder-1);
	control_poly_t rbpts(curve.Uorder);
	vector_t alphas(curve.Uorder-1);
	vector_t e(mm);
	Nreal TOL=tol*ComputeTolerance(curve);
	Pw(0)=curve.P(0);

	for(Nint i=0; i<=ph; ++i)
		Uh(i)=curve.U(0);

	for(Nint i=0; i<=curve.Uorder; ++i)
		bpts(i)=curve.P(i);

	for(Nint i=0; i<mm; ++i)
		e(i)=0.0;

	while( b<mm )
	{
		Nint i=b;

		while( (b<mm) && ( curve.U(b)==curve.U(b+1) ) )
			b=b+1;

		mult=b-i+1;
		mh=mh+mult-1;
		Nint oldr=r;
		r=curve.Uorder-mult;

		Nint lbz=0;
		if(oldr>0)
			lbz=(oldr+2)/2;
		else
			lbz=1;

		if(r>0)
		{
			Nreal numer=curve.U(b)-curve.U(a);

			for(Nint k=curve.Uorder; k>=mult; --k)
				alphas(k-mult-1)=numer/( curve.U(a+k)-curve.U(a) );

			for(Nint j=1; j<=r; ++j)
			{
				Nint save=r-j;
				Nint s=mult+j;

				for(Nint k=curve.Uorder; k>=s; --k)
					bpts(k)=alphas(k-s)*bpts(k)+(1.0-alphas(k-s))*bpts(k-1);

				Nextbpts(save)=bpts(curve.Uorder);
			}
		}

		Nreal maxErr=0.0;
		BezDegreeReduce(bpts,rbpts,maxErr);
		e(a)+=maxErr;

		if( e(a)>TOL )
			return false;

		if(oldr>0)
		{
			Nint first=kind;
			Nint last=kind;

			for(Nint k=0; k<oldr; ++k)
			{
				Nint i=first;
				Nint j=last;
				Nint kj=j-kind;

				while( (j-i)>k )
				{
					Nreal alfa=( curve.U(a)-Uh(i-1) ) / ( curve.U(b)-Uh(i-1) );
					Nreal beta=( curve.U(a)-Uh(j-k-1) ) / ( curve.U(b)-Uh(j-k-1) );
					Pw(i-1)=( Pw(i-1)-(1.0-alfa)*Pw(i-2) ) / alfa;
					rbpts(kj)=( rbpts(kj)-beta*rbpts(kj+1) ) / (1.0-beta);
					++i; --j; --kj;
				}

				Nreal Br=0.0;
				if( (j-i)<k )
					Br=Pw(i-2).Distance(rbpts(kj+1));
				else
				{
					Nreal delta=( curve.U(a)-Uh(i-1) ) / ( curve.U(b)-Uh(i-1) );
					Vector4 A=delta*rbpts(kj+1)+(1.0-delta)*Pw(i-2);
					Br=Pw(i-1).Distance(A);
				}

				Nint K=a+oldr-k;
				Nint q=(2*curve.Uorder-k+1)/2;
				Nint L=K-q;

				for(Nint ii=L; ii<=a; ++ii)
				{
					e(ii)+=Br;

					if( e(ii)>TOL )
						return false;
				}

				--first; ++last;
			}
			cind=i-1;
		}

		if(a!=curve.Uorder)
		{
			for(Nint i=0; i<(ph-oldr); ++i)
			{
				Uh(kind)=curve.U(a);
				++kind;
			}
		}

		for(Nint i=lbz; i<=ph; ++i)
		{
			Pw(cind)=rbpts(i);
			++cind;
		}

		if(b<mm)
		{
			for(Nint i=0; i<r; ++i)
				bpts(i)=Nextbpts(i);

			for(Nint i=r; i<=curve.Uorder; ++i)
				bpts(i)=curve.P(b-curve.Uorder+i);

			a=b;
			++b;
		}
		else
		{
			for(Nint i=0; i<=ph; ++i)
				Uh(kind+i)=curve.U(b);
		}
	}
	curve.n=mh-ph-1;
	//curve.Uorder--;
	curve.P.Swap(Pw);
	curve.U.Swap(Uh);
	curve.Refresh(curve.Uorder-1,curve.DUorder);
	return true;
}

template <class Nreal, class Nint>
GeometryPtr NurbsCurveTool< Nreal,Nint >::MakeControlPolygonGeometry(const control_poly_t& cp, const GpuDevicePtr& device, const EffectPtr& effect,const std::string& name)
{
	assert(device!=0 &&"Null geometry pointer");
	assert(effect!=0 &&"Null effect pointer");

	IndicesPtr ind=device->CreateIndices(eGeometryTopology::LinesStrip,0);

	GeometryPtr geo=Geometry::Create(device,effect,ind);
	//TODO: default effect
	//if(effect!=0)
	
	AttributePtr pos=geo->NewAttribute(AttribNames::P.name, AttribQuery(AttribNames::P.get_type_id(),AttribNames::P.semantic) );
	
	std::vector<AttribNames::P_t::value_type> positions( cp.Size() );
	std::vector<IIndices::value_type> indices( cp.Size() );

	for(unsigned int i=0; i<cp.Size(); ++i )
	{
		positions[i]=cp(i)/cp(i).w();
		indices[i]=i;
	}

	ind->Swap(indices);
	pos->Swap(positions);

	return geo;
};

template <class Nreal, class Nint>
GeometryPtr NurbsCurveTool< Nreal,Nint >::MakeCurveGeometry(curve_t& curve, unsigned int step, const GpuDevicePtr& device, const EffectPtr& effect, unsigned int bitfield,const std::string& name)
{
	assert(device!=0 &&"Null geometry pointer");
	assert(effect!=0 &&"Null effect pointer");
	assert(step>1);

	IndicesPtr ind=device->CreateIndices(eGeometryTopology::LinesStrip,0);

	GeometryPtr geo=Geometry::Create(device,effect,ind);
	//TODO: default effect
	AttributePtr pos,dPdu,N;
	std::vector<IIndices::value_type> indices( step );
	std::vector<AttribNames::P_t::value_type> positions;
	std::vector<AttribNames::N_t::value_type> normals;
	std::vector<AttribNames::dPdu_t::value_type> tangents;
	unsigned int gP=bitfield & genP;
	unsigned int gdPdu=( (bitfield & gendPdu) && ( curve.GetDUorder()>0 ) );
	//unsigned int gN=(bitfield & genN) && ( curve.GetDUorder()>0 );
	
	if(gP)
	{
		pos=geo->NewAttribute(AttribNames::P.name, AttribQuery(AttribNames::P.get_type_id(), AttribNames::P.semantic) );
		positions.resize(step);
	}

	if(gdPdu)
	{
		dPdu=geo->NewAttribute(AttribNames::dPdu.name, AttribQuery(AttribNames::dPdu.get_type_id(),AttribNames::dPdu.semantic) );
		tangents.resize(step);
	}

	/*if(gN)
	{
		N=&(chunk->NewAttribute(AttribNames::N.name, AttribNames::N.type_id ));
		normals.resize(step);
	}*/	

	Nreal du=curve.Umax()/(step-1);
	Nreal u=curve.Umin();

	for(unsigned int i=0; i<step; ++i )
	{
		curve.PointDerivs(u);
		//curve.Point(u);

		if(gP)
		{
			positions[i]=curve.Result();
		}

		if(gdPdu)
		{
			tangents[i]=curve.Result(1);
		}
		
		indices[i]=i;
		u+=du;
	}

	ind->Swap(indices);

	if(gP)
		pos->Swap(positions);

	if(gdPdu)
		dPdu->Swap(tangents);

	return geo;
	
}


template <class Nreal, class Nint>
typename NurbsCurveTool< Nreal,Nint >::curve_t
NurbsCurveTool< Nreal,Nint >::MakeCircle(Nreal radius, Nreal start, Nreal end)
{
	if( end < start )
		end=Nreal(360.0) + end;

	Nreal theta=end-start;
	Nint narcs=0;

	if(theta<=90.0)
		narcs=1;
	else if(theta<=180.0)
		narcs=2;
	else if(theta<=270.0)
		narcs=3;
	else
		narcs=4;

	using namespace Thor::Math;

	theta=DegToRad(theta);
	start=DegToRad(start);
	Nreal dtheta=theta/narcs;
	Nint n=2*narcs;
	Nreal w1=Cos( dtheta / Nreal(2.0) );
	Vector4 P0( radius*Cos(start), radius*Sin(start), 0.0, 1.0 );
	ThVec3 T0( -Sin(start), Cos(start), 0.0 );
	control_poly_t Pw(n+1);

	Pw(0)=P0;
	Nint index=0;
	Nreal angle=start;

	for(int i=1; i<=narcs; ++i)
	{
		angle+=dtheta;
		Vector4 P2( radius*Cos(angle), radius*Sin(angle), 0.0, 1.0 );
		Pw(index+2)=P2;
		ThVec3 T2( -Sin(angle), Cos(angle), 0.0 );

		Ray r0( ThVec3( P0(0), P0(1), P0(2) ), T0 );
		Ray r2( ThVec3( P2(0), P2(1), P2(2) ), T2 );

		Nreal t=0.0,dummy=0.0;
		Vector4 P1;

		if( IntersectRays(r0,r2,t,dummy) )
		{
			ThVec3 res=r0.Point(t);
			P1(0)=res(0)*w1;
			P1(1)=res(1)*w1;
			P1(2)=res(2)*w1;
			P1(3)=w1;
		}
		Pw(index+1)=P1;

		index+=2;

		if(i<narcs)
		{
			P0=P2;
			T0=T2;
		}
	}

	Nint j=2*narcs+1;
	vector_t U(n+2+1+1);

	for(Nint i=0; i<3; ++i)
	{
		U(i)=Nreal(0.0);
		U(i+j)=Nreal(1.0);
	}

	switch(narcs)
	{
	case 1:
		break;
	case 2:
		U(3)=Nreal(0.5);
		U(4)=Nreal(0.5);
		break;
	case 3:
		U(3)=U(4)=Nreal(1.0 / 3.0);
		U(5)=U(6)=Nreal(2.0 / 3.0);
		break;
	case 4:
		U(3)=U(4)=Nreal(0.25);
		U(5)=U(6)=Nreal(0.5);
		U(7)=U(8)=Nreal(0.75);
	}

	return curve_t(Pw,U,2,1);

}

template <class Nreal, class Nint>
typename NurbsCurveTool< Nreal,Nint >::curve_t
NurbsCurveTool< Nreal,Nint >::MakeEllipse(Nreal a, Nreal b)
{
	Nreal rmin=Min(a,b);
	curve_t curve=MakeCircle(rmin,0.0,360.0);

	//unsigned int n=curve.GetControlPoly().Size()-1;


	if(rmin==a)
	{
		Nreal diff=b-a;
		Nreal w=curve(1).w();
		Vector4 v( Nreal(0.0), diff, Nreal(0.0), Nreal(0.0) );
		Vector4 wv( Nreal(0.0), w*diff, Nreal(0.0), Nreal(0.0) );

		curve(1)+=wv;
		curve(2)+=v;
		curve(3)+=wv;
		curve(5)-=wv;
		curve(6)-=v;
		curve(7)-=wv;
	}
	else
	{
		Nreal diff=a-b;
		Nreal w=curve(1).w();
		Vector4 v( diff, Nreal(0.0), Nreal(0.0), Nreal(0.0) );
		Vector4 wv( w*diff, Nreal(0.0), Nreal(0.0), Nreal(0.0) );

		curve(0)+=v;
		curve(1)+=wv;
		curve(7)+=wv;
		curve(8)=curve(0);
		curve(3)-=wv;
		curve(4)-=v;
		curve(5)-=wv;
	}

	return curve;
	
}

template <class Nreal, class Nint>
//typename NurbsCurveTool< Nreal,Nint >::curve_t
Nreal NurbsCurveTool< Nreal,Nint >::MakeConicArc(const Vector3& P0, const Vector3& T0, const Vector3& P2, const Vector3& T2, const Vector3& P, Vector3& P1)
{
	Nreal t1,t2;
	Vector3 V02=P2-P0;
	Ray r0(P0,T0);
	Ray r2(P2,T2);
	Nreal w1=1.0;
	bool res=IntersectRays( r0, r2, t1, t2 );
	//Vector3
	P1=r0.Point(t1);
	
	if(res)
	{
		Vector3 V1P=P-P1;
		Ray r1P(P1,V1P);
		Ray r02(P0,V02);
		IntersectRays(r1P,r02,t1,t2);
		Vector3 Q=r02.Point(t2);
		Vector3 P0Q=Q-P0;
		Vector3 QP2=P2-Q;
		Nreal a=Math::Sqrt( P0Q.Length() / QP2.Length() );
		Nreal u=a / ( Nreal(1.0) + a );
		Vector3 P1P=P1-P;
		Nreal num=( Nreal(1.0) - u) * ( Nreal(1.0) - u) * (P-P0) * P1P + u*u*(P-P2)*P1P;
		Nreal den=Nreal(2.0) * u * ( Nreal(1.0) - u ) * P1P * P1P;
		w1=num/den;
	}
	else
	{
		w1=0.0;
		Ray rP0(P,T0);
		Ray r02(P0,V02);
		IntersectRays(rP0,r02,t1,t2);
		Vector3 Q=r02.Point(t2);
		Vector3 P0Q=Q-P0;
		Vector3 QP2=P2-Q;
		Nreal a=Math::Sqrt( P0Q.Length() / QP2.Length() );
		Nreal u=a / ( Nreal(1.0) + a );
		Nreal b=Nreal(2.0) * u * ( Nreal(1.0) - u);
		b=-t1 * ( Nreal(1.0) - b ) / b;
		P1=b*T0;
	}

	/*control_poly_t Pw(3);
	Pw(0)=Vec4(P0,1.0);
	if(w1!=0.0)
		Pw(1)=Vec4(w1*P1,w1);
	else
		Pw(1)=Vec4(P1,0.0);
	Pw(2)=Vec4(P2,1.0);
	vector_t U(6);
	for(int i=0; i<3; ++i)
	{
		U(i)=0.0;
		U(3+i)=1.0;
	}

	return curve_t(Pw,U,2,1);*/
	return w1;
}

template <class Nreal, class Nint>
//typename NurbsCurveTool< Nreal,Nint >::curve_t
Nreal NurbsCurveTool< Nreal,Nint >::Angle(const Vector3& P, const Vector3& Q, const Vector3& R)
{
	using namespace Math;
	Vector3 x=P-Q;
	Vector3 y=R-Q;
	x.Normalize();
	y.Normalize();
	return RadToDeg( Acos(x*y) );
}

template <class Nreal, class Nint>
typename NurbsCurveTool< Nreal,Nint >::curve_t
NurbsCurveTool< Nreal,Nint >::MakeOpenConic(const Vector3& P0, const Vector3& T0, const Vector3& P2, const Vector3& T2, const Vector3& P)
{
	Vector3 P1;
	Nreal w1=MakeConicArc(P0,T0,P2,T2,P,P1);
	Nint nsegs=0;

	if(w1<=-1.0)
		throw NurbsNegativeWeight();

	if(w1>=1.0)
		nsegs=1;
	else
	{
		if( ( w1>0.0 ) && ( Angle(P0,P1,P2) > 60.0 ) )
			nsegs=1;
		else if( ( w1 < 0.0 ) && ( Angle(P0,P1,P2) > 90.0 ) )
			nsegs=4;
		else
			nsegs=2;
	}

	Nint n=2*nsegs;
	Nint j=2*nsegs+1;
	vector_t U(n+4);
	control_poly_t Pw(n+1);

	for(Nint i=0; i<3; ++i)
	{
		U(i)=Nreal(0.0);
		U(i+j)=Nreal(1.0);
	}

	Pw(0)=Vector4(P0, Nreal(1.0) );
	Pw(n)=Vector4(P2, Nreal(1.0) );

	if(nsegs==1)
	{
		if(w1!=0.0)
			Pw(1)=Vector4( w1*P1,w1 );
		else 
			Pw(1)=Vector4( P1, Nreal(0.0) );
		return curve_t(Pw,U,2,1);
	}

	Nreal wqr;
	Vector3 Q1,S,R1;
	SplitArc(P0,P1,w1,P2,Q1,S,R1,wqr);

	if(nsegs==2)
	{
		Pw(2)=Vector4(S, Nreal(1.0) );
		Pw(1)=Vector4(wqr*Q1,wqr);
		Pw(3)=Vector4(wqr*R1,wqr);
		U(3)=0.5;
		U(4)=0.5;
		return curve_t(Pw,U,2,1);
	}

	Pw(4)=Vector4(S, Nreal(1.0) );
	w1=wqr;
	Vector3 HQ1,HS,HR1;
	SplitArc(P0,Q1,w1,S,HQ1,HS,HR1,wqr);
	Pw(2)=Vector4(HS, Nreal(1.0) );
	Pw(1)=Vector4(wqr*HQ1,wqr);
	Pw(3)=Vector4(wqr*HR1,wqr);
	SplitArc(S,R1,w1,P2,HQ1,HS,HR1,wqr);
	Pw(6)=Vector4(HS, Nreal(1.0) );
	Pw(5)=Vector4(wqr*HQ1,wqr);
	Pw(7)=Vector4(wqr*HR1,wqr);

	for(int i=0; i<2; ++i)
	{
		U(i+3)=Nreal(0.25);
		U(i+5)=Nreal(0.5);
		U(i+7)=Nreal(0.75);
	}

	return curve_t(Pw,U,2,1);

}

template <class Nreal, class Nint>
void NurbsCurveTool< Nreal,Nint >::SplitArc(const Vector3& P0, const Vector3& P1, Nreal w1, const Vector3& P2, Vector3& Q1, Vector3& S, Vector3& R1, Nreal& wqr )
{
	if(w1!=0)
	{
		Q1=( P0+w1*P1 ) / (1+w1);
		R1=( w1*P1+P2 ) / (1+w1);
		S=Nreal(0.5) * (Q1+R1);
		wqr=Math::Sqrt( Nreal(0.5) * (1+w1) );
	}
	else
	{
		Q1=P0+P1;
		R1=P2+P1;
		S=Nreal(0.5) * (Q1+R1);
		wqr=Nreal(0.5) * Math::Sqrt( Nreal(2.0) );
	}
}

template <class Nreal, class Nint>
typename NurbsCurveTool< Nreal,Nint >::curve_t
NurbsCurveTool< Nreal,Nint >::MakeLine(const Vector3& P0, const Vector3& P1)
{
	control_poly_t Pw(2);
	Pw(0)=Vector4(P0,1.0);
	Pw(1)=Vector4(P1,1.0);

	vector_t U(4);

	U(0)=Nreal(0.0);
	U(1)=Nreal(0.0);
	U(2)=Nreal(1.0);
	U(3)=Nreal(1.0);

	return curve_t(Pw,U,1,1);
}


template <class Nreal, class Nint>
typename NurbsCurveTool< Nreal,Nint >::vector_t
NurbsCurveTool< Nreal,Nint >::MakeBezierKnotVec(Nint order)
{
	vector_t U( (order+1)*2 );
	Nint k=order+1;

	for(Nint i=0; i<k; ++i)
	{
		U(i)=Nreal(0.0);
		U(k+i)=Nreal(1.0);
	}

	return U;

}

