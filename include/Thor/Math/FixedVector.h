#pragma once

#include <Thor/Math/ListConstructor.h>
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	#include <Thor/Math/ExpressionNodes.h>
#endif

#include <Thor/Math/FixedVectorFwd.h>
#include <Thor/Math/ThorMath.h>

namespace Thor{


//epsilon for vector comparisons
const double epsilon = 1e-30;

#include <Thor/Math/FixedVectorGeneric.inl>

#pragma region General Vector 

template < 
			class DataT, unsigned int sz, //sz is the number of elements in the vector
			class TagT
		 >
class ThFixedVecContainer
{
public:

	ThFixedVecContainer()
	{
		memset(&Vec[0], 0, sz * sizeof(DataT) );
	};
	
	template< class _T, class _TagT >
	ThFixedVecContainer( const ThFixedVector<_T,sz,_TagT> &copy )
	{
		for ( unsigned int i = 0; i < sz; ++i )
			Vec[i] = copy(i);
	};
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedVecContainer tmp;
	
		for( unsigned int i = 0; i < sz; ++i )
			tmp.Vec[i] = e.tree.at<DataT>(i);

		for( unsigned int i = 0; i < sz; ++i )
			Vec[i] = tmp.Vec[i];
	};
#endif
protected:
	DataT Vec[sz];
};
#pragma endregion

#pragma region Vec4 specialization
//////////////////////////
//Vec4 decl
template <
			class DataT
		 >
class ThFixedVecContainer<DataT,4,generic_vec_tag>
{
	typedef ThFixedVecContainer<DataT,3,generic_vec_tag> vec3_t;
public:
	
	THOR_INLINE ThFixedVecContainer()
		:mx( DataT(0.0) ),my( DataT(0.0) ),mz( DataT(0.0) ),mw( DataT(0.0) )
	{};

	//template< class _T, class _TagT >
	//THOR_INLINE ThFixedVecContainer( const ThFixedVecContainer<_T,4,_TagT>& copy )
	//	:mx( DataT(copy.mx) ),my( DataT(copy.my) ),mz( DataT(copy.mz) ),mw( DataT(copy.mw) )
	//{};

	template< class _T, class _TagT >
	THOR_INLINE ThFixedVecContainer( const ThFixedVector<_T,3,_TagT>& vec, const DataT& w_ )
		:mx( DataT( vec.x() ) ),my( DataT(vec.y() ) ),mz( DataT(vec.z() ) ),mw( DataT(w_) )
	{};

	template< class _T, class _TagT >
	THOR_INLINE ThFixedVecContainer( const ThFixedVector<_T,2,_TagT>& vec, const DataT& z_, const DataT& w_ )
		:mx( DataT( vec.x() ) ),my( DataT(vec.y() ) ),mz( DataT(z_) ),mw( DataT(w_) )
	{};

	THOR_INLINE ThFixedVecContainer( const DataT& x_, const DataT& y_, const DataT& z_, const DataT& w_ )
		:mx(x_),my(y_),mz(z_),mw(w_)
	{};

	vec3_t ToVec3()const
	{
		return vec3_t(x,y,z);
	}

	THOR_INLINE void Set( const DataT& x_, const DataT& y_, const DataT& z_, const DataT& w_ )
	{
		mx = x_;
		my = y_;
		mz = z_;
		mw = w_;
	}

#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedVecContainer tmp;
		
		tmp.Vec[0] = e.tree.at<DataT>(0);
		tmp.Vec[1] = e.tree.at<DataT>(1);
		tmp.Vec[2] = e.tree.at<DataT>(2);
		tmp.Vec[3] = e.tree.at<DataT>(3);

		Vec[0] = tmp.Vec[0];
		Vec[1] = tmp.Vec[1];
		Vec[2] = tmp.Vec[2];
		Vec[3] = tmp.Vec[3];
		
	};
