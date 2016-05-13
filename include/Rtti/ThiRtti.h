#pragma once

#include <Thor/Framework/ThTypeInfo.h>
#include <Thor/Framework/ThUUID.h>

#define THOR_DECLARE_CLASS(className)\
	THOR_FORWARD_CLASS(className);\
	THOR_DECL_TYPE(className);\
	THOR_DECLARE_HANDLE(className);

#define THOR_DECLARE_STRUCT(structName)\
	THOR_FORWARD_STRUCT(structName);\
	THOR_DECL_TYPE(structName);\
	THOR_DECLARE_HANDLE(structName);

#define THOR_DECLARE_CLASS_NS(className, ns)\
	THOR_FORWARD_CLASS(className);\
	THOR_DECL_TYPE(ns::className);\
	THOR_DECLARE_HANDLE(className);

#define THOR_DECLARE_STRUCT_NS(structName, ns)\
	THOR_FORWARD_STRUCT(structName);\
	THOR_DECL_TYPE(ns::structName);\
	THOR_DECLARE_HANDLE(structName);

#define THOR_DECLARE_HDL_CLASS_NS(className, ns)\
class className;\
	THOR_DECL_TYPE(ns::className);\
	THOR_DECLARE_HANDLE(className);

#define THOR_DECLARE_HDL_STRUCT_NS(structName, ns)\
struct structName;\
	THOR_DECL_TYPE(ns::structName);\
	THOR_DECLARE_HANDLE(structName);

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
	virtual ThUUID	GetUUID()const;
};

}