#include <Thor/Core/Filesystem/StlFileStream.h>

namespace Thor
{
THOR_REG_TYPE(ThStlFileStream, THOR_TYPELIST_1(ThiFileStream));
//----------------------------------------------------------------------------------------
//
//					ThStlFileStream
//
//----------------------------------------------------------------------------------------
ThStlFileStream::ThStlFileStream()
	:
ThiFileStream(""),
m_In(&m_Buf),
m_Out(&m_Buf)
{
	//
}
//----------------------------------------------------------------------------------------
ThStlFileStream::ThStlFileStream(const ThString& filename_,eStreamMode::Val streamMode_,eFileWriteMode::Val writeMode_)
	:
ThiFileStream(filename_,streamMode_,writeMode_),
m_In(&m_Buf),
m_Out(&m_Buf)
{
	m_Bitfield=std::ios::binary;

	if(m_Mode==eStreamMode::ReadMode)
		m_Bitfield|=std::ios::in;

	if(m_Mode==eStreamMode::WriteMode)
		m_Bitfield|=std::ios::out;

	if(m_Mode==eStreamMode::ReadWriteMode)
		m_Bitfield|=( std::ios::out | std::ios::in );

	if(writeMode_==eFileWriteMode::Append)
		m_Bitfield|=std::ios::app;

	if(writeMode_==eFileWriteMode::Truncate)
		m_Bitfield|=std::ios::trunc;	
}
//----------------------------------------------------------------------------------------
ThStlFileStream::~ThStlFileStream()
{
	Close();
}
//----------------------------------------------------------------------------------------
void ThStlFileStream::OpenImpl()const
{
	m_Buf.open(m_Filename.c_str(),  m_Bitfield);
}
//----------------------------------------------------------------------------------------
void ThStlFileStream::CloseImpl()const
{
	m_Buf.close();
}
//----------------------------------------------------------------------------------------
ThSize ThStlFileStream::TellImpl()const
{
	//it seems that 'out' file marker is set automatically as we set it in 'in'
	m_In.clear();
	return m_In.tellg();
}
//----------------------------------------------------------------------------------------
void ThStlFileStream::SeekImpl(ThSize pos)const
{
	m_In.clear();
	m_In.seekg(pos);
}
//----------------------------------------------------------------------------------------
ThSize ThStlFileStream::GetSizeImpl()const
{
	ThSize curPos = Tell();
	m_In.clear();
	m_In.seekg(0, std::ios::end);
	ThSize sz=m_In.tellg();
	m_In.seekg(curPos);
	return sz;
}
//----------------------------------------------------------------------------------------
ThBool ThStlFileStream::EofImpl()const
{
	//return in.tellg()==in.seekg(0,std::ios::end);
	return m_In.eof();
}
//----------------------------------------------------------------------------------------
ThSize ThStlFileStream::ReadImpl(void* buf, ThSize bytes)const
{
	ThSize f=m_In.tellg();
	m_In.read(static_cast<char*>(buf),bytes);
	//in.clear();
	ThSize l=m_In.tellg();			
	assert( (l-f)== bytes);
	return l-f;
}
//----------------------------------------------------------------------------------------
ThSize ThStlFileStream::WriteImpl(const void* buf, ThSize bytes)
{
	ThSize f=m_Out.tellp();
	m_Out.write(static_cast<const char*>(buf),bytes);
	ThSize l=m_Out.tellp();
	//assert( (l-f)== bytes);
	return l-f;
}
//----------------------------------------------------------------------------------------
void ThStlFileStream::Flush()
{
	m_Out.flush();
}
//----------------------------------------------------------------------------------------
ThiType* ThStlFileStream::GetType()const
{
	return Thor::GetType<ThStlFileStream>();
}

}//Thor