#endif

	THOR_INLINE DataT& x()
	{
		return mx;
	};

	THOR_INLINE const DataT& x() const
	{
		return mx;
	};

	THOR_INLINE DataT& y()
	{
		return my;
	};

	THOR_INLINE const DataT& y() const
	{
		return my;
	};

	THOR_INLINE DataT& z()
	{
		return mz;
	};

	THOR_INLINE const DataT& z() const
	{
		return mz;
	};

	THOR_INLINE DataT& w()
	{
		return mw;
	};

	THOR_INLINE const DataT& w() const
	{
		return mw;
	};
    
    THOR_INLINE DataT& r()
    {
        return mx;
    };
    
    THOR_INLINE const DataT& r() const
    {
        return mx;
    };
    
    THOR_INLINE DataT& g()
    {
        return my;
    };
    
    THOR_INLINE const DataT& g() const
    {
        return my;
    };
    
    THOR_INLINE DataT& b()
    {
        return mz;
    };
    
    THOR_INLINE const DataT& b() const
    {
        return mz;
    };
    
    THOR_INLINE DataT& a()
    {
        return mw;
    };
    
    THOR_INLINE const DataT& a() const
    {
        return mw;
    };
    
protected:
    union
    {
        DataT Vec[4];
	
		struct
		{
			DataT mx,my,mz,mw;
		};
	};
};
#pragma endregion

#pragma region Vec3 specialization
//////////////////////////
//Vec3 decl
template <
			class DataT
		 >
class ThFixedVecContainer<DataT,3,generic_vec_tag>
{
public:
	
	THOR_INLINE ThFixedVecContainer():mx( DataT(0.0) ),my( DataT(0.0) ),mz( DataT(0.0) )
	{};
	
    //template< class _T, class _TagT >
	//THOR_INLINE ThFixedVecContainer( const ThFixedVecContainer<_T,3,_TagT>& copy ):mx( copy.mx ),my( copy.my ),mz( copy.mz )
	//{};

	THOR_INLINE ThFixedVecContainer( const DataT& x_, const DataT& y_, const DataT& z_ )
		:mx(x_),my(y_),mz(z_)
	{};

	//template< class _T, class _TagT >
	//THOR_INLINE ThFixedVecContainer( const ThFixedVector<_T,2,_TagT>& vec, const DataT& z_ )
	//	:mx( DataT( vec.x() ) ),my( DataT(vec.y() ) ),mz( DataT(z_) )
	//{};

	THOR_INLINE void Set( const DataT& x_, const DataT& y_, const DataT& z_ )
	{
		mx = x_;
		my = y_;
		mz = z_;
	}
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedVecContainer tmp;
		
		tmp.Vec[0] = e.tree.at<DataT>(0);
		tmp.Vec[1] = e.tree.at<DataT>(1);
		tmp.Vec[2] = e.tree.at<DataT>(2);
		
		Vec[0] = tmp.Vec[0];
		Vec[1] = tmp.Vec[1];
		Vec[2] = tmp.Vec[2];
	};
#endif
	THOR_INLINE DataT& x()
	{
		return mx;
	};

	THOR_INLINE const DataT& x() const
	{
		return mx;
	};

	THOR_INLINE DataT& y()
	{
		return my;
	};

	THOR_INLINE const DataT& y() const
	{
		return my;
	};

	THOR_INLINE DataT& z()
	{
		return mz;
	};

	THOR_INLINE const DataT& z() const
	{
		return mz;
	};
    
    THOR_INLINE DataT& r()
    {
        return mx;
    };
    
    THOR_INLINE const DataT& r() const
    {
        return mx;
    };
    
    THOR_INLINE DataT& g()
    {
        return my;
    };
    
    THOR_INLINE const DataT& g() const
    {
        return my;
    };
    
    THOR_INLINE DataT& b()
    {
        return mz;
    };
    
    THOR_INLINE const DataT& b() const
    {
        return mz;
    };
    
protected:
    union
    {
		DataT Vec[3];
	
		struct
		{
			DataT mx,my,mz;
		};
	};
};
#pragma endregion

#pragma region Vec2 specialization
//////////////////////////
//Vec2 decl
template <
			class DataT
		 >
class ThFixedVecContainer<DataT,2,generic_vec_tag>
{
public:
	
	THOR_INLINE ThFixedVecContainer():mx( DataT(0.0) ),my( DataT(0.0) )
	{};
	
    //template< class _T, class _TagT >
	//THOR_INLINE ThFixedVecContainer( const ThFixedVecContainer<_T,2,_TagT>& copy ):mx( copy.mx ),my( copy.my )
	//{};

	THOR_INLINE ThFixedVecContainer( const DataT& x_, const DataT& y_ )
		:mx(x_),my(y_)
	{};

	THOR_INLINE void Set( const DataT& x_, const DataT& y_ )
	{
		mx = x_;
		my = y_;
	}
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedVecContainer tmp;
		
		tmp.Vec[0] = e.tree.at<DataT>(0);
		tmp.Vec[1] = e.tree.at<DataT>(1);
		
