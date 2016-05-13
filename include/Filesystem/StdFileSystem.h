#ifndef THOR_FS_BOOST_FILESYSTEM_H
#define THOR_FS_BOOST_FILESYSTEM_H

#include <Thor/Framework/Filesystem/FSLibConfig.h>
#include <Thor/Framework/Filesystem/StlFileStream.h>
#include <Thor/Framework/Filesystem/ThiFileSystem.h>
#include <Thor/Framework/Filesystem/FSExceptions.h>
#include <Thor/Framework/RefPtr.h>
#include <filesystem>
#include <tbb/tbb_thread.h>

namespace Thor{

namespace fs=std::tr2::sys;
//----------------------------------------------------------------------------------------
//
//					ThBoostFileSystem
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThBoostFileSystem: public ThiFileSystem
{

public:

	ThBoostFileSystem();

	/*!
	* \copydoc FileSystem::MountDir
	*/
	virtual void MountDir(const ThString& path, const ThString& target=ThString(), eMountMode::Val mode=eMountMode::ReadOnly);

	/*!
	* \copydoc FileSystem::DismountDir
	*/
	virtual void DismountDir(const ThString& path);

	/*!
	* \copydoc FileSystem::Exists
	*/
	virtual ThBool Exists(const ThString& path)const;	

	/*!
	* \copydoc FileSystem::Enumerate
	*/
	virtual string_list Enumerate(const ThString& path);
	
	/*!
	* \copydoc FileSystem::GetSearchPaths
	*/
	virtual string_list GetSearchPaths();

	/*!
	* \copydoc FileSystem::MakeDirectory
	*/
	virtual void MakeDirectory(const ThString& path);

	/*!
	* \copydoc FileSystem::OpenFile
	*/
	virtual ThiFileStreamPtr OpenFile(const ThString& filename, eStreamMode::Val streamMode=eStreamMode::ReadMode,eFileWriteMode::Val writeMode=eFileWriteMode::Append);
	
	/*!
	* \copydoc FileSystem::Delete
	*/
	virtual void Delete(const ThString& path);

	/*!
	* \copydoc FileSystem::IsDirectory
	*/
	virtual ThBool IsDirectory(const ThString& path);

	/*!
	* \copydoc FileSystem::GetFullPath
	*/
	virtual ThString GetFullPath(const ThString& path)const;

	/*!
	* \copydoc FileSystem::GetInitialPath
	*/
	virtual ThString GetInitialPath()const;

	virtual ThiType*	GetType()const;

private:

	string_list EnumerateDir(const ThString& path);

	/*!
	 * \brief
	 * Tests for file existance.
	 * 
	 * \param path
	 * Tested path(may be not full path).
	 * 
	 * \param res
	 * Full path if found.
	 * 
	 * \returns
	 * True if the file is found.
	 * 
	 */
	ThBool ExistsIn(const ThString& path, ThString& res)const;

	struct Path
	{
		fs::path path;
		//unsigned int mode;

		Path(const fs::path& path_)
			:path(path_)
		{}
	};

	typedef tbb::spin_mutex mutex_t;
	typedef std::vector< Path > path_list_t;
	
	/*!
	* \brief
	* Mutex for thread safety.
	*/
	mutable mutex_t m_Mutex;

	/*!
	* \brief
	* Read-only search path list.
	*/
	path_list_t		m_PathList;
	
	/*!
	* \brief
	* Write directory path.
	*/		
	fs::path		m_WriteDir;
};

}//Thor

#endif