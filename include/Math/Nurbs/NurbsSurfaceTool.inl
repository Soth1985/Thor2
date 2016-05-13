
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::KnotInsertU(Nreal u,Nint r,surface_t& surface)
{
	
	if(r<=0)
		return;

	if(r>surface.Uorder)
		return;

	if( u<surface.U(0) || u>surface.U(surface.n+surface.Uorder) )
	{
		throw NurbsParameterOutOfRange();
	}

	Nint mp,nq,k,i,j,s,L,row;
	//Nreal alpha;

	k=surface.FindSpanMultU(u,s);
	L=k-s;

	if( (r+s)>surface.Uorder ) r=surface.Uorder-s;

	mp=surface.n+surface.Uorder+1;//high index of U vec
	nq=surface.n+r;//updated number of CV

	control_poly_t Qw;

	Qw.Realloc(nq+1,surface.m+1);

	vector_t UQ;
	UQ.Realloc(mp+r+1);
	DynArray <Nreal> Tmp( Max( surface.Uorder,surface.Vorder)+1 , Max( surface.Uorder,surface.Vorder)+1 );
	DynArray <Vector4,Shape1d> Rw ( Max(surface.Uorder,surface.Vorder)+1 );

	//Load knot vector
	for(i=0;i<=k;i++) UQ(i)=surface.U(i);
	for(i=1;i<=r;i++) UQ(k+i)=u;
	for(i=k+1;i<=mp;i++) UQ(i+r)=surface.U(i);
	//use Tmp to compute alphas
	for(j=1;j<=r;j++)
	{
		L=k-surface.Uorder+j;
		for(i=0;i<=surface.Uorder-j-s;i++)
		{
			Tmp(i,j)=( u - surface.U(L+i) )/( surface.U(i+k+1)-surface.U(L+i) );
		}
	}
	//compute new CV for each row

	//save unmodified CV
	for(row=0;row<=surface.m;row++)
	{
		for(i=0;i<=k-surface.Uorder;i++) Qw(i,row)=surface.P(i,row);//consider r=1 alpha=0
		for(i=k-s;i<=surface.n;i++) Qw(i+r,row)=surface.P(i,row);//alpha=1
		for(i=0;i<=surface.Uorder-s;i++) Rw(i)=surface.P(k-surface.Uorder+i,row);//need k-Uorder`s CV

		for(j=1;j<=r;j++)
		{
			L=k-surface.Uorder+j;
			for(i=0;i<=surface.Uorder-s-j;i++)
			{				
				Rw(i)=Rw(i+1)*Tmp(i,j) + Rw(i)*(1.0f-Tmp(i,j));
			}

			Qw(L,row)=Rw(0);
			Qw(k+r-j-s,row)=Rw(surface.Uorder-j-s);
		}

		for(i=L+1;i<k-s;i++)
			Qw(i,row)=Rw(i-L);
	}

	surface.P.Swap(Qw);
	surface.U.Swap(UQ);
	surface.n=nq;
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::KnotInsertV(Nreal v,Nint r,surface_t& surface)
{

	if(r<=0)
		return;

	if(r>surface.Vorder)
		return;

	if( v<surface.V(0) || v>surface.V(surface.m+surface.Vorder) )
	{
		throw NurbsParameterOutOfRange();
	}

	Nint mp,nq,k,i,j,s,L,row;
	//Nreal alpha;

	k=surface.FindSpanMultV(v,s);
	L=k-s;

	if( (r+s)>surface.Vorder ) r=surface.Vorder-s;

	mp=surface.m+surface.Vorder+1;//high index of U vec
	nq=surface.m+r;//updated number of CV

	control_poly_t Qw;
	Qw.Realloc(surface.n+1,nq+1);



	vector_t VQ;
	VQ.Realloc(mp+r+1);

	DynArray <Nreal> Tmp( Max( surface.Uorder,surface.Vorder)+1 , Max(surface.Uorder,surface.Vorder)+1 );
	DynArray <Vector4,Shape1d> Rw ( Max(surface.Uorder,surface.Vorder)+1 );

	//Load knot vector
	for(i=0;i<=k;i++) VQ(i)=surface.V(i);
	for(i=1;i<=r;i++) VQ(k+i)=v;
	for(i=k+1;i<=mp;i++) VQ(i+r)=surface.V(i);
	//use Tmp to compute alphas
	for(j=1;j<=r;j++)
	{
		L=k-surface.Vorder+j;
		for(i=0;i<=surface.Vorder-j-s;i++)
		{
			Tmp(i,j)=( v - surface.V(L+i) )/( surface.V(i+k+1)-surface.V(L+i) );
		}
	}
	//compute new CV for each row

	//save unmodified CV
	for(row=0;row<=surface.n;row++)
	{
		for(i=0;i<=k-surface.Vorder;i++) Qw(row,i)=surface.P(row,i);//consider r=1 alpha=0
		for(i=k-s;i<=surface.m;i++) Qw(row,i+r)=surface.P(row,i);//alpha=1
		for(i=0;i<=surface.Vorder-s;i++) Rw(i)=surface.P(row,k-surface.Vorder+i);//need k-Uorder`s CV

		for(j=1;j<=r;j++)
		{
			L=k-surface.Vorder+j;
			for(i=0;i<=surface.Vorder-s-j;i++)
			{				
				Rw(i)=Rw(i+1)*Tmp(i,j) + Rw(i)*(1.0f-Tmp(i,j));
			}

			Qw(row,L)=Rw(0);
			Qw(row,k+r-j-s)=Rw(surface.Uorder-j-s);
		}

		for(i=L+1;i<k-s;i++)
			Qw(row,i)=Rw(i-L);
	}

	surface.P.Swap(Qw);
	surface.V.Swap(VQ);
	surface.m=nq;

}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t 
NurbsSurfaceTool< Nreal,Nint >::SplitU(Nreal u,surface_t& surface)
{
	if( u<surface.U(0) || u>surface.U(n+surface.Uorder) )
	{
		throw NurbsParameterOutOfRange();
	}

	Nint s,k=surface.FindSpanMultU(u,s),i;
	SurfaceKnotInsertU(u,surface.Uorder-s,surface);

	//load knot vector
	vector_t resU;
	resU.Realloc(surface.U.c()-k+s);

	resU(0)=u;

	for(i=1;i<resU.c();i++)
	{
		resU(i)=surface.U(i+k-s);
	}
	
	surface.U.Realloc(k+1+surface.Uorder);
	
	for(i=1;i<=surface.Uorder;i++)
	{
		surface.U(k+i)=u;
	}

	//load CV
	control_poly_t resP;
	resP.Realloc(surface.n-k+1+s,surface.m+1);
	
	int row;

	for(row=0;row<=surface.m;row++)
	{
		resP(0,row)=surface.P(k-s,row);

		for(i=1;i<resP.r();i++)
		{
			resP(i,row)=surface.P(i+k-s,row);
		}
	}

	control_poly_t tP(k+1-s,surface.m+1);//temp CP	

	for(i=0;i<=k-s;i++)
	{
		for(row=0;row<=surface.m;row++)
		{
			tP(i,row)=surface.P(i,row);
		}
	}

	//tP.Copy(P,(k+1-s)*(m+1));

	surface.P.Swap(tP);
	surface.n=k-s;
	return surface_t(resP,resU,surface.V,surface.Uorder,surface.Vorder,surface.DUorder,surface.DVorder);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t 
NurbsSurfaceTool< Nreal,Nint >::SplitV(Nreal v,surface_t& surface)
{
	if( v<surface.V(0) || v>surface.V(surface.m+surface.Vorder) )
	{
		throw NurbsParameterOutOfRange();
	}

	Nint s,k=surface.FindSpanMultV(v,s),i;
	surface.KnotInsertV(v,surface.Vorder);


	//load knot vector
	vector_t resV;
	resV.Realloc(surface.V.c()-k+s);

	resV(0)=u;

	for(i=1;i<resV.c();i++)
	{
		resV(i)=surface.V(i+k-s);
	}

	surface.V.Realloc(k+1+surface.Vorder);
	
	for(i=1;i<=surface.Vorder;i++)
	{
		surface.V(k+i)=v;
	}

	//load CV
	control_poly_t resP;
	resP.Realloc(surface.n+1,surface.m-k+1+s);

	int row;

	for(row=0;row<=surface.n;row++)
	{
		resP(row,0)=surface.P(row,k-s);

		for(i=1;i<resP.c();i++)
		{
			resP(row,i)=surface.P(row,i+k-s);
		}
	}

	control_poly_t tP(surface.n+1,k+1-s);//temp CV
	tP.Realloc;

	for(i=0;i<=surface.n;i++)
	{
		for(row=0;row<=k-s;row++)
		{
			tP(i,row)=surface.P(i,row);
		}
	}

	surface.P.Swap(tP);
	surface.m=k-s;

	return surface_t(resP,surface.V,resV,surface.Uorder,surface.Vorder,surface.DUorder,surface.DVorder);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::KnotRefineU( const vector_t& x, surface_t& surface )
{
	if(x.Size()<1) return;
	Nint mm=surface.n+surface.Uorder+1;
	Nint r=x.c()-1;
	Nint a=surface.FindSpanU( x(0) );
	Nint b=surface.FindSpanU( x(r) ) +1;
	Nint i,j,k,l,ind,row;
	Nreal alfa;

	control_poly_t Qw( (surface.n+r+2), surface.m+1 );	
	vector_t Ubar(mm+r+2);

	for( row=0;row<=surface.m;row++)
	{
		for(j=0;j<=a-surface.Uorder;j++)
			Qw(j,row)=surface.P(j,row);

		for(j=b-1;j<=surface.n;j++)
			Qw(j+r+1,row)=surface.P(j,row);
	}
	for(j=0;j<=a;j++)
		Ubar(j)=surface.U(j);

	for(j=b+surface.Uorder;j<=mm;j++)
		Ubar(j+r+1)=surface.U(j);

	i=b+surface.Uorder-1;
	k=b+surface.Uorder+r;

	for(j=r;j>=0;j--)
	{
		while( ( x(j) <= surface.U(i) ) && ( i>a ) )
		{
			Ubar(k)=surface.U(i);
			for(row=0;row<=surface.m;row++)
				Qw(k-surface.Uorder-1,row)=surface.P(i-surface.Uorder-1,row);			
			k=k-1;
			i=i-1;
		}

		for(row=0;row<=surface.m;row++) 
			Qw(k-surface.Uorder-1,row)=Qw(k-surface.Uorder,row);

		for( l=1; l<=surface.Uorder; l++ )
		{
			ind=k-surface.Uorder+l;
			alfa=Ubar(k+l)-x(j);
			if( abs(alfa)==0.0 )
				for(row=0;row<=surface.m;row++) Qw(ind-1,row)=Qw(ind,row);
			else
			{
				alfa=alfa/( Ubar(k+l) - surface.U(i-surface.Uorder+l) );
				for(row=0;row<=surface.m;row++)
					Qw(ind-1,row)=alfa*Qw(ind-1,row) + (1.0-alfa)*Qw(ind,row);
			}
		}
		Ubar(k) = x(j);
		k=k-1;
	}

	surface.U.Swap( Ubar );
	surface.P.Swap( Qw );
	surface.n=surface.n+r+1;
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::KnotRefineV( const vector_t& x, surface_t& surface )
{
	if(x.Size()<1) return;
	Nint mm=surface.m+surface.Vorder+1;
	Nint r=x.c()-1;
	Nint a=surface.FindSpanV( x(0) );
	Nint b=surface.FindSpanV( x(r) ) +1;
	Nint i,j,k,l,ind,row;
	Nreal alfa;

	control_poly_t Qw( surface.n+1,(surface.m+r+2) );	
	vector_t Vbar(mm+r+2);

	for( row=0;row<=surface.n;row++)
	{
		for(j=0;j<=a-surface.Vorder;j++)
			Qw(row,j)=surface.P(row,j);

		for(j=b-1;j<=surface.m;j++)
			Qw(row,j+r+1)=surface.P(row,j);
	}
	for(j=0;j<=a;j++)
		Vbar(j)=surface.V(j);

	for(j=b+surface.Vorder;j<=mm;j++)
		Vbar(j+r+1)=surface.V(j);

	i=b+surface.Vorder-1;
	k=b+surface.Vorder+r;

	for(j=r;j>=0;j--)
	{
		while( ( x(j) <= surface.V(i) ) && ( i>a ) )
		{
			Vbar(k)=surface.V(i);
			for(row=0;row<=surface.n;row++)
				Qw(row,k-surface.Vorder-1)=surface.P(row,i-surface.Vorder-1);			
			k=k-1;
			i=i-1;
		}

		for(row=0;row<=surface.n;row++)
			Qw(row,k-surface.Vorder-1)=Qw(row,k-surface.Vorder);

		for( l=1; l<=surface.Vorder; l++ )
		{
			ind=k-surface.Vorder+l;
			alfa=Vbar(k+l)-x(j);
			if( abs(alfa)==0.0 )
				for(row=0;row<=surface.n;row++) Qw(row,ind-1)=Qw(row,ind);
			else
			{
				alfa=alfa/( Vbar(k+l) - surface.V(i-surface.Vorder+l) );
				for(row=0;row<=surface.n;row++)
					Qw(row,ind-1)=alfa*Qw(row,ind-1) + (1.0-alfa)*Qw(row,ind);
			}
		}
		Vbar(k) = x(j);
		k=k-1;
	}

	surface.V.Swap( Vbar );
	surface.P.Swap( Qw );
	surface.m=surface.m+r+1;
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t 
NurbsSurfaceTool< Nreal,Nint >::MakePlane( Nint spansU, Nint spansV, Nreal w, Nreal h, Nint uOrd, Nint vOrd )
{
	Nreal dw=w/vOrd;
	Nreal dh=h/uOrd;

	Vector4 Pinit( 0.0,0.0,0.0,1.0),vi(dw,0.0,0.0,0.0),vj(0.0,dh,0.0,0.0);
	Nint i,j;


	if( (spansU==0) || (spansV==0) )
		throw NurbsPlaneWithZeroSpan();

	//first create a bezier surface Uord x Vord
	typename surface_t::vector_t U( 2*(uOrd+1) );

	for(i=0;i<=uOrd;i++)
	{
		U(i)=0.0f;
	};

	for(i=uOrd+1;i<=2*uOrd+1;i++)
	{
		U(i)=1.0;
	};

	typename surface_t::vector_t V( 2*(vOrd+1) );
	
	for(j=0;j<=vOrd;j++)
	{
		V(j)=0.0f;
	};

	for(j=vOrd+1;j<=2*vOrd+1;j++)
	{
		V(j)=1.0;
	};

	control_poly_t P( uOrd+1,vOrd+1 );
	/////
	for(j=0;j<=vOrd;j++)
	{		
		for(i=0;i<=uOrd;i++)
		{
			P(i,j)=Pinit+i*vi;
		}
		Pinit+=vj;
	};

	//then refine surface knot vectors so that they become ([order+1] zeros).. du.. 2*du .. 3*du .... ([order+1] ones) 
	vector_t rU(spansU-1);
	vector_t rV(spansV-1);

	Nreal du=1.0/spansU;
	Nreal dv=1.0/spansV;


	for(i=1;i<spansU;i++)
		rU(i-1)=i*du;

	for(j=1;j<spansV;j++)
		rV(j-1)=j*dv;

	surface_t plane(P,U,V,uOrd,vOrd,1,1);

	if( spansU>1 )
		KnotRefineU( rU,plane );


	if( spansV>1 )
		KnotRefineV( rV,plane );

	return plane;

};

template <class Nreal, class Nint>
std::vector<typename  NurbsSurfaceTool< Nreal,Nint >::surface_t> 
NurbsSurfaceTool< Nreal,Nint >::DecomposeToBezierStripsU(const surface_t& surface)
{
	
	Nint a=surface.Uorder;
	Nint b=surface.Uorder+1;
	Nint nb=0;
	Nint mm=surface.n+surface.Uorder+1;
	vector_t alphas(surface.Uorder);
	vector_t resU( 2*(surface.Uorder+1) );
	control_poly_t resP(surface.Uorder+1,surface.m+1);
	control_poly_t resP1(surface.Uorder+1,surface.m+1);
	std::vector<surface_t> result;

	if( b<mm )
	{
		for(Nint i=0; i<=surface.Uorder; ++i)
		{
			for(Nint row=0; row<=surface.m; ++row)
				resP(i,row)=surface.P(i,row);
		}

		for(Nint i=0; i<=surface.Uorder; ++i)
			resU(i)=0.0;//a

		for(Nint i=surface.Uorder+1; i< 2*(surface.Uorder+1); ++i)
			resU(i)=1.0;//b
	}


	while( b<mm )
	{
		Nint i=b;

		while( ( b<mm ) && ( surface.U(b+1)==surface.U(b) ) )
			++b;

		Nint mult=b-i+1;

		if(mult<surface.Uorder)
		{
			Nreal numer=surface.U(b)-surface.U(a);

			for(Nint j=surface.Uorder; j>mult; --j)
				alphas(j-mult-1)=numer/( surface.U(a+j)-surface.U(a) );

			Nint r=surface.Uorder-mult;

			for(Nint j=1; j<=r; ++j)
			{
				Nint save=r-j;
				Nint s=mult+j;

				for(Nint k=surface.Uorder; k>=s; --k)
				{
					Nreal alpha=alphas(k-s);
					for(Nint row=0; row<=surface.m; ++row)
						resP(k,row)=alpha*resP(k,row)+(1.0-alpha)*resP(k-1,row);
				}

				if(b<mm)
				{
					for(Nint row=0; row<=surface.m; ++row)
						resP1(save,row)=resP(surface.Uorder,row);
				}
			}
		}

		result.push_back( surface_t(resP,resU,surface.V,surface.Uorder,surface.Vorder,surface.DUorder,surface.DVorder) );
		nb=nb+1;
		resP.Swap(resP1);

		if(b<mm)
		{
			for(Nint i=surface.Uorder-mult; i<=surface.Uorder; ++i)
			{
				for(Nint row=0; row<=surface.m; ++row)
					resP(i,row)=surface.P(b-surface.Uorder+i,row);
			}
			a=b;
			b=b+1;
		}
	}

	return result;
}

template <class Nreal, class Nint>
std::vector<typename  NurbsSurfaceTool< Nreal,Nint >::surface_t> 
NurbsSurfaceTool< Nreal,Nint >::DecomposeToBezierStripsV(const surface_t& surface)
{

	Nint a=surface.Vorder;
	Nint b=surface.Vorder+1;
	Nint nb=0;
	Nint mm=surface.m+surface.Vorder+1;
	vector_t alphas(surface.Vorder);
	vector_t resV( 2*(surface.Vorder+1) );
	control_poly_t resP(surface.n+1,surface.Vorder+1);
	control_poly_t resP1(surface.n+1,surface.Vorder+1);
	std::vector<surface_t> result;

	if( b<mm )
	{
		for(Nint i=0; i<=surface.Vorder; ++i)
		{
			for(Nint row=0; row<=surface.n; ++row)
				resP(row,i)=surface.P(row,i);
		}

		for(Nint i=0; i<=surface.Vorder; ++i)
			resV(i)=0.0;//a

		for(Nint i=surface.Vorder+1; i< 2*(surface.Vorder+1); ++i)
			resV(i)=1.0;//b
	}


	while( b<mm )
	{
		Nint i=b;

		while( ( b<mm ) && ( surface.V(b+1)==surface.V(b) ) )
			++b;

		Nint mult=b-i+1;

		if(mult<surface.Vorder)
		{
			Nreal numer=surface.V(b)-surface.V(a);

			for(Nint j=surface.Vorder; j>mult; --j)
				alphas(j-mult-1)=numer/( surface.V(a+j)-surface.V(a) );

			Nint r=surface.Vorder-mult;

			for(Nint j=1; j<=r; ++j)
			{
				Nint save=r-j;
				Nint s=mult+j;

				for(Nint k=surface.Vorder; k>=s; --k)
				{
					Nreal alpha=alphas(k-s);
					for(Nint row=0; row<=surface.n; ++row)
						resP(row,k)=alpha*resP(row,k)+(1.0-alpha)*resP(row,k-1);
				}

				if(b<mm)
				{
					for(Nint row=0; row<=surface.n; ++row)
						resP1(row,save)=resP(row,surface.Vorder);
				}
			}
		}

		result.push_back( surface_t(resP,surface.U,resV,surface.Uorder,surface.Vorder,surface.DUorder,surface.DVorder) );
		nb=nb+1;
		resP.Swap(resP1);

		if(b<mm)
		{
			for(Nint i=surface.Vorder-mult; i<=surface.Vorder; ++i)
			{
				for(Nint row=0; row<=surface.n; ++row)
					resP(row,i)=surface.P(row,b-surface.Uorder+i);
			}
			a=b;
			b=b+1;
		}
	}

	return result;
}

template <class Nreal, class Nint>
Nreal NurbsSurfaceTool< Nreal,Nint >::ComputeTolerance(const surface_t& surface)
{	
	Nreal wMin=std::numeric_limits<Nreal>::max();
	Nreal pMax=0.0;

	for(Nint i=0; i<=surface.n; ++i)
	{
		for(Nint j=0; j<=surface.m; ++j)
		{
			if( surface.P(i,j).w()<wMin )
				wMin=surface.P(i,j).w();

			Nreal temp=surface.P(i,j)*surface.P(i,j);

			if( temp>pMax )
				pMax=temp;
		}		
	}

	return wMin / ( 1+Math::Sqrt(pMax) );

}

template <class Nreal, class Nint>
Nint NurbsSurfaceTool< Nreal,Nint >::KnotRemoveU(Nreal u, Nint num, surface_t& surface, Nreal tol)
{
	Nint s,r=surface.FindSpanMultU(u,s);
	Nint t=0;
	Nint mm=surface.n+surface.Uorder+1;
	Nint ord=surface.Uorder+1;
	Nint fout=(2*r-s-surface.Uorder)/2;
	Nint last=r-s;
	Nint first=r-surface.Uorder;
	DynArray<Vector4,Shape1d> temp(2*surface.Uorder+1);
	Nreal TOL=tol*ComputeTolerance(surface);
	bool stop=false;

	for(t=0; t<num; ++t)
	{
		if(stop)
			break;

		Nint off=first-1;
		/*Nint i=first;
		Nint j=last;
		Nint ii=1;
		Nint jj=last-off;
		Nreal alfi=0.0,alfj=0.0;

		while( (j-i)>t )
		{
			alfi=( u-surface.U(i) ) / ( surface.U(i+ord+t)-surface.U(i) );
			alfj=( u-surface.U(j-t) ) / ( surface.U(j+ord)-surface.U(j-t) );
			i=i+1; ii=ii+1;
			j=j-1; jj=jj-1;
		}*/		

		for(Nint row=0; row<=surface.m; ++row)
		{
			//off=first-1;
			temp(0)=surface.P(off,row);
			temp(last+1-off)=surface.P(last+1,row);

			Nint i=first;
			Nint j=last;
			Nint ii=1;
			Nint jj=last-off;
			
			Nint remflag=0;

			while( (j-i)>t )
			{
				Nreal alfi=( u-surface.U(i) ) / ( surface.U(i+ord+t)-surface.U(i) );
				Nreal alfj=( u-surface.U(j-t) ) / ( surface.U(j+ord)-surface.U(j-t) );
				temp(ii)=( surface.P(i,row)-(1.0-alfi)*temp(ii-1) ) / alfi;
				temp(jj)=( surface.P(j,row)-alfj*temp(jj+1) ) / ( 1.0-alfj );
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
				Nreal alfi=( u-surface.U(i) ) / ( surface.U(i+ord+t)-surface.U(i) );

				if( surface.P(i,row).Distance( alfi*temp(ii+t+1)+(1.0-alfi)*temp(ii-1) ) <=TOL )
					remflag=1;
			}

			if(remflag==0)
			{
				stop=true;
				break;
			}
			else
			{
				i=first; j=last;

				while( (j-i)>t )
				{
					surface.P(i,row)=temp(i-off);
					surface.P(j,row)=temp(j-off);
					i=i+1; j=j-1;
				}
			}			
		}
		first=first-1; last=last+1;
	}

	if(t==0)
		return t;

	for(Nint k=r+1; k<=mm; ++k)
		surface.U(k-t)=surface.U(k);

	surface.U.Realloc(surface.U.c()-t);

	Nint j=fout;
	Nint i=j;

	for(Nint k=1; k<t; ++k)
	{
		if( (k%2)==1 )
			i=i+1;
		else
			j=j-1;
	}

	Nint jj=j;

	for(Nint row=0; row<=surface.m; ++row)
	{
		j=jj;
		for(Nint k=i+1; k<=surface.n; k++)
		{
			surface.P(j,row)=surface.P(k,row);
			j=j+1;
		}
	}

	control_poly_t tP(surface.n-t+1,surface.m+1);

	for(Nint k=0; k<tP.r(); ++k)
	{
		for(Nint m=0; m<=surface.m; ++m)
			tP(k,m)=surface.P(k,m);
	}

	surface.P.Swap(tP);
	surface.n-=t;
	return t;
}

template <class Nreal, class Nint>
Nint NurbsSurfaceTool< Nreal,Nint >::KnotRemoveV(Nreal v, Nint num, surface_t& surface, Nreal tol)
{
	Nint s,r=surface.FindSpanMultV(v,s);
	Nint t=0;
	Nint mm=surface.m+surface.Vorder+1;
	Nint ord=surface.Vorder+1;
	Nint fout=(2*r-s-surface.Vorder)/2;
	Nint last=r-s;
	Nint first=r-surface.Vorder;
	DynArray<Vector4,Shape1d> temp(2*surface.Vorder+1);
	Nreal TOL=tol*ComputeTolerance(surface);
	//DynArray<Nreal,Shape1d> alfi(num),alfj(num);
	bool stop=false;

	for(t=0; t<num; ++t)
	{
		if(stop)
			break;

		Nint off=first-1;
		/*
		Nint i=first;
		Nint j=last;
		Nint ii=1;
		Nint jj=last-off;		

		while( (j-i)>t )
		{
			alfi(j-i-1)=( v-surface.V(i) ) / ( surface.V(i+ord+t)-surface.V(i) );
			alfj(j-i-1)=( v-surface.V(j-t) ) / ( surface.V(j+ord)-surface.V(j-t) );
			i=i+1; ii=ii+1;
			j=j-1; jj=jj-1;
		}

		Nreal alfii;

		if( (j-i)>=t )
			alfii=( v-surface.V(i) ) / ( surface.V(i+ord+t)-surface.V(i) );*/

		for(Nint row=0; row<=surface.n; ++row)
		{
			//off=first-1;
			temp(0)=surface.P(row,off);
			temp(last+1-off)=surface.P(row,last+1);

			Nint i=first;
			Nint j=last;
			Nint ii=1;
			Nint jj=last-off;

			Nint remflag=0;

			while( (j-i)>t )
			{
				Nreal alfi=( v-surface.V(i) ) / ( surface.V(i+ord+t)-surface.V(i) );
				Nreal alfj=( v-surface.V(j-t) ) / ( surface.V(j+ord)-surface.V(j-t) );
				temp(ii)=( surface.P(row,i)-( 1.0-alfi )*temp(ii-1) ) / alfi;
				temp(jj)=( surface.P(row,j)-alfj*temp(jj+1) ) / ( 1.0-alfj );
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
				Nreal alfii=( v-surface.V(i) ) / ( surface.V(i+ord+t)-surface.V(i) );

				if( surface.P(row,i).Distance( alfii*temp(ii+t+1)+(1.0-alfii)*temp(ii-1) ) <=TOL )
					remflag=1;
			}

			if(remflag==0)
			{
				stop=true;
				break;
			}
			else
			{
				i=first; j=last;

				while( (j-i)>t )
				{
					surface.P(row,i)=temp(i-off);
					surface.P(row,j)=temp(j-off);
					i=i+1; j=j-1;
				}
			}			
		}
		first=first-1; last=last+1;
	}

	if(t==0)
		return t;

	for(Nint k=r+1; k<=mm; ++k)
		surface.V(k-t)=surface.V(k);

	surface.V.Realloc(surface.V.c()-t);

	Nint j=fout;
	Nint i=j;

	for(Nint k=1; k<t; ++k)
	{
		if( (k%2)==1 )
			i=i+1;
		else
			j=j-1;
	}

	Nint jj=j;

	for(Nint row=0; row<=surface.n; ++row)
	{
		j=jj;
		for(Nint k=i+1; k<=surface.m; k++)
		{
			surface.P(row,j)=surface.P(row,k);
			j=j+1;
		}
	}

	control_poly_t tP(surface.n+1,surface.m-t+1);

	for(Nint k=0; k<tP.c(); ++k)
	{
		for(Nint m=0; m<=surface.n; ++m)
			tP(m,k)=surface.P(m,k);
	}

	surface.P.Swap(tP);
	surface.m-=t;
	return t;
}

template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::DegreeElevateU(Nint t, surface_t& surface)
{
	Nint mm=surface.n+surface.Uorder+1;
	Nint ph=surface.Uorder+t;
	Nint ph2=ph/2;
	DynArray<Nreal> bezalfs(surface.Uorder+t+1,surface.Uorder+1);
	bezalfs(0,0)=1.0;
	bezalfs(ph,surface.Uorder)=1.0;

	Nint newN=surface.n*(t+1)+t+1;//new number of control points
	Nint newM=newN+surface.Uorder+t+1;//new high index
	//

	for(Nint i=1; i<=ph2; ++i)
	{
		Nreal inv=1.0/Math::Bin<Nreal>(ph,i);
		Nint mpi=Min(surface.Uorder,i);

		for(Nint j=Max(0,i-t); j<=mpi; ++j)
			bezalfs(i,j)=inv*Math::Bin<Nreal>(surface.Uorder,j)*Math::Bin<Nreal>(t,i-j);
	}

	for(Nint i=ph2+1; i<=ph-1; ++i)
	{
		Nint mpi=Min(surface.Uorder,i);

		for(Nint j=Max(0,i-t); j<=mpi; ++j)
			bezalfs(i,j)=bezalfs(ph-i,surface.Uorder-j);
	}

	Nint mh=ph;
	Nint kind=ph+1;
	Nint r =-1;
	Nint a=surface.Uorder;
	Nint b=surface.Uorder+1;
	Nint cind=1;
	Nreal ua=surface.U(0);
	control_poly_t Qw(newN,surface.m+1);
	vector_t Uh(newM+1);

	for(Nint row=0; row<=surface.m; ++row)
		Qw(0,row)=surface.P(0,row);

	for(Nint i=0; i<=ph; ++i)
		Uh(i)=ua;

	control_poly_t bpts(surface.Uorder+1,surface.m+1);
	control_poly_t Nextbpts(surface.Uorder-1,surface.m+1);
	control_poly_t ebpts(surface.Uorder+t+1,surface.m+1);
	vector_t alfs(surface.Uorder-1);


	//first bezier segment
	for(Nint i=0; i<=surface.Uorder; ++i)
		for(Nint j=0; j<=surface.m; ++j)
			bpts(i,j)=surface.P(i,j);

	while( b<mm )
	{
		Nint i=b;

		while( (b<mm) && ( surface.U(b)==surface.U(b+1) ) )
			b=b+1;

		Nint mul=b-i+1;
		mh=mh+mul+t;
		Nreal ub=surface.U(b);
		Nint oldr=r;
		r=surface.Uorder-mul;
		//insert u(b) r times
		Nint lbz=0;
		if(oldr>0)
			lbz=(oldr+2)/2;
		else
			lbz=1;

		Nint rbz=0;
		if(r>0)
			rbz=ph-(r+1)/2;
		else
			rbz=ph;

		for(Nint row=0; row<=surface.m; ++row)
		{

			if(r>0)
			{
				Nreal numer=ub-ua;				

				for(Nint k=surface.Uorder; k>mul; --k)
					alfs(k-mul-1)=numer / (surface.U(a+k)-ua);

				for(Nint j=1; j<=r; ++j)
				{
					Nint save=r-j;
					Nint s=mul+j;
					for(Nint k=surface.Uorder; k>=s; --k)
					{
						bpts(k,row)=alfs(k-s)*bpts(k,row)+(1.0-alfs(k-s))*bpts(k-1,row);
					}

					Nextbpts(save,row)=bpts(surface.Uorder,row);
				}
			}//end of insert knot

			

			for(int i=lbz; i<=ph; ++i)
			{
				ebpts(i,row)=Vector4(0.0,0.0,0.0,0.0);
				Nint mpi=Min(surface.Uorder,i);

				for(Nint j=Max(0,i-t); j<=mpi; ++j)
					ebpts(i,row)=ebpts(i,row)+bezalfs(i,j)*bpts(j,row);
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
							Qw(i,row)=alf*Qw(i,row) + (1.0-alf)*Qw(i-1,row);
						}
						if(j>=lbz)
						{
							if( (j-tr)<=(kind-ph+oldr) )
							{
								Nreal gam=( ub-Uh(j-tr) ) / den;
								ebpts(kj,row)=gam*ebpts(kj,row)+(1.0-gam)*ebpts(kj+1,row);
							}
							else
								ebpts(kj,row)=bet*ebpts(kj,row)+(1.0-bet)*ebpts(kj+1,row);
						}

						i=i+1; j=j-1; kj=kj-1;
					}

					first=first-1; last=last+1;
				}
			}			
		}

		if( a!=surface.Uorder )
		{
			for( Nint i=0; i<(ph-oldr); ++i )
			{
				Uh(kind)=ua;
				kind=kind+1;
			}
		}

		for(Nint j=lbz; j<=rbz; ++j)
		{
			for(Nint row=0; row<=surface.m; ++row)
				Qw(cind,row)=ebpts(j,row);
			
			cind=cind+1;
		}

		if(b<mm)
		{
			for(Nint j=0; j<r; ++j)
				for(Nint row=0; row<=surface.m; ++row)
					bpts(j,row)=Nextbpts(j,row);

			for(Nint j=r; j<=surface.Uorder; ++j)
				for(Nint row=0; row<=surface.m; ++row)
					bpts(j,row)=surface.P(b-surface.Uorder+j,row);

			a=b; b=b+1; ua=ub;
		}
		else
		{
			for(Nint i=0; i<=ph; ++i)
				Uh(kind+i)=ub;
		}	
	}

	
	Uh.Realloc(mh-ph+surface.Uorder+t+1);
	surface.P.Realloc(mh-ph, surface.m+1);

	for(unsigned int i=0; i<(mh-ph); ++i)
	{
		for(unsigned int j=0; j<=surface.m; ++j)
		{
			surface.P(i,j)=Qw(i,j);
		}
	}

	surface.n=mh-ph-1;
	surface.Refresh(surface.Uorder+t,surface.Vorder,surface.DUorder,surface.DVorder);
	surface.U.Swap(Uh);
}

template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::DegreeElevateV(Nint t, surface_t& surface)
{
	Nint mm=surface.m+surface.Vorder+1;
	Nint ph=surface.Vorder+t;
	Nint ph2=ph/2;
	DynArray<Nreal> bezalfs(surface.Vorder+t+1,surface.Vorder+1);
	bezalfs(0,0)=1.0;
	bezalfs(ph,surface.Vorder)=1.0;

	Nint newN=surface.m*(t+1)+t+1;//new number of control points
	Nint newM=newN+surface.Vorder+t+1;//new high index
	//

	for(Nint i=1; i<=ph2; ++i)
	{
		Nreal inv=1.0/Math::Bin<Nreal>(ph,i);
		Nint mpi=Min(surface.Vorder,i);

		for(Nint j=Max(0,i-t); j<=mpi; ++j)
			bezalfs(i,j)=inv*Math::Bin<Nreal>(surface.Vorder,j)*Math::Bin<Nreal>(t,i-j);
	}

	for(Nint i=ph2+1; i<=ph-1; ++i)
	{
		Nint mpi=Min(surface.Vorder,i);

		for(Nint j=Max(0,i-t); j<=mpi; ++j)
			bezalfs(i,j)=bezalfs(ph-i,surface.Vorder-j);
	}

	Nint mh=ph;
	Nint kind=ph+1;
	Nint r =-1;
	Nint a=surface.Vorder;
	Nint b=surface.Vorder+1;
	Nint cind=1;
	Nreal ua=surface.V(0);
	control_poly_t Qw(surface.n+1,newN);
	vector_t Uh(newM+1);

	for(Nint row=0; row<=surface.n; ++row)
		Qw(row,0)=surface.P(row,0);

	for(Nint i=0; i<=ph; ++i)
		Uh(i)=ua;

	control_poly_t bpts(surface.n+1,surface.Vorder+1);
	control_poly_t Nextbpts(surface.n+1,surface.Vorder-1);
	control_poly_t ebpts(surface.n+1,surface.Vorder+t+1);
	vector_t alfs(surface.Vorder-1);


	//first bezier segment
	for(Nint i=0; i<=surface.Vorder; ++i)
		for(Nint j=0; j<=surface.n; ++j)
			bpts(j,i)=surface.P(j,i);

	while( b<mm )
	{
		Nint i=b;

		while( (b<mm) && ( surface.V(b)==surface.V(b+1) ) )
			b=b+1;

		Nint mul=b-i+1;
		mh=mh+mul+t;
		Nreal ub=surface.V(b);
		Nint oldr=r;
		r=surface.Vorder-mul;
		//insert u(b) r times
		Nint lbz=0;
		if(oldr>0)
			lbz=(oldr+2)/2;
		else
			lbz=1;

		Nint rbz=0;
		if(r>0)
			rbz=ph-(r+1)/2;
		else
			rbz=ph;

		for(Nint row=0; row<=surface.n; ++row)
		{

			if(r>0)
			{
				Nreal numer=ub-ua;				

				for(Nint k=surface.Vorder; k>mul; --k)
					alfs(k-mul-1)=numer / (surface.V(a+k)-ua);

				for(Nint j=1; j<=r; ++j)
				{
					Nint save=r-j;
					Nint s=mul+j;
					for(Nint k=surface.Vorder; k>=s; --k)
					{
						bpts(row,k)=alfs(k-s)*bpts(row,k)+(1.0-alfs(k-s))*bpts(row,k-1);
					}

					Nextbpts(row,save)=bpts(row,surface.Vorder);
				}
			}//end of insert knot



			for(int i=lbz; i<=ph; ++i)
			{
				ebpts(row,i)=Vector4(0.0,0.0,0.0,0.0);
				Nint mpi=Min(surface.Vorder,i);

				for(Nint j=Max(0,i-t); j<=mpi; ++j)
					ebpts(row,i)=ebpts(row,i)+bezalfs(i,j)*bpts(row,j);
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
							Qw(row,i)=alf*Qw(row,i) + (1.0-alf)*Qw(row,i-1);
						}
						if(j>=lbz)
						{
							if( (j-tr)<=(kind-ph+oldr) )
							{
								Nreal gam=( ub-Uh(j-tr) ) / den;
								ebpts(row,kj)=gam*ebpts(row,kj)+(1.0-gam)*ebpts(row,kj+1);
							}
							else
								ebpts(row,kj)=bet*ebpts(row,kj)+(1.0-bet)*ebpts(row,kj+1);
						}

						i=i+1; j=j-1; kj=kj-1;
					}

					first=first-1; last=last+1;
				}
			}			
		}

		if( a!=surface.Vorder )
		{
			for( Nint i=0; i<(ph-oldr); ++i )
			{
				Uh(kind)=ua;
				kind=kind+1;
			}
		}

		for(Nint j=lbz; j<=rbz; ++j)
		{
			for(Nint row=0; row<=surface.n; ++row)
				Qw(row,cind)=ebpts(row,j);

			cind=cind+1;
		}

		if(b<mm)
		{
			for(Nint j=0; j<r; ++j)
				for(Nint row=0; row<=surface.n; ++row)
					bpts(row,j)=Nextbpts(row,j);

			for(Nint j=r; j<=surface.Vorder; ++j)
				for(Nint row=0; row<=surface.n; ++row)
					bpts(row,j)=surface.P(row,b-surface.Vorder+j);

			a=b; b=b+1; ua=ub;
		}
		else
		{
			for(Nint i=0; i<=ph; ++i)
				Uh(kind+i)=ub;
		}	
	}


	Uh.Realloc(mh-ph+surface.Vorder+t+1);
	surface.P.Realloc(surface.n+1,mh-ph);

	for(unsigned int i=0; i<(mh-ph); ++i)
	{
		for(unsigned int j=0; j<=surface.n; ++j)
		{
			surface.P(j,i)=Qw(j,i);
		}
	}

	surface.m=mh-ph-1;
	surface.Refresh(surface.Uorder,surface.Vorder+t,surface.DUorder,surface.DVorder);
	surface.V.Swap(Uh);
	
}