		Vec[0] = tmp.Vec[0];
		Vec[1] = tmp.Vec[1];
	};
#endif

	THOR_INLINE DataT& x()
	{
		return mx;
	};

	THOR_INLINE const DataT& x() const
	{
		return mx;
	};

	THOR_INLINE DataT& y()
	{
		return my;
	};

	THOR_INLINE const DataT& y() const
	{
		return my;
	};
protected:
	union
	{
		DataT Vec[2];
	
		struct
		{
			DataT mx,my;
		};
	};
};
#pragma endregion

#pragma region Vec4 float with SSE support specialization
//////////////////////////
//Vec4 with sse decl
template <>
class ThFixedVecContainer<float,4,sse4f_vec_tag>
{
public:	

	THOR_INLINE ThFixedVecContainer():mx( float(0.0) ),my( float(0.0) ),mz( float(0.0) ),mw( float(0.0) )
    {};
    
	//template< class _T, class _TagT >
	//THOR_INLINE ThFixedVecContainer( const ThFixedVecContainer<_T,4,_TagT>& copy ):mx( copy.mx ),my( copy.my ),mz( copy.mz ),mw( copy.mw )
	//{};

	THOR_INLINE ThFixedVecContainer( const float& x_, const float& y_, const float& z_, const float& w_ )
		:mx(x_),my(y_),mz(z_),mw(w_)
	{};

	THOR_INLINE void Set( const float& x_, const float& y_, const float& z_, const float& w_ )
	{
		mx = x_;
		my = y_;
		mz = z_;
		mw = w_;
	}
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedVecContainer tmp;
		
		tmp.Vec[0] = e.tree.at<DataT>(0);
		tmp.Vec[1] = e.tree.at<DataT>(1);
		tmp.Vec[2] = e.tree.at<DataT>(2);
		tmp.Vec[3] = e.tree.at<DataT>(3);

		Vec[0] = tmp.Vec[0];
		Vec[1] = tmp.Vec[1];
		Vec[2] = tmp.Vec[2];
		Vec[3] = tmp.Vec[3];
		
	};
#endif
	THOR_INLINE float& x()
	{
		return mx;
	};

	THOR_INLINE const float& x() const
	{
		return mx;
	};

	THOR_INLINE float& y()
	{
		return my;
	};

	THOR_INLINE const float& y() const
	{
		return my;
	};

	THOR_INLINE float& z()
	{
		return mz;
	};

	THOR_INLINE const float& z() const
	{
		return mz;
	};

	THOR_INLINE float& w()
	{
		return mw;
	};

	THOR_INLINE const float& w() const
	{
		return mw;
	};
	
protected:
	union
	{
        float THOR_ALIGNED(16) Vec[4];
	
		struct
		{
			float mx,my,mz,mw;
		};
	};
};
#pragma endregion

#pragma region FixedVector definition
/////////////////////////
template < 
			class DataT,
            unsigned int sz, //sz is the number of elements in the vector
			class TagT
		 >
class ThFixedVector : public ThFixedVecContainer< DataT, sz, TagT >
{
public:
	typedef ThFixedVecContainer< DataT, sz, TagT > container_t;
	typedef ThFixedVector< DataT, sz, TagT > self_t;
	typedef DataT value_type;
	//enum { size = sz };

	typedef DataT* iterator;
	typedef const DataT* const_iterator;
	//typedef typename FixedVecContainer::block_iterator;
	
	ThFixedVector():container_t(){};

	template<class T1>
	explicit ThFixedVector(const T1& v0)
		:container_t(v0)
	{};

	template<class T1, class T2>
	explicit ThFixedVector(const T1& v0, const T2& v1)
		:container_t(v0,v1)
	{};

	explicit ThFixedVector( const DataT& v0 )
		:container_t(v0)
	{};

	explicit ThFixedVector( const DataT& v0, const DataT& v1 )
		:container_t(v0,v1)
	{};

	explicit ThFixedVector( const DataT& v0, const DataT& v1, const DataT& v2 )
		:container_t(v0,v1,v2)
	{};

