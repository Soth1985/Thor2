#ifndef THOR_FS_THIFILESTREAM_H
#define THOR_FS_THIFILESTREAM_H

#include <Thor/Framework/Filesystem/FSLibConfig.h>
#include <Thor/Framework/Filesystem/ThiDataStream.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

namespace Thor{	

THOR_INLINE ThString FileExtension( const ThString& fileName )
{
	size_t pos=fileName.find_last_of(".");
	if( pos!=ThString::npos )
		return ThString( &fileName[pos+1], &fileName[fileName.length()] );
	else
		return ThString();
}

//----------------------------------------------------------------------------------------
//
//					ThiFileStream
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* A stream for handling file IO, base class for implementation of
* streams dealing with FILE*, std::fstream, etc.
*
*/
class THOR_FRAMEWORK_DLL ThiFileStream:public ThiDataStream
{
public:		

	ThiFileStream(const ThString& filename_,eStreamMode::Val streamMode_=eStreamMode::ReadMode,eFileWriteMode::Val writeMode_=eFileWriteMode::Append);		

	eFileWriteMode::Val GetWriteMode()const;

	const ThString& GetFileName()const;

	virtual ThiAsyncResultPtr BeginRead(void* buf, ThSize bufOffset, ThSize size, const ThiAsyncResultCallback& callback);

	virtual ThiAsyncResultPtr BeginWrite(const void* buf, ThSize bufOffset, ThSize size, const ThiAsyncResultCallback& callback);
	
protected:
	/*!
	* \brief
	* Name of the file.
	*/	
	ThString m_Filename;

	/*!
	* \brief
	* File write mode.
	*/	
	eFileWriteMode::Val m_WriteMode;
};	

}//Thor

#endif  