template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::BezDegreeReduceU(const control_poly_t& bpts,control_poly_t& rbpts,Nreal& maxErr,Nint row)
{
	Nint p=rbpts.r();
	Nint r=(p-1)/2;

	rbpts(0,row)=bpts(0,row);
	rbpts(p-1,row)=bpts(p,row);
		

	
	if( (p%2)==0 )//even
	{
		for(int i=1; i<=r; ++i)
		{
			Nreal alfa=Nreal(i)/Nreal(p);
			rbpts(i,row)= ( bpts(i,row)-alfa*rbpts(i-1,row) ) / (1-alfa);
		}

		for(int i=r+1; i<=(p-2); ++i)
		{
			Nreal alfa=Nreal( (i+1) )/ Nreal( p );
			rbpts(i,row)= ( bpts(i+1,row)-(1.0-alfa)*rbpts(i+1,row) ) / alfa;
		}

		Vector4 len=bpts(r+1,row)-0.5*( rbpts(r,row)+rbpts(r+1,row) );
		maxErr=len.Length();

	}
	else//odd
	{
		for(int i=1; i<=(r-1); ++i)
		{
			Nreal alfa=Nreal(i)/Nreal(p);
			rbpts(i,row)= ( bpts(i,row)-alfa*rbpts(i-1,row) ) / (1-alfa);
		}

		for(int i=r+1; i<=(p-2); ++i)
		{
			Nreal alfa=Nreal( (i+1) )/ Nreal( p );
			rbpts(i,row)= ( bpts(i+1,row)-(1.0-alfa)*rbpts(i+1,row) ) / alfa;
		}

		Nreal alfar=Nreal(r)/Nreal(p);
		Vector4 Pl=( bpts(r,row)-alfar*rbpts(r-1,row) ) / (1-alfar);
		alfar=Nreal( (r+1) )/ Nreal( p );
		Vector4 Pr=( bpts(r+1,row)-(1.0-alfar)*rbpts(r+1,row) ) / alfar;
		rbpts(r,row)=0.5*(Pl+Pr);

		Vector4 len=Pl-Pr;
		maxErr=len.Length();
	}
}

