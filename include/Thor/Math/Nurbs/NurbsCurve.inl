//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
NurbsCurve< Nreal,Nint >::NurbsCurve(	const control_poly_t& cp, const vector_t& uVec,
											 Nint uOrder, Nint duOrder )
											 :P(cp),U(uVec),Uorder(uOrder),DUorder(duOrder)
{	
	n=P.c()-1;
	
	//allocate supplementary data
	Nu.Realloc(DUorder+1,Uorder+1);
	Nipu.Realloc(DUorder+1);
	left.Realloc(Uorder+1);
	right.Realloc(Uorder+1);
	ndu.Realloc(Uorder+1,Uorder+1);
	a.Realloc(2,Uorder+1);
	ND.Realloc(DUorder+1);

	SKL.Realloc(DUorder+1);
	Aw.Realloc(DUorder+1);
}

//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
void NurbsCurve< Nreal,Nint >::Refresh(	Nint uOrder, Nint duOrder )									 
{
	//allocate supplementary data
	Uorder=uOrder;
	DUorder=Min(Uorder,duOrder);
	Nu.Realloc(DUorder+1,Uorder+1);
	Nipu.Realloc(DUorder+1);
	left.Realloc(Uorder+1);
	right.Realloc(Uorder+1);
	ndu.Realloc(Uorder+1,Uorder+1);
	a.Realloc(2,Uorder+1);
	ND.Realloc(DUorder+1);

	SKL.Realloc(DUorder+1);
	Aw.Realloc(DUorder+1);
}

template <class Nreal, class Nint>
NurbsCurve< Nreal,Nint >::~NurbsCurve()
{
}
///////////////////End Constructors//////////////////////////////////////
template <class Nreal, class Nint>
Nint NurbsCurve< Nreal,Nint >::FindSpan(Nreal u)
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
void NurbsCurve< Nreal,Nint >::BasisFunc(Nint i,Nreal u)
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
void NurbsCurve< Nreal,Nint >::DersBasisFunc(Nint i,Nreal u)
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
void NurbsCurve< Nreal,Nint >::OneBasisFunc(Nint i,Nreal u)
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
void NurbsCurve< Nreal,Nint >::DersOneBasisFunc(Nint i,Nreal u)
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
//(Nu.Data[0]*P.Vertices.Data[0].x+Nu.Data[1]*P.Vertices.Data[1].x+Nu.Data[2]*P.Vertices.Data[2].x)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
//(Nu.Data[0]*P.Vertices.Data[0].y+Nu.Data[1]*P.Vertices.Data[1].y+Nu.Data[2]*P.Vertices.Data[2].y)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
//(Nu.Data[0]*P.Vertices.Data[0].z+Nu.Data[1]*P.Vertices.Data[1].z+Nu.Data[2]*P.Vertices.Data[2].z)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
template <class Nreal, class Nint>
void NurbsCurve< Nreal,Nint >::Point(Nreal u)
{
	if( u>Umax() )
		u=Umax();

	if( u<Umin() )
		u=Umin();

	Nint span,j,ind;

	span=FindSpan(u);
	BasisFunc(span,u);
	ind=span-Uorder;
	SKL(0)=ThVec4(0.0,0.0,0.0,0.0);
	for(j=0;j<=Uorder;j++)
		SKL(0)=SKL(0)+P(ind+j)*Nu(0,j);

	SKL(0)*=( 1.0 / SKL(0).w() );
	SKL(0).w()=1.0;
}
//////////////////////////////////////////////////////////////////////////
//(Nu.Data[0]*P.Vertices.Data[0].x+Nu.Data[1]*P.Vertices.Data[1].x+Nu.Data[2]*P.Vertices.Data[2].x)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
//(Nu.Data[0]*P.Vertices.Data[0].y+Nu.Data[1]*P.Vertices.Data[1].y+Nu.Data[2]*P.Vertices.Data[2].y)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
//(Nu.Data[0]*P.Vertices.Data[0].z+Nu.Data[1]*P.Vertices.Data[1].z+Nu.Data[2]*P.Vertices.Data[2].z)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])

