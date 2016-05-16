#pragma once

#include <Thor/Core/Rtti/ThTypeInfo.h>

#define THOR_DECLARE_CLASS(className)\
	THOR_FORWARD_CLASS(className);\
	THOR_DECL_TYPE(className);

#define THOR_DECLARE_STRUCT(structName)\
	THOR_FORWARD_STRUCT(structName);\
	THOR_DECL_TYPE(structName);

#define THOR_DECLARE_CLASS_NS(className, ns)\
	THOR_FORWARD_CLASS(className);\
	THOR_DECL_TYPE(ns::className);

#define THOR_DECLARE_STRUCT_NS(structName, ns)\
	THOR_FORWARD_STRUCT(structName);\
	THOR_DECL_TYPE(ns::structName);

#define THOR_DECLARE_PLAIN_CLASS_NS(className, ns)\
class className;\
	THOR_DECL_TYPE(ns::className);

#define THOR_DECLARE_PLAIN_STRUCT_NS(structName, ns)\
struct structName;\
	THOR_DECL_TYPE(ns::structName);

namespace Thor
{
THOR_DECLARE_PLAIN_CLASS_NS(ThiRtti, Thor);
//----------------------------------------------------------------------------------------
//
//					ThiRtti
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThiRtti: public ThMemoryAllocator
{
public:
	virtual ~ThiRtti();
	virtual ThiType*		GetType()const=0;
};

}