template <class Nreal, class Nint>
bool NurbsSurfaceTool< Nreal,Nint >::DegreeReduceU(surface_t& surface, Nreal tol)
{
	Nint ph=surface.Uorder-1;
	Nint mh=ph;
	Nint kind=ph+1;
	Nint r=-1;
	Nint a=surface.Uorder;
	Nint b=surface.Uorder+1;
	Nint cind=1;
	Nint mult=surface.Uorder;
	Nint mm=surface.n+surface.Uorder+1;
	control_poly_t Pw(surface.n,surface.m+1);
	vector_t Uh( surface.U.c()-2 );
	control_poly_t bpts(surface.Uorder+1,surface.m+1);
	control_poly_t Nextbpts(surface.Uorder-1,surface.m+1);
	control_poly_t rbpts(surface.Uorder,surface.m+1);
	vector_t alphas(surface.Uorder-1);
	vector_t e(mm);
	Nreal TOL=tol*ComputeTolerance(surface);

	for(Nint row=0; row<=surface.m; ++row)
		Pw(0,row)=surface.P(0,row);

	for(Nint i=0; i<=ph; ++i)
		Uh(i)=surface.U(0);

	for(Nint i=0; i<=surface.Uorder; ++i)
		for(Nint row=0; row<=surface.m; ++row)
			bpts(i,row)=surface.P(i,row);

	for(Nint i=0; i<mm; ++i)
		e(i)=0.0;

	while( b<mm )
	{
		Nint i=b;

		while( (b<mm) && ( surface.U(b)==surface.U(b+1) ) )
			b=b+1;

		mult=b-i+1;
		mh=mh+mult-1;
		Nint oldr=r;
		r=surface.Uorder-mult;

		Nint lbz=0;
		if(oldr>0)
			lbz=(oldr+2)/2;
		else
			lbz=1;

		for(Nint row=0; row<=surface.m; ++row)
		{

			if(r>0)
			{
				Nreal numer=surface.U(b)-surface.U(a);

				for(Nint k=surface.Uorder; k>=mult; --k)
					alphas(k-mult-1)=numer/( surface.U(a+k)-surface.U(a) );

				for(Nint j=1; j<=r; ++j)
				{
					Nint save=r-j;
					Nint s=mult+j;

					for(Nint k=surface.Uorder; k>=s; --k)
						bpts(k,row)=alphas(k-s)*bpts(k,row)+(1.0-alphas(k-s))*bpts(k-1,row);

					Nextbpts(save,row)=bpts(surface.Uorder,row);
				}
			}

			Nreal maxErr=0.0;
			BezDegreeReduceU(bpts,rbpts,maxErr,row);
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
						Nreal alfa=( surface.U(a)-Uh(i-1) ) / ( surface.U(b)-Uh(i-1) );
						Nreal beta=( surface.U(a)-Uh(j-k-1) ) / ( surface.U(b)-Uh(j-k-1) );
						Pw(i-1,row)=( Pw(i-1,row)-(1.0-alfa)*Pw(i-2,row) ) / alfa;
						rbpts(kj,row)=( rbpts(kj,row)-beta*rbpts(kj+1,row) ) / (1.0-beta);
						++i; --j; --kj;
					}

					Nreal Br=0.0;
					if( (j-i)<k )
						Br=Pw(i-2,row).Distance(rbpts(kj+1,row));
					else
					{
						Nreal delta=( surface.U(a)-Uh(i-1) ) / ( surface.U(b)-Uh(i-1) );
						Vector4 A=delta*rbpts(kj+1,row)+(1.0-delta)*Pw(i-2,row);
						Br=Pw(i-1,row).Distance(A);
					}

					Nint K=a+oldr-k;
					Nint q=(2*surface.Uorder-k+1)/2;
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
		}

		if(a!=surface.Uorder)
		{
			for(Nint i=0; i<(ph-oldr); ++i)
			{
				Uh(kind)=surface.U(a);
				++kind;
			}
		}

		for(Nint i=lbz; i<=ph; ++i)
		{
			for(Nint row=0; row<=surface.m; ++row)
				Pw(cind,row)=rbpts(i,row);
			++cind;
		}

		if(b<mm)
		{
			for(Nint i=0; i<r; ++i)
				for(Nint row=0; row<=surface.m; ++row)
					bpts(i,row)=Nextbpts(i,row);

			for(Nint i=r; i<=surface.Uorder; ++i)
				for(Nint row=0; row<=surface.m; ++row)
					bpts(i,row)=surface.P(b-surface.Uorder+i,row);

			a=b;
			++b;
		}
		else
		{
			for(Nint i=0; i<=ph; ++i)
				Uh(kind+i)=surface.U(b);
		}
	}
	surface.n=mh-ph-1;
	surface.Refresh(surface.Uorder-1,surface.Vorder,surface.DUorder,surface.DVorder);
	surface.P.Swap(Pw);
	surface.U.Swap(Uh);
	return true;
}

