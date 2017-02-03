#pragma once

#include <string>
#include <unordered_map>
#include <Thor/Core/CoreExport.h>
#include <Thor/Core/Containers/Singleton.h>
#include <Thor/Core/Containers/ThHashMap.h>

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

	bool UnregisterType(const ThiType* t);

	void RegisterType(const ThiType* t);	

	ThiType* GetType(const ThChar* name)const;

	static ThRttiManager& Instance();

private:

	typedef SingletonWithLongevity<ThRttiManager, eSingletonsLongevity::RttiManager> SingletonT;
	friend SingletonT;
	typedef ThHashMap<const ThChar*, ThiType*> TypeMap;	

	TypeMap m_RegisteredTypes;	
};

}//Thor
