#pragma once

#include <Thor/Core/Common.h>
#include <assert.h>

namespace Thor{

// in managed code ListConstructor fails to initialize all values but first

template < class IterT, class DataT >
class ThListConstructor
{	
public:
	THOR_INLINE ThListConstructor( IterT it, const DataT& val, const unsigned int elem_, const unsigned int size_ ):
	  iter(it), elem(elem_), size(size_)
	{
		assert(( elem < size ));//bounds checking
		*it = val;
	};

	THOR_INLINE ThListConstructor<IterT,DataT> operator,(const DataT& val) const
	{
		//IterT ii = iter + 1;
		return ThListConstructor<IterT,DataT>( iter + 1, val, elem + 1, size );
	};
private:
	IterT& iter;
	unsigned int elem;
	unsigned int size;
	ThListConstructor(){};
	//ListConstructor( const ListConstructor& copy ){};

};

}//Thor