template <class Nreal, class Nint>
void NurbsSurfaceTool< Nreal,Nint >::BezDegreeReduceV(const control_poly_t& bpts,control_poly_t& rbpts,Nreal& maxErr,Nint row)
{
	Nint p=rbpts.c();
	Nint r=(p-1)/2;

	rbpts(row,0)=bpts(row,0);
	rbpts(row,p-1)=bpts(row,p);



	if( (p%2)==0 )//even
	{
		for(int i=1; i<=r; ++i)
		{
			Nreal alfa=Nreal(i)/Nreal(p);
			rbpts(row,i)= ( bpts(row,i)-alfa*rbpts(row,i-1) ) / (1-alfa);
		}

		for(int i=r+1; i<=(p-2); ++i)
		{
			Nreal alfa=Nreal( (i+1) )/ Nreal( p );
			rbpts(row,i)= ( bpts(row,i+1)-(1.0-alfa)*rbpts(row,i+1) ) / alfa;
		}

		Vector4 len=bpts(row,r+1)-0.5*( rbpts(row,r)+rbpts(row,r+1) );
		maxErr=len.Length();

	}
	else//odd
	{
		for(int i=1; i<=(r-1); ++i)
		{
			Nreal alfa=Nreal(i)/Nreal(p);
			rbpts(row,i)= ( bpts(row,i)-alfa*rbpts(row,i-1) ) / (1-alfa);
		}

		for(int i=r+1; i<=(p-2); ++i)
		{
			Nreal alfa=Nreal( (i+1) )/ Nreal( p );
			rbpts(row,i)= ( bpts(row,i+1)-(1.0-alfa)*rbpts(row,i+1) ) / alfa;
		}

		Nreal alfar=Nreal(r)/Nreal(p);
		Vector4 Pl=( bpts(row,r)-alfar*rbpts(row,r-1) ) / (1-alfar);
		alfar=Nreal( (r+1) )/ Nreal( p );
		Vector4 Pr=( bpts(row,r+1)-(1.0-alfar)*rbpts(row,r+1) ) / alfar;
		rbpts(row,r)=0.5*(Pl+Pr);

		Vector4 len=Pl-Pr;
		maxErr=len.Length();
	}
}