	explicit ThFixedVector( const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3 )
		:container_t(v0,v1,v2,v3)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4
				)
		:container_t(v0,v1,v2,v3,v4)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5
				)
		:container_t(v0,v1,v2,v3,v4,v5)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7,
					const DataT& v8
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7,
					const DataT& v8, const DataT& v9
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)
	{};

	explicit ThFixedVector( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7,
					const DataT& v8, const DataT& v9, const DataT& v10
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
	{};

	explicit ThFixedVector( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11)
	{};

	explicit ThFixedVector( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12)
	{};

	explicit ThFixedVector( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12, const DataT& v13
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13)
	{};
	
	explicit ThFixedVector( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12, const DataT& v13, const DataT& v14
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14)
	{};

	explicit ThFixedVector( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12, const DataT& v13, const DataT& v14, const DataT& v15
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15)
	{};
	
	//template< class _T, class _TagT >
	//ThFixedVector( const ThFixedVector<_T,sz,_TagT>& copy ):container_t(copy){};
	//assigns all elements of the vector to s
	template < class RealT >
	inline ThListConstructor< iterator, DataT > operator=(const RealT& s)
	{
		return ThListConstructor< iterator, DataT >( &container_t::Vec[0], s, 0, Size() );
	};
	//
#ifdef USE_VECTOR_EXPRESSION_TEMPLATES	
	template <class T, class R>
	THOR_INLINE  const ThFixedVector& operator=( const Expression<T,R>& e)
	{		
		assign_expr(e);
		
		return *this;
	};
#else
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector operator+(const ThFixedVector<_T,sz,_TagT> &v) const
	{
		ThFixedVector tmp;
		VecAdd( *this, v, tmp );
		return tmp;
	};
	//
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector operator-(const ThFixedVector<_T,sz,_TagT> &v) const
	{
		ThFixedVector tmp;
		VecSub( *this, v, tmp );
		return tmp;
	};
	//
	//template < class RealT >
	THOR_INLINE ThFixedVector operator*(const value_type& s) const
	{
		ThFixedVector tmp;
		VecMulScalar( *this, s, tmp );
		return tmp;
	};
	
	//
	//template < class RealT >
	THOR_INLINE ThFixedVector operator/(const value_type& s) const
	{
		ThFixedVector tmp;
		VecDivScalar( *this, s, tmp );
		return tmp;
	};

	//
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector operator^(const ThFixedVector<_T,sz,_TagT>& v) const
	{
		ThFixedVector tmp;
		VecXor( *this, v, tmp );
		return tmp;
	};	

	//
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector operator%(const ThFixedVector<_T,sz,_TagT>& v) const
	{
		ThFixedVector tmp;
		VecModulus( *this, v, tmp );
		return tmp;
	};	
	////////////////////////////////////////////////////////////////////////////////
	template< class _T, class _TagT/*, class RealT*/ >
	friend THOR_INLINE ThFixedVector operator*(const value_type& s,const ThFixedVector<_T,sz,_TagT> &v)
	{
		ThFixedVector tmp;
		VecMulScalar( v, s, tmp );
		return tmp;
	};

	//unary minus
	template< class _T, class _TagT >
	friend THOR_INLINE ThFixedVector operator-(const ThFixedVector<_T,sz,_TagT>& arg)
	{
		ThFixedVector tmp;
		VecUnaryMinus(arg,tmp);
		return tmp;
	};
#endif
	//assign vector to vector
	template< class _T, class _TagT, unsigned int Sz >
	THOR_INLINE ThFixedVector& operator=(const ThFixedVector<_T,Sz,_TagT> &v)
	{
		VecAssign( *this, v );
		return *this;
	};
	//add assign vec
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector& operator+=(const ThFixedVector<_T,sz,_TagT> &v)
	{
		VecAdd( *this, v, *this );
		return *this;

	};
	//xor assign vec
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector& operator^=(const ThFixedVector<_T,sz,_TagT> &v)
	{
		VecXor( *this, v, *this );
		return *this;

	};
	//sub assign vec
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector& operator-=(const ThFixedVector<_T,sz,_TagT> &v)
	{
		VecSub( *this, v, *this );
		return *this;

	};
	//
	//template < class RealT >
	THOR_INLINE ThFixedVector& operator*=(const value_type& s)
	{
		VecMulScalar( *this, s, *this );

		return *this;
	};
	//
	//template < class RealT >
	THOR_INLINE ThFixedVector& operator/=(const value_type& s)
	{
		VecDivScalar( *this, s, *this );

		return *this;
	};
	//modulus assign vec
	template< class _T, class _TagT >
	THOR_INLINE ThFixedVector& operator%=(const ThFixedVector<_T,sz,_TagT> &v)
	{
		VecModulus( *this, v, *this );
		return *this;

	};
	//modulus assign quat
	template< class _T >
	THOR_INLINE ThFixedVector<_T,4,quaternion_tag>& operator%=(const ThFixedVector<_T,4,quaternion_tag> &v)
	{
		ThFixedVector<_T,4,quaternion_tag> tmp(*this);
		VecModulus( tmp, v, *this );
		return *this;

	};
	//calculates dot product
	template< class _T, class _TagT >
	THOR_INLINE DataT operator*(const ThFixedVector<_T,sz,_TagT> &v) const
	{
		DataT dp;
		VecDot( *this, v, dp );
		return dp;
	};	
	//checks if vectors are equal within epsilon
	template< class _T, class _TagT >
	THOR_INLINE bool  operator==( const ThFixedVector<_T,sz,_TagT> &v) const
	{
		return AreVecEqual( *this, v ) ;
	};

	template< class _T, class _TagT >
	THOR_INLINE bool  operator!=( const ThFixedVector<_T,sz,_TagT> &v) const
	{
		return !AreVecEqual( *this, v ) ;
	};
	//checks if this vector is shorter than v
	template< class _T, class _TagT >
	THOR_INLINE bool  operator< ( const ThFixedVector<_T,sz,_TagT> &v) const
	{
		return (*this)*(*this) < v*v ;
	};
	//returns the size of the vector
	THOR_INLINE const unsigned int Size(void) const
	{	
		return sz;
	};
	//returns an iterator pointing to the first element
	THOR_INLINE const_iterator Begin(void) const
	{	
		return &container_t::Vec[0];
	};
	//returns an iterator pointing to the element behind the last
	THOR_INLINE const_iterator End(void) const
	{	
		return &container_t::Vec[sz];
	};
	//returns an iterator pointing to the first element
	THOR_INLINE iterator Begin(void)
	{	
		return &container_t::Vec[0];
	};
	//returns an iterator pointing to the element behind the last
	THOR_INLINE iterator End(void)
	{	
		return &container_t::Vec[sz];
	};
	//vector accessor
	THOR_INLINE const DataT& operator()( const unsigned int i ) const
	{	
		return container_t::Vec[i];
	};
	//vector accessor
	THOR_INLINE DataT& operator()( const unsigned int i )
	{	
		return container_t::Vec[i];
	};
	
	THOR_INLINE ThFixedVector& Normalize()
	{
		VecNorm2(*this,*this);
		return *this;
	};

	THOR_INLINE DataT Length() const
	{
		DataT dp;
		VecDot( *this, *this, dp );
		return Math::Sqrt( dp );
	};

	THOR_INLINE DataT LengthSquared() const
	{
		DataT dp;
		VecDot( *this, *this, dp );
		return dp;
	};
	
	template< class _T, class _TagT >
	THOR_INLINE DataT DistanceSquared( const ThFixedVector<_T,sz,_TagT> &v) const
	{
		ThFixedVector tmp = v - (*this);
		return tmp * tmp;
	};

	template< class _T, class _TagT >
	THOR_INLINE DataT Distance( const ThFixedVector<_T,sz,_TagT> &v) const
	{
		ThFixedVector tmp = v - (*this);
		return Math::Sqrt(tmp*tmp);
	};
};
#pragma endregion

template <class DataT, class TagT>
static void SetWeight( ThFixedVector<DataT,4,TagT> & vec, const DataT& w_)
{
	DataT tmp = w_ / vec.w();
	vec.x() *= tmp;
	vec.y() *= tmp;
	vec.z() *= tmp;
	vec.w() = w_;
}
    
template <class DataT, unsigned int sz, class TagT>
static ThFixedVector<DataT, sz, TagT> Reflect(const ThFixedVector<DataT, sz, TagT>& vec, const ThFixedVector<DataT, sz, TagT>& norm)
{
    return vec - norm * (2.0 * vec * norm);
}
    
static const ThVec3f yAxis(0.0f,1.0f,0.0f);
static const ThVec3f xAxis(1.0f,0.0f,0.0f);
static const ThVec3f zAxis(0.0f,0.0f,1.0f);

static const ThVec4f yAxis4(0.0f,1.0f,0.0f,0.0f);
static const ThVec4f xAxis4(1.0f,0.0f,0.0f,0.0f);
static const ThVec4f zAxis4(0.0f,0.0f,1.0f,0.0f);

}//Thor

#ifdef USE_VECTOR_EXPRESSION_TEMPLATES
#include  <Thor/Math/Operators.h>
#endif