#ifndef THOR_FS_EXCEPTIONS_H
#define THOR_FS_EXCEPTIONS_H

#include <Thor/Framework/Filesystem/FSLibConfig.h>
#include <Thor/Framework/ThException.h>

namespace Thor{
//----------------------------------------------------------------------------------------
//
//					FileSystemException
//
//----------------------------------------------------------------------------------------
class FileSystemException:public ThException
{
public:
	explicit FileSystemException( const ThString& str ): ThException( str ){};
};
//----------------------------------------------------------------------------------------
//
//					MountFailedException
//
//----------------------------------------------------------------------------------------
class MountFailedException:public FileSystemException
{
public:
	explicit MountFailedException( const ThString& str ): FileSystemException( ThString("Failed to mount directory: ") + str ){};
};
//----------------------------------------------------------------------------------------
//
//					MakeDirException
//
//----------------------------------------------------------------------------------------
class MakeDirException:public FileSystemException
{
public:
	explicit MakeDirException( const ThString& str ): FileSystemException( ThString("Failed to make directory: ") + str ){};
};
//----------------------------------------------------------------------------------------
//
//					OpenFileException
//
//----------------------------------------------------------------------------------------
class OpenFileException:public FileSystemException
{
public:
	explicit OpenFileException( const ThString& str ): FileSystemException( ThString("Failed to open file: ") + str ){};
};
//----------------------------------------------------------------------------------------
//
//					DeleteFileException
//
//----------------------------------------------------------------------------------------
class DeleteFileException:public FileSystemException
{
public:
	explicit DeleteFileException( const ThString& str ): FileSystemException( ThString("Failed to delete file or directory: ") + str ){};
};
//----------------------------------------------------------------------------------------
//
//					FileSystemInitException
//
//----------------------------------------------------------------------------------------
class FileSystemInitException:public FileSystemException
{
public:
	explicit FileSystemInitException( const ThString& str ): FileSystemException(ThString("Failed to init filesystem ") + str ){};
};
//----------------------------------------------------------------------------------------
//
//					CodecNotFoundException
//
//----------------------------------------------------------------------------------------
class CodecNotFoundException:public FileSystemException
{
public:
	explicit CodecNotFoundException( const ThString& str ): FileSystemException( ThString("The following codec was not found: ") + str ){};
};
//----------------------------------------------------------------------------------------
//
//					FileDoesNotExistException
//
//----------------------------------------------------------------------------------------
class FileDoesNotExistException:public FileSystemException
{
public:
	explicit FileDoesNotExistException( const ThString& str ): FileSystemException( ThString("The following file was not found: ") + str ){};
};
	
}//Thor

#endif