template <class Nreal, class Nint>
bool NurbsSurfaceTool< Nreal,Nint >::DegreeReduceV(surface_t& surface, Nreal tol)
{
	Nint ph=surface.Vorder-1;
	Nint mh=ph;
	Nint kind=ph+1;
	Nint r=-1;
	Nint a=surface.Vorder;
	Nint b=surface.Vorder+1;
	Nint cind=1;
	Nint mult=surface.Vorder;
	Nint mm=surface.m+surface.Vorder+1;
	control_poly_t Pw(surface.n+1,surface.m);
	vector_t Vh( surface.V.c()-2 );
	control_poly_t bpts(surface.n+1,surface.Vorder+1);
	control_poly_t Nextbpts(surface.n+1,surface.Vorder-1);
	control_poly_t rbpts(surface.n+1,surface.Vorder);
	vector_t alphas(surface.Vorder-1);
	vector_t e(mm);
	Nreal TOL=tol*ComputeTolerance(surface);

	for(Nint row=0; row<=surface.n; ++row)
		Pw(row,0)=surface.P(row,0);

	for(Nint i=0; i<=ph; ++i)
		Vh(i)=surface.V(0);

	for(Nint i=0; i<=surface.Vorder; ++i)
		for(Nint row=0; row<=surface.n; ++row)
			bpts(row,i)=surface.P(row,i);

	for(Nint i=0; i<mm; ++i)
		e(i)=0.0;

	while( b<mm )
	{
		Nint i=b;

		while( (b<mm) && ( surface.V(b)==surface.V(b+1) ) )
			b=b+1;

		mult=b-i+1;
		mh=mh+mult-1;
		Nint oldr=r;
		r=surface.Vorder-mult;

		Nint lbz=0;
		if(oldr>0)
			lbz=(oldr+2)/2;
		else
			lbz=1;

		for(Nint row=0; row<=surface.n; ++row)
		{

			if(r>0)
			{
				Nreal numer=surface.V(b)-surface.V(a);

				for(Nint k=surface.Vorder; k>=mult; --k)
					alphas(k-mult-1)=numer/( surface.V(a+k)-surface.V(a) );

				for(Nint j=1; j<=r; ++j)
				{
					Nint save=r-j;
					Nint s=mult+j;

					for(Nint k=surface.Vorder; k>=s; --k)
						bpts(row,k)=alphas(k-s)*bpts(row,k)+(1.0-alphas(k-s))*bpts(row,k-1);

					Nextbpts(row,save)=bpts(row,surface.Vorder);
				}
			}

			Nreal maxErr=0.0;
			BezDegreeReduceV(bpts,rbpts,maxErr,row);
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
						Nreal alfa=( surface.V(a)-Vh(i-1) ) / ( surface.V(b)-Vh(i-1) );
						Nreal beta=( surface.V(a)-Vh(j-k-1) ) / ( surface.V(b)-Vh(j-k-1) );
						Pw(row,i-1)=( Pw(row,i-1)-(1.0-alfa)*Pw(row,i-2) ) / alfa;
						rbpts(row,kj)=( rbpts(row,kj)-beta*rbpts(row,kj+1) ) / (1.0-beta);
						++i; --j; --kj;
					}

					Nreal Br=0.0;
					if( (j-i)<k )
						Br=Pw(row,i-2).Distance(rbpts(row,kj+1));
					else
					{
						Nreal delta=( surface.V(a)-Vh(i-1) ) / ( surface.V(b)-Vh(i-1) );
						Vector4 A=delta*rbpts(row,kj+1)+(1.0-delta)*Pw(row,i-2);
						Br=Pw(row,i-1).Distance(A);
					}

					Nint K=a+oldr-k;
					Nint q=(2*surface.Vorder-k+1)/2;
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
		}

		if(a!=surface.Vorder)
		{
			for(Nint i=0; i<(ph-oldr); ++i)
			{
				Vh(kind)=surface.V(a);
				++kind;
			}
		}

		for(Nint i=lbz; i<=ph; ++i)
		{
			for(Nint row=0; row<=surface.n; ++row)
				Pw(row,cind)=rbpts(row,i);
			++cind;
		}

		if(b<mm)
		{
			for(Nint i=0; i<r; ++i)
				for(Nint row=0; row<=surface.n; ++row)
					bpts(row,i)=Nextbpts(row,i);

			for(Nint i=r; i<=surface.Vorder; ++i)
				for(Nint row=0; row<=surface.n; ++row)
					bpts(row,i)=surface.P(row,b-surface.Vorder+i);

			a=b;
			++b;
		}
		else
		{
			for(Nint i=0; i<=ph; ++i)
				Vh(kind+i)=surface.V(b);
		}
	}
	surface.m=mh-ph-1;
	surface.Refresh(surface.Uorder,surface.Vorder-1,surface.DUorder,surface.DVorder);
	surface.P.Swap(Pw);
	surface.V.Swap(Vh);
	return true;
}


