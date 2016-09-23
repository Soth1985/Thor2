#include <Thor/Core/Filesystem/PhysFileStream.h>
#include <Thor/Core/String/ThStringUtilities.h>

namespace Thor
{

THOR_REG_TYPE(ThPhysFileStream, THOR_TYPELIST_1(ThiFileStream));
//----------------------------------------------------------------------------------------
//
//					ThPhysFileStream
//
//----------------------------------------------------------------------------------------
ThPhysFileStream::ThPhysFileStream()
	:
ThiFileStream("")
{

}
//----------------------------------------------------------------------------------------
ThPhysFileStream::ThPhysFileStream(const ThString& filename_,eStreamMode::Val streamMode_,eFileWriteMode::Val writeMode_)
	:
ThiFileStream( filename_ , streamMode_, writeMode_),
m_VirtualName(filename_),
m_Fp(0)
{
	const char* p=PHYSFS_getRealDir( m_VirtualName.c_str() );
	if( p )
		m_Filename=ThString(p)+m_VirtualName;
	else
	{
		p=PHYSFS_getWriteDir();
		assert(p);
		m_Filename=ThString(p)+m_VirtualName;
	}
}
//----------------------------------------------------------------------------------------
ThPhysFileStream::~ThPhysFileStream()
{
	Close();
}	
//----------------------------------------------------------------------------------------
void ThPhysFileStream::OpenImpl()const
{
	if(m_Mode==eStreamMode::ReadMode)
		m_Fp=PHYSFS_openRead( m_VirtualName.c_str() );

	if(m_Mode==eStreamMode::WriteMode || m_Mode==eStreamMode::ReadWriteMode)
	{
		if(m_WriteMode==eFileWriteMode::Append)
			m_Fp=PHYSFS_openAppend( m_VirtualName.c_str() );
		else
			m_Fp=PHYSFS_openWrite( m_VirtualName.c_str() );
	}

	if (!m_Fp)
	{
		ThWideString fname = ThStringUtilities::Utf8ToWideString(m_VirtualName);
		ThWideString error = ThStringUtilities::Utf8ToWideString(PHYSFS_getLastError());
		THOR_ERR_W("Failed to open file %ls , error = %ls", coreSysLogTag, fname.c_str(), error.c_str());
		THOR_ASSERT(0, "Failed to open file.");
	}
}
//----------------------------------------------------------------------------------------
void ThPhysFileStream::CloseImpl()const
{
	if( !PHYSFS_close(m_Fp) )
	{
		//THOR_LOG(PHYSFS_getLastError());
	}
}
//----------------------------------------------------------------------------------------
ThSize ThPhysFileStream::TellImpl()const
{
	ThSize pos=ThSize( PHYSFS_tell(m_Fp) );
	assert( pos!=-1 && ( PHYSFS_getLastError() || 1 ) );
	return pos;
}
//----------------------------------------------------------------------------------------
void ThPhysFileStream::SeekImpl(ThSize pos)
{
	int res=PHYSFS_seek(m_Fp, pos);
	assert( res!=0 && ( PHYSFS_getLastError() || 1 ) );
}
//----------------------------------------------------------------------------------------
ThSize ThPhysFileStream::GetSizeImpl()const
{
	ThSize sz=ThSize( PHYSFS_fileLength(m_Fp) );
	assert( sz!=-1 && ( PHYSFS_getLastError() || 1 ) );
	return sz;
}
//----------------------------------------------------------------------------------------
bool ThPhysFileStream::EofImpl()const
{
	//return in.tellg()==in.seekg(0,std::ios::end);
	return ( PHYSFS_eof(m_Fp)!=0 );
}
//----------------------------------------------------------------------------------------
ThSize ThPhysFileStream::ReadImpl(void* buf, ThSize bytes)const
{
	ThSize sz=(ThSize) PHYSFS_read(m_Fp, buf, 1, (PHYSFS_uint32)bytes) ;
	assert( sz!=-1 && ( PHYSFS_getLastError() || 1 ) );
	return sz;
}
//----------------------------------------------------------------------------------------
size_t ThPhysFileStream::WriteImpl(const void* buf, size_t bytes)
{
	ThSize sz=(ThSize)PHYSFS_write(m_Fp, buf, 1, (PHYSFS_uint32)bytes);
	assert( sz!=-1 && ( PHYSFS_getLastError() || 1 ) );
	return sz;
}
//----------------------------------------------------------------------------------------
void ThPhysFileStream::Flush()
{
	PHYSFS_flush(m_Fp);
}
//----------------------------------------------------------------------------------------
ThiType* ThPhysFileStream::GetType()const
{
	return Thor::TypeOf<ThPhysFileStream>();
}

}//Thor