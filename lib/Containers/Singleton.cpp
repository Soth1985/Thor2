#include <Thor/Framework/Singleton.h>

namespace Thor
{
	namespace Private
	{
	
	/*LongevityObjectBase::~LongevityObjectBase()
	{}*/
	
	bool LongevityManager::Less( const pair_t& a, const pair_t& b )
	{
		return a.first < b.first;
	};

	void LongevityManager::Register( unsigned int longevity, LongevityObjectBase* ptr )
	{
		longevityObj.push_back( pair_t( longevity,ptr ) );
		std::sort( longevityObj.begin(), longevityObj.end(), &Less );
		
	};

	LongevityManager::~LongevityManager()
	{
		list_t::iterator i;

		for (i = longevityObj.begin(); i != longevityObj.end(); ++i)
		{
			i->second->Release();
			delete i->second;
		}
	}

	LongevityManager& LongevityManager::Instance()
	{
		return Singleton::Instance();
	}

	//LongevityManager LongevityManager::longevityManager;

	//static LongevityManager longevityManager;

	
}//Private
}//Thor