template <class Nreal, class Nint>
GeometryPtr NurbsSurfaceTool< Nreal,Nint >::MakeControlPolygonGeometry(const control_poly_t& cp, const GpuDevicePtr& device, const EffectPtr& effect, eGeometryTopology::Val topology, const std::string& name)
{
	assert(device!=0 &&"Null geometry pointer");
	assert(effect!=0 &&"Null effect pointer");

	IndicesPtr ind=device->CreateIndices(topology,0);
	GeometryPtr geo=Geometry::Create(device,effect,ind);

	//TODO: default effect
	//generate triangles
	//GeometryChunkPtr chunk;
	AttributePtr pos;
	//AttributePtr N;
	Nint numTris=2*(cp.r()-1)*(cp.c()-1);
	Nint numVerts=cp.r()*cp.c();
	Nint numLinesU=(cp.r()-1)*cp.c();
	Nint numLinesV=cp.r()*(cp.c()-1);
	std::vector<IIndices::value_type> indices;

	if(topology==eGeometryTopology::Triangles)
		indices.resize(numTris*3);

	if(topology==eGeometryTopology::Lines)
		indices.resize( (numLinesU+numLinesV)*2 );

	std::vector<AttribNames::P_t::value_type> positions;
	//std::vector<AttribNames::N_t::value_type> normals;

	//unsigned int gP=bitfield & genP;
	//unsigned int gN=(bitfield & genN);

	//if(gP)
	//{
		pos=geo->NewAttribute(AttribNames::P.name, AttribQuery(AttribNames::P.get_type_id(),AttribNames::P.semantic) );
		positions.resize(numVerts);
	//}

	/*if(gN)
	{
		N=chunk->NewAttribute(AttribNames::N.name, AttribNames::N.type_id );
		normals.resize(numVerts);
	}*/

	unsigned int k=0;


	for(unsigned int i=0; i<cp.r();++i)
	{
		for(unsigned int j=0; j<cp.c(); ++j )
		{
			positions[k]=cp(i,j)/cp(i,j).w();//pij
			++k;			
		}
	}

	unsigned int c=cp.c();
	k=0;

	if(topology==eGeometryTopology::Triangles)
	{
		for(unsigned int i=0; i<(cp.r()-1);++i)
		{
			for(unsigned int j=0; j<(cp.c()-1); ++j )
			{
				indices[k]=i*c+j;
				indices[k+1]=i*c+j+1;
				indices[k+2]=(i+1)*c+j;

				indices[k+3]=i*c+j+1;
				indices[k+4]=(i+1)*c+j+1;
				indices[k+5]=(i+1)*c+j;
				k+=6;			
			}
		}
	}

	unsigned int nextLine=numLinesV*2;

	if(topology==eGeometryTopology::Lines)
	{
		for(unsigned int i=0; i<cp.r();++i) //generate lines in V direction
		{
			for(unsigned int j=0; j<(cp.c()-1); ++j )
			{
				indices[k]=i*c+j;
				indices[k+1]=i*c+j+1;
				k+=2;			
			}
		}

		k=0;

		for(unsigned int j=0; j<cp.c();++j) //generate lines in U direction
		{
			for(unsigned int i=0; i<(cp.r()-1); ++i )
			{
				indices[nextLine+k]=i*c+j;
				indices[nextLine+k+1]=(i+1)*c+j;
				k+=2;			
			}
		}
	}	

	ind->Swap(indices);
	pos->Swap(positions);
	//N->Swap(normals);

	return geo;
};

