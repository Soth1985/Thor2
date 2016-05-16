#ifndef THOR_NURBS_EXCEPTIONS_H
#define THOR_NURBS_EXCEPTIONS_H

#include <Thor/Utility/Exception.h>

namespace Thor{

	class NurbsException:public Exception
	{
	public:
		explicit NurbsException( const std::string& str ): Exception( str ){};
	};

	class NurbsParameterOutOfRange:public NurbsException
	{
	public:
		explicit NurbsParameterOutOfRange(): NurbsException( "Parameter is out of range" ){};
	};

	class NurbsNegativeWeight:public NurbsException
	{
	public:
		explicit NurbsNegativeWeight(): NurbsException( "Negative weight" ){};
	};
	

	class NurbsPlaneWithZeroSpan:public NurbsException
	{
	public:
		explicit NurbsPlaneWithZeroSpan(): NurbsException( "Attempt to create a Nurbs plane with a zero span" ){};
	};

}//Thor

#endif