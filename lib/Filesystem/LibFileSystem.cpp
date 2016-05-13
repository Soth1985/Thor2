#include <Thor/Framework/FileSystem/LibFileSystem.h>

#ifdef THOR_USE_PHYSFS
	#include <Thor/Framework/FileSystem/PhysFileStream.h>
	#include <Thor/Framework/FileSystem/PhysFileSystem.h>
#endif

#ifdef THOR_USE_BOOSTFS
	#include <Thor/Framework/FileSystem/StlFileStream.h>
	#include <Thor/Framework/FileSystem/BoostFileSystem.h>
#endif

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThFileSystemFactory
//
//----------------------------------------------------------------------------------------
ThFileSystemFactory& ThFileSystemFactory::Instance()
{
	return singleton_t::Instance();
}
//----------------------------------------------------------------------------------------
ThiFileSystemPtr ThFileSystemFactory::CreateFileSystem(eFileSystemType::Val type)
{
#ifdef THOR_USE_BOOSTFS
	if(type==eFileSystemType::BoostFS)
		return ThiFileSystemPtr(new ThBoostFileSystem() );
#endif

#ifdef THOR_USE_PHYSFS
	if(type==eFileSystemType::PhysFS)
		return ThiFileSystemPtr(new ThPhysFileSystem());
#endif    
	return ThiFileSystemPtr(0);
}
//----------------------------------------------------------------------------------------
ThFileSystemFactory::ThFileSystemFactory()
{
	//
}

}//Thor