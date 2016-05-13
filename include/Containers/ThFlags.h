#pragma once

#include <Thor/Framework/ThMutexPolicy.h>

namespace Thor
{

template <class T, bool threadSafe = false>
class ThFlags: public ThMutexPolicy<threadSafe>
{
public:
	void	SetFlag(ThBool state, T flag)
	{
		ScopedLock lock;
		InitScopedLock(lock);

		if(state)
			m_BitField |= flag;
		else
			m_BitField &= ~flag;
	}

	void	Combine(const ThFlags& f)
	{
		Combine(f.GetBitField());
	}

	void	Combine(T f)
	{
		ScopedLock lock;
		InitScopedLock(lock);

		m_BitField |= f;
	}

	void	Substract(const ThFlags& f)
	{
		Substract(f.GetBitField());
	}

	void	Substract(T f)
	{
		ScopedLock lock;
		InitScopedLock(lock);

		for(UINT i=0; i < NumBits(); ++i)
		{
			T b = 1 << i;
			if( b & m_BitField )
			{
				SetBit(false, i);
			}
		}
	}

	ThBool	CheckFlag(T flag)const
	{
		ScopedLock lock;
		InitScopedLock(lock);

		return ( m_BitField & flag ) != 0;
	}

	void	SetBit(ThBool state, unsigned int bit)
	{
		ScopedLock lock;
		InitScopedLock(lock);

		T b = 1 << bit;

		if(state)
			m_BitField |= b;
		else
			m_BitField &= ~b;		
	}

	ThBool	CheckBit(unsigned int bit)const
	{
		ScopedLock lock;
		InitScopedLock(lock);

		T b = 1 << bit;
		return ( m_BitField & b ) != 0;	
	}

	void	Reset()
	{
		ScopedLock lock;
		InitScopedLock(lock);

		m_BitField=0;
	}

	unsigned int NumBits()const
	{
		return	sizeof(m_BitField) * 8;
	}

	const T& GetBitField()const
	{
		ScopedLock lock;
		InitScopedLock(lock);

		return m_BitField;
	}

	ThFlags(T bits)
		:
	m_BitField(bits)
	{}

	ThFlags()
		:
	m_BitField(0)
	{}
private:
	T m_BitField;
};

typedef ThFlags<ThU32> ThFlags32;
typedef ThFlags<ThU32, true> ThFlags32TS;

typedef ThFlags<ThU8> ThFlags8;
typedef ThFlags<ThU8, true> ThFlags8TS;

}//Thor