template <class Nreal, class Nint>
GeometryPtr NurbsSurfaceTool< Nreal,Nint >::MakeSurfaceGeometry(surface_t& surface, unsigned int stepU, unsigned int stepV, const GpuDevicePtr& device, const EffectPtr& effect, unsigned int bitfield, eGeometryTopology::Val topology, const std::string& name )
{
	assert(device!=0 &&"Null geometry pointer");
	assert(effect!=0 &&"Null effect pointer");

	IndicesPtr ind=device->CreateIndices(topology,0);
	GeometryPtr geo=Geometry::Create(device,effect,ind);

	//TODO: default effect
	//generate triangles
	//GeometryChunkPtr chunk;
	AttributePtr pos,N,dPdu,dPdv,tex;
	Nint numTris=2*(stepU-1)*(stepV-1);
	Nint numVerts=stepU*stepV;
	Nint numLinesU=(stepU-1)*stepV;
	Nint numLinesV=stepU*(stepV-1);
	std::vector<IIndices::value_type> indices;	

	if(topology==eGeometryTopology::Lines)
		indices.resize( (numLinesU+numLinesV)*2 );	

	if(topology==eGeometryTopology::Triangles)
		indices.resize(numTris*3);

	std::vector<AttribNames::P_t::value_type> positions;
	std::vector<AttribNames::N_t::value_type> normals;
	std::vector<AttribNames::dPdu_t::value_type> tangentU;
	std::vector<AttribNames::dPdv_t::value_type> tangentV;
	std::vector<AttribNames::uv_t::value_type> uv;

	unsigned int gP=bitfield & genP;
	unsigned int gN=( (bitfield & genN) && ( surface.GetDUorder()>0 ) && ( surface.GetDVorder()>0 ) );
	unsigned int gdPdu=( (bitfield & gendPdu) && ( surface.GetDUorder()>0 ) );
	unsigned int gdPdv=( (bitfield & gendPdv) && ( surface.GetDVorder()>0 ) );
	unsigned int gUV=(bitfield & genUV);

	if(gP)
	{
		pos=geo->NewAttribute(AttribNames::P.name, AttribQuery((eAttributeType::Val)AttribNames::P.type_id, AttribNames::P.semantic) );
		positions.resize(numVerts);
	}

	if(gN)
	{
		N=geo->NewAttribute(AttribNames::N.name, AttribQuery((eAttributeType::Val)AttribNames::N.type_id, AttribNames::N.semantic) );
		normals.resize(numVerts);
	}

	if(gdPdu)
	{
		dPdu=geo->NewAttribute(AttribNames::dPdu.name, AttribQuery((eAttributeType::Val)AttribNames::dPdu.type_id, AttribNames::dPdu.semantic) );
		tangentU.resize(numVerts);
	}

	if(gdPdv)
	{
		dPdv=geo->NewAttribute(AttribNames::dPdv.name, AttribQuery((eAttributeType::Val)AttribNames::dPdv.type_id, AttribNames::dPdv.semantic) );
		tangentV.resize(numVerts);
	}

	if(gUV)
	{
		tex=geo->NewAttribute(AttribNames::uv.name, AttribQuery((eAttributeType::Val)AttribNames::uv.type_id, AttribNames::uv.semantic) );
		uv.resize(numVerts);
	}

	unsigned int k=0;
	Nreal du=surface.Umax() / (stepU-1);
	Nreal dv=surface.Vmax() / (stepV-1);
	Nreal u=surface.Umin();
	Nreal v=surface.Vmin();


	for(unsigned int i=0; i<stepU;++i)
	{
		v=surface.Vmin();
		for(unsigned int j=0; j<stepV; ++j )
		{
			surface.PointDerivs(u,v);

			if(gP)
			{
				positions[k]=surface.Result();
			}

			Vector3 tu,tv;

			if( gN )
			{
				tu=surface.Result(1,0);
				tv=surface.Result(0,1);

				Vector3 norm=tu%tv;

				//if( norm*xAxis<0 )
				//	norm=-norm;

				normals[k]=norm;
			}
			
			if(gdPdu)
			{		
				tu=surface.Result(1,0);
				tangentU[k]=tu;
			}

			if(gdPdv)
			{		
				tv=surface.Result(0,1);
				tangentV[k]=tv;
			}

			if(gUV)
			{
				uv[k]=AttribNames::uv_t::value_type(u,v);
			}
			
			++k;
			v+=dv;
		}
		u+=du;
	}

	unsigned int c=stepV;
	k=0;

	if(topology==eGeometryTopology::Triangles)
	{
		for(unsigned int i=0; i<(stepU-1);++i)
		{
			for(unsigned int j=0; j<(stepV-1); ++j )
			{
				indices[k]=i*c+j;
				indices[k+1]=i*c+j+1;
				indices[k+2]=(i+1)*c+j;

				indices[k+3]=i*c+j+1;
				indices[k+4]=(i+1)*c+j+1;
				indices[k+5]=(i+1)*c+j;
				k+=6;			
			}
		}
	}

	unsigned int nextLine=numLinesV*2;

	if(topology==eGeometryTopology::Lines)
	{
		for(unsigned int i=0; i<stepU;++i) //generate lines in V direction
		{
			for(unsigned int j=0; j<(stepV-1); ++j )
			{
				indices[k]=i*c+j;
				indices[k+1]=i*c+j+1;
				k+=2;			
			}
		}

		k=0;

		for(unsigned int j=0; j<stepV;++j) //generate lines in U direction
		{
			for(unsigned int i=0; i<(stepU-1); ++i )
			{
				indices[nextLine+k]=i*c+j;
				indices[nextLine+k+1]=(i+1)*c+j;
				k+=2;			
			}
		}
	}	

	ind->Swap(indices);

	if(gP)
		pos->Swap(positions);

	if(gN)
		N->Swap(normals);

	if(gdPdu)
		dPdu->Swap(tangentU);

	if(gdPdv)
		dPdv->Swap(tangentV);

	if(gUV)
		tex->Swap(uv);

	return geo;
};


template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeBilinearSurface(const Vector4& p00, const Vector4& p10, const Vector4& p01, const Vector4& p11)
{
	control_poly_t Pw(2,2);
	vector_t vec(4);

	Pw(0,0)=p00;
	Pw(1,0)=p10;
	Pw(0,1)=p01;
	Pw(1,1)=p11;

	vec(0)=0.0;
	vec(1)=0.0;
	vec(2)=1.0;
	vec(3)=1.0;

	return surface_t(Pw,vec,vec,1,1,1,1);
}

