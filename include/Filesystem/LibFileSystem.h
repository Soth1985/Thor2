#ifndef THOR_LIB_FILESYSTEM_H
#define THOR_LIB_FILESYSTEM_H

#include <Thor/Framework/FileSystem/ThiFileSystem.h>
#include <Thor/Framework/FileSystem/ThiDataStream.h>
#include <Thor/Framework/FileSystem/ThiFileStream.h>
#include <Thor/Framework/FileSystem/ThiMemoryStream.h>
#include <Thor/Framework/FileSystem/FSExceptions.h>
#include <Thor/Framework/FileSystem/LoggerFileOutput.h>
#include <Thor/Framework/Singleton.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					eFileSystemType
//
//----------------------------------------------------------------------------------------
struct eFileSystemType
{
	enum Val
	{
		PhysFS,
		BoostFS
	};
};
//----------------------------------------------------------------------------------------
//
//					ThFileSystemFactory
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThFileSystemFactory:public NonCopyable
{
public:
	ThiFileSystemPtr CreateFileSystem(eFileSystemType::Val type);

	static ThFileSystemFactory& Instance();
protected:

	friend class Singleton<ThFileSystemFactory>;
	typedef Singleton<ThFileSystemFactory> singleton_t;

	ThFileSystemFactory();	
};

}//Thor
#endif