#include <Thor/Core/Filesystem/ThiMemoryStream.h>

namespace Thor
{

THOR_REG_TYPE(ThiMemoryStream, THOR_TYPELIST_1(ThiDataStream));
//----------------------------------------------------------------------------------------
//
//					ThMemoryStream
//
//----------------------------------------------------------------------------------------		
ThiMemoryStream::ThiMemoryStream(ThU8* beg_, ThSize size, eStreamMode::Val mode)
	:
ThiDataStream(mode,true),
m_Beg(beg_),
m_Cur(beg_),
m_End(beg_+size),
m_FreeMem(false)
{
	//
};
//----------------------------------------------------------------------------------------
ThiMemoryStream::ThiMemoryStream(ThSize size, eStreamMode::Val mode)
	:
ThiDataStream(mode,true),
m_FreeMem(true)
{
	m_Beg = new ThU8[size];
	m_Cur = m_Beg;
	m_End = m_Beg + size;
};
//----------------------------------------------------------------------------------------
ThiMemoryStream::ThiMemoryStream(eStreamMode::Val mode)
	:
ThiDataStream(mode, true),
m_Beg(0),
m_Cur(0),
m_End(0),
m_FreeMem(true)
{
	//
}
//----------------------------------------------------------------------------------------
ThiMemoryStream::~ThiMemoryStream()
{
	if(m_Beg && m_FreeMem)
		delete[] m_Beg;
}
//----------------------------------------------------------------------------------------
ThSize ThiMemoryStream::TellImpl()const
{
	return m_Cur-m_Beg;
}
//----------------------------------------------------------------------------------------
void ThiMemoryStream::SeekImpl(ThSize pos)
{
	assert( (m_Beg + pos) < m_End );
	m_Beg += pos;
	//return cur;
}
//----------------------------------------------------------------------------------------
ThSize ThiMemoryStream::GetSizeImpl()const
{
	return m_End-m_Beg;
}
//----------------------------------------------------------------------------------------
bool ThiMemoryStream::EofImpl()const
{
	return m_Cur >= m_End;
}
//----------------------------------------------------------------------------------------
ThSize ThiMemoryStream::ReadImpl(void* buf, ThSize bytes)const
{
	assert( (m_Cur + bytes) <= m_End );
	memcpy(buf, m_Cur, bytes);
	m_Cur += bytes;
	return bytes;
}
//----------------------------------------------------------------------------------------
ThSize ThiMemoryStream::WriteImpl(const void* buf, ThSize bytes)
{
	ThU8* newPos = m_Cur + bytes;

	if( newPos >= m_End )
		Grow(newPos - m_End + 1);

	memcpy(m_Cur, buf, bytes);
	m_Cur += bytes;
	return bytes;
}
//----------------------------------------------------------------------------------------
void ThiMemoryStream::Flush()
{
	if(m_Beg && m_FreeMem)
		delete[] m_Beg;

	m_Beg=0;
	m_Cur=0;
	m_End=0;
	m_FreeMem=true;
}
//----------------------------------------------------------------------------------------
void ThiMemoryStream::Grow(ThSize addSize)
{
	ThSize oldSize = GetSize();
	ThSize newSize = oldSize + addSize;
	ThSize curPos = Tell();
	
	ThU8* buf = new ThU8[newSize];
	
	if(m_Beg)
	{
		memcpy(buf, m_Beg, oldSize);

		if(m_FreeMem)
			delete[] m_Beg;
	}

	m_FreeMem = true;
	m_Beg = buf;
	m_End = m_Beg + newSize;
	m_Cur = m_Beg + curPos;
}

}//Thor