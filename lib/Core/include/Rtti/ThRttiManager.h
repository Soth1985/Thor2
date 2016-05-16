#pragma once

#include <string>
#include <unordered_map>
#include <Thor/Framework/FrameworkExport.h>
#include <Thor/Framework/Singleton.h>
#include <Thor/Framework/Containers/ThHashMap.h>

namespace Thor
{

class ThiType;
//----------------------------------------------------------------------------------------
//
//					ThRttiManager
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThRttiManager
{
public:

	~ThRttiManager();

	void UnregisterType(const ThiType* t);

	void RegisterType(const ThiType* t);	

	ThiType* GetType(const ThI8* name)const;

	static ThRttiManager& Instance();

private:

	typedef SingletonWithLongevity<ThRttiManager, eSingletonsLongevity::RttiManager> SingletonT;
	friend class SingletonT;
	typedef ThHashMap<const ThI8*, ThiType*> TypeMap;	

	TypeMap m_RegisteredTypes;	
};

}//Thor