//(Nu.Data[3]*P.Vertices.Data[0].x+Nu.Data[4]*P.Vertices.Data[1].x+Nu.Data[5]*P.Vertices.Data[2].x)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
//(Nu.Data[3]*P.Vertices.Data[0].y+Nu.Data[4]*P.Vertices.Data[1].y+Nu.Data[5]*P.Vertices.Data[2].y)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])

//(Nu.Data[6]*P.Vertices.Data[0].x+Nu.Data[7]*P.Vertices.Data[1].x+Nu.Data[8]*P.Vertices.Data[2].x)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
//(Nu.Data[6]*P.Vertices.Data[0].y+Nu.Data[7]*P.Vertices.Data[1].y+Nu.Data[8]*P.Vertices.Data[2].y)/(Nu.Data[0]+Nu.Data[1]+Nu.Data[2])
template <class Nreal, class Nint>
void NurbsCurve< Nreal,Nint >::PointDerivs(Nreal u)
{
	Nint span,k,j,i;
	Vector4  v;

	if( u>Umax() )
		u=Umax();

	if( u<Umin() )
		u=Umin();

	/*if( u<U(0) || u>U(n+Uorder) )
	{
		throw NurbsParameterOutOfRange();
	}*/



	span=FindSpan(u);

	DersBasisFunc(span,u);

	for(k=0;k<=DUorder;k++)
	{
		SKL(k)=ThVec4(0.0,0.0,0.0,0.0);
		Aw(k)=ThVec4(0.0,0.0,0.0,0.0);//get ders of a 4 dimensional curve
		for(j=0;j<=Uorder;j++)
			Aw(k)=Aw(k)+P(span-Uorder+j)*Nu(k,j);

		v=Aw(k);//get ders of a 3 dimensional curve

		for(i=1;i<=k;i++)
			v=v-SKL(k-i)*Math::Bin<Nreal>(k,i)*Aw(i).w();


		SKL(k)=v/Aw(0).w();
		SKL(k).w()=1.0;
	}

}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nreal NurbsCurve< Nreal,Nint >::Umax(void)const
{
	return U( U.c() - 1 );
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nreal NurbsCurve< Nreal,Nint >::Umin(void)const
{
	return U(0);
}

template <class Nreal, class Nint>
void NurbsCurve< Nreal,Nint >::Reparametrize(Nreal c, Nreal d)
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
void NurbsCurve< Nreal,Nint >::RationalReparametrize(Nreal c, Nreal d)
{
	assert(d>c);

	Nreal w0=Math::Pow( P(0).w(), Nreal( 1.0/Uorder ) );
	Nreal wN=Math::Pow( P(n).w(), Nreal( 1.0/Uorder ) );

	Nreal a=Umin();
	Nreal b=Umax();
	Nreal alpha=wN*d-w0*c;
	Nreal beta= w0*b*c-wN*a*d;
	Nreal gamma=wN-w0;
	Nreal delta=w0*b-wN*a;

	for(Nint i=0; i<U.c(); ++i)
	{
		U(i)=( alpha*U(i)+beta ) / ( gamma*U(i)+delta );
	}

	for(Nint i=0; i<=n; ++i)
	{
		Nreal l=P(i).w();

		for(Nint j=1; j<=Uorder; ++j)
			l*=( gamma*U(i+j)-alpha );

		//l*=P(i).w();
		SetWeight( P(i), l);
	}
}

template <class Nreal, class Nint>
void NurbsCurve< Nreal,Nint >::Reverse()
{
	vector_t S( U.c() );
	Nint m=n+Uorder+1;
	Nreal a=Umin();
	Nreal b=Umax();

	for(int i=0; i<U.c(); ++i)
	{
		S(i)=U(i);
	}

	for(int i=1; i<=(m-2*Uorder-1); ++i)
	{
		S(m-Uorder-i)=-U(Uorder+i)+a+b;
	}

	control_poly_t Q(n+1);

	for(Nint i=0; i<=n; ++i)
	{
		Q(i)=P(n-i);
	}

	P.Swap(Q);
	U.Swap(S);
}
//////////////////////////////////////////////////////////////////////////
template <class Nreal, class Nint>
Nint NurbsCurve< Nreal,Nint >::FindSpanMult(Nreal u,Nint &s)
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