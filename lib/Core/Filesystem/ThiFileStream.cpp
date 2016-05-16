#include <Thor/Framework/Filesystem/ThiFileStream.h>
#include <Thor/Framework/ThAsyncOpManager.h>
#include <functional>
#include <tbb/tbb_thread.h>
#include <tbb/concurrent_queue.h>

namespace Thor{	

THOR_REG_TYPE(ThiFileStream, THOR_TYPELIST_1(ThiDataStream));

//----------------------------------------------------------------------------------------
//
//					ThiFileStream
//
//----------------------------------------------------------------------------------------
ThiFileStream::ThiFileStream(const ThString& filename_,eStreamMode::Val streamMode_,eFileWriteMode::Val writeMode_)
	:
ThiDataStream(streamMode_,false),
m_WriteMode(writeMode_),
m_Filename(filename_)
{
	//
}		
//---------------------------------------------------------------------------------
eFileWriteMode::Val ThiFileStream::GetWriteMode()const
{
	return m_WriteMode;
};
//---------------------------------------------------------------------------------
const ThString& ThiFileStream::GetFileName()const
{
	return m_Filename;
}
//---------------------------------------------------------------------------------
ThiAsyncResultPtr ThiFileStream::BeginRead(void* buf, ThSize bufOffset, ThSize size, const ThiAsyncResultCallback& callback)
{
	ThiAsyncResultPtr result( new ThFileAsyncResult( true, ThiFileStreamPtr(this), buf, bufOffset, size, callback ) );
	return result;
}
//---------------------------------------------------------------------------------
ThiAsyncResultPtr ThiFileStream::BeginWrite(const void* buf, ThSize bufOffset, ThSize size, const ThiAsyncResultCallback& callback)
{
	ThiAsyncResultPtr result( new ThFileAsyncResult( false, ThiFileStreamPtr(this), buf, bufOffset, size, callback ) );
	return result;
}

}//Thor 