template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeGeneralCylinder(const curve_t& curve, const Vector3& direction, Nreal distance)
{
	Vector4 dir(direction,0.0);
	dir.Normalize();

	unsigned int r=curve.GetControlPoly().Size();
	control_poly_t Pw(r,2);
	vector_t V(4);

	for(unsigned int i=0; i<r; ++i)
	{
		Pw(i,0)=curve(i);
		Pw(i,1)=curve(i)+curve(i).w()*dir*distance;
	}

	V(0)=0.0;
	V(1)=0.0;
	V(2)=1.0;
	V(3)=1.0;

	return surface_t( Pw, curve.GetKnotVec(), V, curve.GetUorder(), 1, curve.GetDUorder(), 1);

}


template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeCylinder(Nreal radius, Nreal start, Nreal end, Nreal height)
{
	curve_t curve=curve_tool_t::MakeCircle(radius,start,end);
	return MakeGeneralCylinder(curve,Vector3(0,0,1),height);
}


template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeRuledSurface(const curve_t& curve1, const curve_t& curve2)
{
	curve_t a(curve1), b(curve2);

	Nint aOrd=a.GetUorder(),bOrd=b.GetUorder();
	Nint maxOrd=Max(aOrd,bOrd);

	if(aOrd<maxOrd)
		curve_tool_t::DegreeElevate( (maxOrd-aOrd), a );

	if(bOrd<maxOrd)
		curve_tool_t::DegreeElevate( (maxOrd-bOrd), b);

	Nreal au0=a.Umin(), au1=a.Umax();
	Nreal bu0=b.Umin(), bu1=b.Umax();

	if( (au0!=bu0)||(au1!=bu1) )
		b.Reparametrize(au0,au1);

	Nreal dist1=a(0).Distance( b(0) );
	Nreal dist2=a(0).Distance( b( b.GetN() ) );

	if(dist2<dist1)
		b.Reverse();

	const vector_t& aU=a.GetKnotVec();
	const vector_t& bU=b.GetKnotVec();	

	vector_t aX( (bU.Size()+aU.Size()) );
	vector_t bX( (bU.Size()+aU.Size()) );
	unsigned int ka=0,kb=0;

	for(unsigned int i=0; i<aU.Size(); )
	{
		Nint mult1,mult2;
		a.FindSpanMult(aU(i),mult1);
		b.FindSpanMult(aU(i),mult2);		

		if(mult1!=mult2)
		{
			Nint diff=Math::Abs(mult1-mult2);

			for(Nint j=0; j<diff; ++j)
			{
				bX(kb)=aU(i);
				kb++;
			}
			
		}

		i+=Max(mult1,1);
	}

	for(unsigned int i=0; i<bU.Size(); ++i)
	{
		Nint mult1,mult2;
		a.FindSpanMult(bU(i),mult1);
		b.FindSpanMult(bU(i),mult2);


		if(mult1!=mult2)
		{
			Nint diff=Math::Abs(mult1-mult2);

			for(Nint j=0; j<diff; ++j)
			{
				aX(ka)=bU(i);
				ka++;
			}

		}

		i+=Max(mult2,1);
	}

	aX.Realloc(ka);
	bX.Realloc(kb);

	curve_tool_t::KnotRefine(aX,a);
	curve_tool_t::KnotRefine(bX,b);

	const curve_t::control_poly_t& Pa=a.GetControlPoly();
	const curve_t::control_poly_t& Pb=b.GetControlPoly();
	

	assert(Pa.Size()==Pb.Size());
	control_poly_t Pw( Pa.Size(),2 );

	for(unsigned int i=0; i<Pa.Size(); ++i)
	{
		Pw(i,0)=Pa(i);
		Pw(i,1)=Pb(i);
	}

	vector_t V(4);

	V(0)=0.0;
	V(1)=0.0;
	V(2)=1.0;
	V(3)=1.0;

	return surface_t(Pw, a.GetKnotVec(), V, maxOrd, 1, 1, 1);

}


template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeGeneralCone(const curve_t& base, const Vector3& apex)
{
	unsigned int c=base.GetControlPoly().Size();
	curve_t::control_poly_t Pw(c);
	for(unsigned int i=0; i<c; ++i)
	{
		Nreal w=base(i).w();
		Pw(i)=Vector4(w*apex,w);
	}

	curve_t curve(Pw,base.GetKnotVec(),base.GetUorder(),base.GetDUorder());

	return MakeRuledSurface(base,curve);
}

template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeRevolvedSurface(const Ray& axis, Nreal theta, const curve_t& profile)
{
	Nint narcs=0;
	Nint n=0;
	vector_t U;

	while( theta>Nreal(360.0) )
		theta-=Nreal(360.0);

	if( theta<=Nreal(90.0) )
	{
		narcs=1;
		n=2*narcs;
		U.Realloc(n+4);
	}
	else if(theta<=Nreal(180.0))
	{
		narcs=2;
		n=2*narcs;
		U.Realloc(n+4);
		U(3)=U(4)=Nreal(0.5);
	}
	else if(theta<=Nreal(270))
	{
		narcs=3;
		n=2*narcs;
		U.Realloc(n+4);
		U(3)=U(4)=Nreal(1.0/3.0);
		U(5)=U(6)=Nreal(2.0/3.0);
	}
	else
	{
		narcs=4;
		n=2*narcs;
		U.Realloc(n+4);
		U(3)=U(4)=Nreal(0.25);
		U(5)=U(6)=Nreal(0.5);
		U(7)=U(8)=Nreal(0.75);
	}

	Nreal dtheta=theta/narcs;
	Nint j=3+2*(narcs-1);

	for(int i=0; i<3; ++i, ++j)
	{
		U(i)=Nreal(0.0);
		U(j)=Nreal(1.0);
	}

	dtheta=Math::DegToRad(dtheta);

	Nreal wm=Math::Cos( Nreal(0.5)*dtheta );
	Nreal angle=Nreal(0.0);
	DynArray<Nreal, Shape1d> sines(narcs+1),cosines(narcs+1); 

	for(int i=1; i<=narcs; ++i)
	{
		angle+=dtheta;
		cosines(i)=Math::Cos(angle);
		sines(i)=Math::Sin(angle);
	}

	int m=profile.GetN();

	control_poly_t Pij( n+1, m+1  );
	Vector3 Pj,tmp,X,P0,T0,Y;

	for(int j=0; j<=m; ++j)
	{
		Nreal wj=profile(j).w();
		Nreal wp=Nreal(1.0/wj);		
		Pj=wp*profile(j);
		tmp= axis.ProjectPoint( Pj );
		X=Pj-tmp;
		Nreal r=X.Length();
		X.Normalize();
		Y=axis.GetDirection()%X;
		Pij(0,j)=Vector4(wj*Pj,wj);
		P0=Pj;
		T0=Y;
		int index=0;
		angle=Nreal(0.0);

		for(int i=1; i<=narcs; ++i)
		{
			Vector3 P2=tmp+r*cosines(i)*X+r*sines(i)*Y;
			Pij(index+2,j)=Vector4(wj*P2,wj);
			Vector3 T2=-sines(i)*X+cosines(i)*Y;
			Ray a(P0,T0);
			Ray b(P2,T2);
			Nreal t1=0.0,t2=0.0;
			IntersectRays(a,b,t1,t2);
			Pij(index+1,j)=Vector4( wm*wj*a(t1), wm*wj );
			index+=2;
			if(i<narcs)
			{
				P0=P2;
				T0=T2;
			}
		}
	}

	return surface_t(Pij,U,profile.GetKnotVec(),2,profile.GetUorder(),1,profile.GetDUorder());

}


template <class Nreal, class Nint>
typename NurbsSurfaceTool< Nreal,Nint >::surface_t
NurbsSurfaceTool< Nreal,Nint >::MakeBox(const Vector3& Pmin, const Vector3& Pmax)
{
	vector_t V(4),U(7);

	U(0)=Nreal(0.0);
	U(1)=Nreal(0.0);
	U(2)=Nreal(0.25);
	U(3)=Nreal(0.50);
	U(4)=Nreal(0.75);
	U(5)=Nreal(1.0);
	U(6)=Nreal(1.0);

	V(0)=Nreal(0.0);
	V(1)=Nreal(0.0);
	V(2)=Nreal(1.0);
	V(3)=Nreal(1.0);

	control_poly_t P(5,2);
	Nreal dx=Pmax.x()-Pmin.x();
	Nreal dy=Pmax.y()-Pmin.y();
	Nreal dz=Pmax.z()-Pmin.z();


	P(0,0)=ThVec4(Pmin, Nreal(1.0) );
	P(0,1)=ThVec4(Pmin(0),Pmin(1)+dy,Pmin(2), Nreal(1.0) );

	P(1,0)=ThVec4(Pmin(0)+dx,Pmin(1),Pmin(2), Nreal(1.0) );
	P(1,1)=ThVec4(Pmin(0)+dx,Pmin(1)+dy,Pmin(2), Nreal(1.0) );

	P(2,0)=ThVec4(Pmin(0)+dx,Pmin(1),Pmin(2)+dz, Nreal(1.0) );
	P(2,1)=ThVec4(Pmin(0)+dx,Pmin(1)+dy,Pmin(2)+dz, Nreal(1.0) );

	P(3,0)=ThVec4(Pmin(0),Pmin(1),Pmin(2)+dz, Nreal(1.0) );
	P(3,1)=ThVec4(Pmin(0),Pmin(1)+dy,Pmin(2)+dz, Nreal(1.0) );

	P(4,0)=P(0,0);
	P(4,1)=P(0,1);

	return surface_t(P,U,V,1,1,1,1);	
}


