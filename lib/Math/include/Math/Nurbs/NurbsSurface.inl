//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
NurbsSurface<Nreal,Nint>::NurbsSurface(	const control_poly_t& cp, const vector_t& uVec, const vector_t& vVec,
				Nint uOrder, Nint vOrder, Nint duOrder,	Nint dvOrder )
				:P(cp),U(uVec),V(vVec),Uorder(uOrder),Vorder(vOrder),DUorder(duOrder),DVorder(dvOrder)
{
	//insert error handling
	
	n=P.r()-1;
	m=P.c()-1;

	//allocate supplementary data
	Nu.Realloc(DUorder+1,Uorder+1);
	Nv.Realloc(DVorder+1,Vorder+1);

	Nipu.Realloc(DUorder+1);
	Nipv.Realloc(DVorder+1);

	left.Realloc( Max(Uorder,Vorder)+1 );
	right.Realloc( Max(Uorder,Vorder)+1 );

	ndu.Realloc(Max(Uorder,Vorder)+1,Max(Uorder,Vorder)+1);
	a.Realloc(2,Max(Uorder,Vorder)+1);
	ND.Realloc(Max(DUorder,DVorder)+1);

	SKL.Realloc(DUorder+1,DVorder+1);
	Aw.Realloc(DUorder+1,DVorder+1);

	Temp.Realloc(Vorder+1);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface<Nreal,Nint>::Refresh(	Nint uOrder, Nint vOrder, Nint duOrder,	Nint dvOrder )
{
	Uorder=uOrder;
	Vorder=vOrder;
	DUorder=Min(Uorder,duOrder);
	DVorder=Min(Uorder,dvOrder);

	//allocate supplementary data
	Nu.Realloc(DUorder+1,Uorder+1);
	Nv.Realloc(DVorder+1,Vorder+1);

	Nipu.Realloc(DUorder+1);
	Nipv.Realloc(DVorder+1);

	left.Realloc( Max(Uorder,Vorder)+1 );
	right.Realloc( Max(Uorder,Vorder)+1 );

	ndu.Realloc(Max(Uorder,Vorder)+1,Max(Uorder,Vorder)+1);
	a.Realloc(2,Max(Uorder,Vorder)+1);
	ND.Realloc(Max(DUorder,DVorder)+1);

	SKL.Realloc(DUorder+1,DVorder+1);
	Aw.Realloc(DUorder+1,DVorder+1);

	Temp.Realloc(Vorder+1);
}
///////////////////End Constructors//////////////////////////////////////
template <class Nreal, class Nint>
Nint NurbsSurface< Nreal,Nint >::FindSpanU(Nreal u)
{
	Nint mid,low,high;

	if( u==U(n+1) ) return(n);

	low=Uorder;
	high=n+1;

	mid=(low+high)>>1;

	while( u<U(mid) || u>=U(mid+1) )
	{
		if(u<U(mid)) high=mid;
		else low=mid;

		mid=(low+high)>>1;
	}

	return (mid);
}

//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nint NurbsSurface< Nreal,Nint >::FindSpanV(Nreal v)
{
	Nint mid,low,high;

	if( v==V(m+1) ) return(m);

	low=Vorder;
	high=m+1;

	mid=(low+high)>>1;

	while( v<V(mid) || v>=V(mid+1) )
	{
		if( v<V(mid) ) high=mid;
		else low=mid;

		mid=(low+high)>>1;
	}

	return (mid);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::BasisFuncU(Nint i,Nreal u)
{
	Nint j,r;
	Nreal	saved,//stores i,j-1 bf
		temp;//stores i+1,j-1 bf

	Nu(0,0)=1.0;

	for(j=1;j<=Uorder;j++)  //loop through orders
	{
		left(j)=u-U(i+1-j);
		right(j)=U(i+j)-u;
		saved=0.0;//i-p-1,j-1 bf is zero for the first non zero func
		for(r=0;r<j;r++)
		{
			temp=Nu(0,r)/( right(r+1)+left(j-r) );
			Nu(0,r)=saved+right(r+1)*temp;
			saved=left(j-r)*temp;
		}
		Nu(0,j)=saved;//i+1,j-1 bf is zero
	}
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::BasisFuncV(Nint i,Nreal v)
{
	Nint j,r;
	Nreal	saved,//stores i,j-1 bf
		temp;//stores i+1,j-1 bf

	Nv(0,0)=1.0;

	for(j=1;j<=Vorder;j++)  //loop through orders
	{
		left(j)=v-V(i+1-j);
		right(j)=V(i+j)-v;
		saved=0.0;//i-p-1,j-1 bf is zero for the first non zero func
		for(r=0;r<j;r++)
		{
			temp=Nv(0,r)/( right(r+1)+left(j-r) );
			Nv(0,r)=saved+right(r+1)*temp;
			saved=left(j-r)*temp;
		}
		Nv(0,j)=saved;//i+1,j-1 bf is zero
	}
}

//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::DersBasisFuncU(Nint i,Nreal u)
{
	Nint j,r,s1,s2,pk,rk,j1,j2,k;
	Nreal temp,saved,d;
	ndu(0,0)=1.0;

	for(j=1;j<=Uorder;j++)		//compute bf and differences
	{
		left(j)=u-U(i+1-j);
		right(j)=U(i+j)-u;
		saved=0.0;//i-p-1,j-1 bf is zero for the first non zero func
		for(r=0;r<j;r++)
		{
			ndu(j,r)=right(r+1)+left(j-r);
			temp=ndu(r,j-1)/ndu(j,r);
			ndu(r,j)=saved+right(r+1)*temp;
			saved=left(j-r)*temp;
		}
		ndu(j,j)=saved;//i+1,j-1 bf is zero
	}

	for(j=0;j<=Uorder;j++)//load bf
		Nu(0,j)=ndu(j,Uorder);

	for(r=0;r<=Uorder;r++)//loop over bf index
	{
		s1=0;
		s2=1;

		a(0,0)=1.0;

		for(k=1;k<=DUorder;k++)//compute k-th derivative
		{
			d=0.0;
			rk=r-k;
			pk=Uorder-k;
			if( r>=k )
			{
				a(s2,0)=a(s1,0)/ndu(pk+1,rk);
				d=a(s2,0)*ndu(rk,pk);
			}

			if( rk>=-1 ) j1=1;
			else		j1=-rk;

			if( r-1<=pk ) j2=k-1;
			else          j2=Uorder-r;

			for(j=j1;j<=j2;j++)
			{
				a(s2,j)=( a(s1,j)-a(s1,j-1) )/ndu(pk+1,rk+j);
				d+=a(s2,j)*ndu(rk+j,pk);
			}

			if( r<=pk ) 
			{
				a(s2,k)=-a(s1,k-1)/ndu(pk+1,r);
				d+=a(s2,k)*ndu(r,pk);
			}

			Nu(k,r)=d;

			j=s1;
			s1=s2;
			s2=j;
		}
	}

	r=Uorder;

	for(k=1;k<=DUorder;k++)
	{
		for(j=0;j<=Uorder;j++) Nu(k,j)*=r;
		r*=(Uorder-k);
	}
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::DersBasisFuncV(Nint i,Nreal v)
{
	Nint j,r,s1,s2,pk,rk,j1,j2,k;
	Nreal temp,saved,d;
	ndu(0,0)=1.0;

	for(j=1;j<=Vorder;j++)		//compute bf and differences
	{
		left(j)=v-V(i+1-j);
		right(j)=V(i+j)-v;
		saved=0.0;//i-p-1,j-1 bf is zero for the first non zero func
		for(r=0;r<j;r++)
		{
			ndu(j,r)=right(r+1)+left(j-r);
			temp=ndu(r,j-1)/ndu(j,r);
			ndu(r,j)=saved+right(r+1)*temp;
			saved=left(j-r)*temp;
		}
		ndu(j,j)=saved;//i+1,j-1 bf is zero
	}

	for(j=0;j<=Vorder;j++)//load bf
		Nv(0,j)=ndu(j,Vorder);

	for(r=0;r<=Vorder;r++)//loop over bf index
	{
		s1=0;
		s2=1;

		a(0,0)=1.0;

		for(k=1;k<=DVorder;k++)//compute k-th derivative
		{
			d=0.0;
			rk=r-k;
			pk=Vorder-k;
			if( r>=k )
			{
				a(s2,0)=a(s1,0)/ndu(pk+1,rk);
				d=a(s2,0)*ndu(rk,pk);
			}

			if( rk>=-1 ) j1=1;
			else		j1=-rk;

			if( r-1<=pk ) j2=k-1;
			else          j2=Vorder-r;

			for(j=j1;j<=j2;j++)
			{
				a(s2,j)=( a(s1,j)-a(s1,j-1) )/ndu(pk+1,rk+j);
				d+=a(s2,j)*ndu(rk+j,pk);
			}

			if( r<=pk ) 
			{
				a(s2,k)=-a(s1,k-1)/ndu(pk+1,r);
				d+=a(s2,k)*ndu(r,pk);
			}

			Nv(k,r)=d;

			j=s1;
			s1=s2;
			s2=j;
		}
	}

	r=Vorder;

	for(k=1;k<=DVorder;k++)
	{
		for(j=0;j<=Vorder;j++) Nv(k,j)*=r;
		r*=(Vorder-k);
	}
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::OneBasisFuncU(Nint i,Nreal u)
{
	Nint j,k;
	Nreal Uleft,Uright,saved,temp;

	if( ( i==0 && u==U(0) ) || (i==n && u==U(n) ) )//edges 
	{
		Nipu(0)=1.0;
		return;
	}

	if( u<U(i) || u>U(i+Uorder+1) )//if out of span
	{
		Nipu(0)=0.0;
		return;
	}

	for(j=0;j<=Uorder;j++)//init zero degree func
	{
		if( u>=U(i+j) && u<U(i+j+1) ) Nu(0,j)=1.0;
		else Nu(0,j)=0.0;
	}

	for(k=1;k<=Uorder;k++)//compute higher order bf
	{
		if( Nu(0,0)==0.0 ) saved=0.0;//i,k-1 term is zero
		else  saved=( u-U(i)*Nu(0,0) )/( U(i+k)-U(i) );

		for(j=0;j<Uorder-k+1;j++)
		{
			Uleft=U(i+j+1);
			Uright=U(i+j+k+1);

			if( Nu(0,j+1)==0.0 ) //j+1,k-1 term is zero
			{
				Nu(0,j)=saved;
				saved=0.0;
			}
			else
			{
				temp=Nu(0,j+1)/(Uright-Uleft);
				Nu(0,j)=saved+(Uright-u)*temp;
				saved=(u-Uleft)*temp;
			}
		}
	}
	Nipu(0)=Nu(0,0);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::OneBasisFuncV(Nint i,Nreal v)
{
	Nint j,k;
	Nreal Vleft,Vright,saved,temp;

	if( ( i==0 && v==V(0) ) || (i==m && v==V(m) ) )//edges 
	{
		Nipv(0)=1.0;
		return;
	}

	if( v<V(i) || v>V(i+Vorder+1) )//if out of span
	{
		Nipv(0)=0.0;
		return;
	}

	for(j=0;j<=Vorder;j++)//init zero degree func
	{
		if( v>=V(i+j) && v<V(i+j+1) ) Nv(0,j)=1.0;
		else Nv(0,j)=0.0;
	}

	for(k=1;k<=Vorder;k++)//compute higher order bf
	{
		if( Nv(0,0)==0.0 ) saved=0.0;//i,k-1 term is zero
		else  saved=( v-V(i)*Nv(0,0) )/( V(i+k)-V(i) );

		for(j=0;j<Vorder-k+1;j++)
		{
			Vleft=V(i+j+1);
			Vright=V(i+j+k+1);

			if( Nv(0,j+1)==0.0 ) //j+1,k-1 term is zero
			{
				Nv(0,j)=saved;
				saved=0.0;
			}
			else
			{
				temp=Nv(0,j+1)/(Vright-Vleft);
				Nv(0,j)=saved+(Vright-v)*temp;
				saved=(v-Vleft)*temp;
			}
		}
	}
	Nipv(0)=Nv(0,0);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::DersOneBasisFuncU(Nint i,Nreal u)
{
	Nint j,k,jj;
	Nreal Uleft,Uright,saved,temp;

	//if( ( i==0 && u==U(0) ) || (i==n && u==U(n) ) )//edges 
	//{
	//	
	//	return;
	//}

	if( u<U(i) || u>=U(i+Uorder+1) )//if out of span
	{
		for(k=0;k<=DUorder;k++)
			Nipu(k)=0.0;

		return;
	}

	for(j=0;j<=Uorder;j++)//init zero degree func
	{
		if( u>=U(i+j) && u<U(i+j+1) ) Nu(0,j)=1.0;
		else Nu(0,j)=0.0;
	}

	for(k=1;k<=Uorder;k++)//compute higher order bf
	{
		if( Nu(k-1,0)==0.0 ) saved=0.0;//i,k-1 term is zero
		else  saved=( u-U(i)*Nu(k-1,0) )/( U(i+k)-U(i) );

		for(j=0;j<Uorder-k+1;j++)
		{
			Uleft=U(i+j+1);
			Uright=U(i+j+k+1);

			if( Nu(k-1,j+1)==0.0 ) //j+1,k-1 term is zero
			{
				Nu(k,j)=saved;
				saved=0.0;
			}
			else
			{
				temp=Nu(k-1,j+1)/(Uright-Uleft);
				Nu(k,j)=saved+(Uright-u)*temp;
				saved=(u-Uleft)*temp;
			}
		}
	}

	Nipu(0)=Nu(Uorder,0);

	for(k=1;k<=DUorder;k++)//compute derivatives
	{
		for(j=0;j<=k;j++)//load required column
			ND(j)=Nu(Uorder-k,j);

		for(jj=1;jj<=k;jj++)//compute k-th derivative
		{
			if( ND(0)==0.0 ) saved=0.0;
			else saved=ND(0)/ ( U(i+Uorder-k+jj) - U(i) );

			for(j=0;j<k-jj+1;j++)
			{
				Uleft=U(i+j+1);
				Uright=U(i+j+Uorder+jj+1);
				if( ND(j+1)==0.0 )
				{
					ND(j)=(Uorder-k+jj)*saved;
					saved=0.0;
				}
				else
				{
					temp=ND(j+1)/(Uright-Uleft);
					ND(j)=(Uorder-k+jj)*(saved-temp);
					saved=temp;
				}
			}
		}
		Nipu(k)=ND(0);
	}
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::DersOneBasisFuncV(Nint i,Nreal v)
{
	Nint j,k,jj;
	Nreal Vleft,Vright,saved,temp;

	//if( ( i==0 && u==U(0) ) || (i==n && u==U(n) ) )//edges 
	//{
	//	
	//	return;
	//}

	if( v<V(i) || v>=V(i+Vorder+1) )//if out of span
	{
		for(k=0;k<=DVorder;k++)
			Nipv(k)=0.0;

		return;
	}

	for(j=0;j<=Vorder;j++)//init zero degree func
	{
		if( v>=V(i+j) && v<V(i+j+1) ) Nv(0,j)=1.0;
		else Nv(0,j)=0.0;
	}

	for(k=1;k<=Vorder;k++)//compute higher order bf
	{
		if( Nv(k-1,0)==0.0 ) saved=0.0;//i,k-1 term is zero
		else  saved=( v-V(i)*Nv(k-1,0) )/( V(i+k)-V(i) );

		for(j=0;j<Vorder-k+1;j++)
		{
			Vleft=V(i+j+1);
			Vright=V(i+j+k+1);

			if( Nv(k-1,j+1)==0.0 ) //j+1,k-1 term is zero
			{
				Nv(k,j)=saved;
				saved=0.0;
			}
			else
			{
				temp=Nv(k-1,j+1)/(Vright-Vleft);
				Nv(k,j)=saved+(Vright-v)*temp;
				saved=(v-Vleft)*temp;
			}
		}
	}

	Nipv(0)=Nv(Vorder,0);

	for(k=1;k<=DVorder;k++)//compute derivatives
	{
		for(j=0;j<=k;j++)//load required column
			ND(j)=Nv(Vorder-k,j);

		for(jj=1;jj<=k;jj++)//compute k-th derivative
		{
			if( ND(0)==0.0 ) saved=0.0;
			else saved=ND(0)/ ( V(i+Vorder-k+jj) - V(i) );

			for(j=0;j<k-jj+1;j++)
			{
				Vleft=V(i+j+1);
				Vright=V(i+j+Vorder+jj+1);
				if( ND(j+1)==0.0 )
				{
					ND(j)=(Vorder-k+jj)*saved;
					saved=0.0;
				}
				else
				{
					temp=ND(j+1)/(Vright-Vleft);
					ND(j)=(Vorder-k+jj)*(saved-temp);
					saved=temp;
				}
			}
		}
		Nipv(k)=ND(0);
	}
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nreal NurbsSurface< Nreal,Nint >::Umax(void)const
{
	return U( U.c() - 1 );
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nreal NurbsSurface< Nreal,Nint >::Umin(void)const
{
	return U(0);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nreal NurbsSurface< Nreal,Nint >::Vmax(void)const
{
	//if( V.c() )
	return V( V.c() - 1 );
	//else
	//	return 0.0;
}

//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nreal NurbsSurface< Nreal,Nint >::Vmin(void)const
{
	//if( V.c() )
	return V(0);
	//else
	//	return 0.0;
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::Point(Nreal u,Nreal v)
{
	if( u>Umax() )
		u=Umax();

	if( u<Umin() )
		u=Umin();

	if( v>Vmax() )
		v=Vmax();

	if( v<Vmin() )
		v=Vmin();

	Nint uspan,vspan,k,l,uind,vind;

	uspan=FindSpanU(u);
	vspan=FindSpanV(v);
	BasisFuncU(uspan,u);
	BasisFuncV(vspan,v);
	uind=uspan-Uorder;
	vind=vspan-Vorder;

	for(l=0;l<=Vorder;l++)
	{
		Temp(l)=ThVec4(0.0,0.0,0.0,0.0);
		for(k=0;k<=Uorder;k++)
			Temp(l)=Temp(l)+P(uind+k,vind+l)*Nu(0,k);
	}

	SKL(0,0)=ThVec4(0.0,0.0,0.0,0.0);

	for(l=0;l<=Vorder;l++)
	{
		SKL(0,0)=SKL(0,0)+Temp(l)*Nv(0,l);
	}

	SKL(0,0)*=( 1.0 / SKL(0,0).w() );
	SKL(0,0).w()=1.0;
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::PointDerivs(Nreal u,Nreal v)
{
	if( u>Umax() )
		u=Umax();

	if( u<Umin() )
		u=Umin();

	if( v>Vmax() )
		v=Vmax();

	if( v<Vmin() )
		v=Vmin();

	Nint uspan,vspan,uind,vind,k,l,s,j,i,r,lstop=DVorder;
	Vector4 vec,vec2;

	uspan=FindSpanU(u);
	vspan=FindSpanV(v);

	DersBasisFuncU(uspan,u);
	DersBasisFuncV(vspan,v);

	uind=uspan-Uorder;
	vind=vspan-Vorder;

	for(k=0;k<=DUorder;k++)
	{	
		//compute 4d surface with derivs
		for(s=0;s<=Vorder;s++)
		{
			Temp(s)=ThVec4(0.0,0.0,0.0,0.0);
			for(r=0;r<=Uorder;r++)
			{
				Temp(s)=Temp(s)+P(uind+r,vind+s)*Nu(k,r);
			}//for r
		}//for s



		for(l=0;l<=lstop;l++)
		{
			Aw(k,l)=ThVec4(0.0,0.0,0.0,0.0);
			for(s=0;s<=Vorder;s++)
			{
				Aw(k,l)=Aw(k,l)+Temp(s)*Nv(l,s);
			}//for s

			vec=Aw(k,l);

			for(j=1;j<=l;j++)//now get 3d derivs
				vec=vec-SKL(k,l-j)*Aw(0,j).w()*Math::Bin<Nreal>(l,j);

			for(i=1;i<=k;i++)
			{
				vec=vec-SKL(k-i,l)*Math::Bin<Nreal>(k,i)*Aw(i,0).w();
				vec2=0.0;
				for(j=1;j<=l;j++)
					vec2=vec2+SKL(k-i,l-j)*Aw(i,j).w()*Math::Bin<Nreal>(l,j);
				vec=vec-vec2*Math::Bin<Nreal>(k,i);

			}//for i
			SKL(k,l)=vec/Aw(0,0).w();
			SKL(k,l).w()=1.0;

		}//for l

		lstop=lstop-1;
	}//for k
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nint NurbsSurface< Nreal,Nint >::FindSpanMultU(Nreal u,Nint &s)
{
	Nint mid,low,high;

	if( u==U(n+1) )
	{
		s=1;
		return(n);
	}

	low=Uorder;
	high=n+1;

	mid=(low+high)>>1;

	while( u<U(mid) || u>=U(mid+1) )
	{
		if(u<U(mid)) high=mid;
		else low=mid;

		mid=(low+high)>>1;
	}


	s=0;
	//low=0;
	high=mid+Uorder;

	for(low=0;low<=high;low++ )
	{
		if ( U(low)== u )
			s++;
	}

	return (mid);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nint NurbsSurface< Nreal,Nint >::FindSpanMultV(Nreal v,Nint &s)
{
	Nint mid,low,high;

	if( v==V(m+1) ) return(m);

	low=Uorder;
	high=m+1;

	mid=(low+high)>>1;

	while( v<V(mid) || v>=V(mid+1) )
	{
		if( v<V(mid) ) high=mid;
		else low=mid;

		mid=(low+high)>>1;
	}

	s=0;
	//low=0;
	high=mid+Vorder;

	for(low=0;low<=high;low++ )
	{
		if ( V(low)==v)
			s++;
	}

	return (mid);
}

template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::ReparametrizeU(Nreal c, Nreal d)
{
	assert(d>c);

	Nreal alpha=( Umax()-Umin() ) / ( d-c );
	Nreal beta= Umin()-alpha*c;
	alpha=Nreal(1.0) / alpha;

	for(unsigned int i=0; i<U.c(); ++i)
	{
		U(i)=( U(i)-beta ) * alpha;
	}
}

template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::ReparametrizeV(Nreal c, Nreal d)
{
	assert(d>c);

	Nreal alpha=( Vmax()-Vmin() ) / ( d-c );
	Nreal beta= Vmin()-alpha*c;
	alpha=Nreal(1.0) / alpha;

	for(unsigned int i=0; i<V.c(); ++i)
	{
		V(i)=( V(i)-beta ) * alpha;
	}
}


template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::ReverseU()
{
	vector_t S( U.c() );
	Nint x=n+Uorder+1;
	Nreal a=Umin();
	Nreal b=Umax();

	for(int i=0; i<U.c(); ++i)
	{
		S(i)=U(i);
	}

	for(int i=1; i<=(x-2*Uorder-1); ++i)
	{
		S(x-Uorder-i)=-U(Uorder+i)+a+b;
	}

	control_poly_t Q(n+1,m+1);

	for(Nint i=0; i<=n; ++i)
	{
		for(Nint j=0; j<=m; ++j)
			Q(i,j)=P(n-i,j);
	}

	P.Swap(Q);
	U.Swap(S);
}

template <class Nreal, class Nint>
void NurbsSurface< Nreal,Nint >::ReverseV()
{
	vector_t S( V.c() );
	Nint x=m+Vorder+1;
	Nreal a=Vmin();
	Nreal b=Vmax();

	for(int i=0; i<V.c(); ++i)
	{
		S(i)=V(i);
	}

	for(int i=1; i<=(x-2*Vorder-1); ++i)
	{
		S(x-Vorder-i)=-V(Vorder+i)+a+b;
	}

	control_poly_t Q(n+1,m+1);

	for(Nint i=0; i<=n; ++i)
	{
		for(Nint j=0; j<=m; ++j)
			Q(i,j)=P(i,m-j);
	}

	P.Swap(Q);
	V.Swap(S);
}