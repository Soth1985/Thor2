#pragma once

#include <Thor/Framework/FrameworkExport.h>
#include <Thor/Framework/RefPtr.h>
#include <Thor/Framework/ThMemory.h>
#include <Thor/Framework/ThFlags.h>
#include <Thor/Framework/ThHandle.h>
#include <Thor/Framework/ThiRtti.h>

namespace Thor
{

THOR_DECL_TYPE(Thor::ThF32);
THOR_DECL_TYPE(Thor::ThF64);
THOR_DECL_TYPE(Thor::ThI8);
THOR_DECL_TYPE(Thor::ThU8);
THOR_DECL_TYPE(Thor::ThI16);
THOR_DECL_TYPE(Thor::ThU16);
THOR_DECL_TYPE(Thor::ThI32);
THOR_DECL_TYPE(Thor::ThU32);
THOR_DECL_TYPE(Thor::ThI64);
THOR_DECL_TYPE(Thor::ThU64);
THOR_DECL_TYPE(Thor::ThBool);
//THOR_DECL_TYPE(Thor::ThSize);
THOR_DECL_TYPE(Thor::ThString);
THOR_DECL_TYPE(Thor::ThWideString);

THOR_DECLARE_CLASS_NS(ThiObject, Thor);
//----------------------------------------------------------------------------------------
//
//					ThiObject
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThiObject: public ThRefCounted, public ThiRtti
{
public:
	ThiObject();	

	virtual ~ThiObject();
	
	virtual ThString		ToString()const;
	virtual ThiObjectPtr	DeepCopy();	
	virtual ThU32			GetHashCode()const;	

	ThiObjectPtr			This()const;	
};


}//Thor