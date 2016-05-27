#include <Thor/Core/Filesystem/ThiFileStream.h>
#include <functional>

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

}//Thor 