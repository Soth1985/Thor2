#include <Thor/Core/Rtti/ThRttiManager.h>
#include <Thor/Core/Rtti/ThTypeInfo.h>
#include <Thor/Core/ThException.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThRttiManager
//
//----------------------------------------------------------------------------------------	
ThRttiManager::~ThRttiManager()
{

}
//----------------------------------------------------------------------------------------
void ThRttiManager::UnregisterType(const ThiType* t)
{
	for (TypeMap::Iterator i = m_RegisteredTypes.Begin(); i < m_RegisteredTypes.End(); ++i)
	{
		ThiType* curType = i->Value();
		curType->RemoveType(t);
	}

	bool res = m_RegisteredTypes.Erase(t->GetName());
}
//----------------------------------------------------------------------------------------
void ThRttiManager::RegisterType(const ThiType* t)
{	
	if( !m_RegisteredTypes.Insert( t->GetName(), const_cast<ThiType*>(t)) )
	{
		ThString msg("Attempt to register multiple types under the name ");
		msg += t->GetName();
		throw ThException( msg.c_str() );
	}
}
//----------------------------------------------------------------------------------------
ThiType* ThRttiManager::GetType(const ThI8* name)const
{
	TypeMap::ConstIterator t = m_RegisteredTypes.Find(name);

	if( t != m_RegisteredTypes.End() )
		return t->Value();
	
	return 0;
}
//----------------------------------------------------------------------------------------
ThRttiManager& ThRttiManager::Instance()
{
	return SingletonT::Instance();
}

}//Thor
