#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{

template <class T>
class ThFlags
{
public:
	void	SetFlag(bool state, T flag)
	{

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
        m_BitField |= f;
	}

	void	Substract(const ThFlags& f)
	{
		Substract(f.GetBitField());
	}

	void	Substract(T f)
	{
		for(unsigned int i=0; i < NumBits(); ++i)
		{
			T b = 1 << i;
			if( b & m_BitField )
			{
				SetBit(false, i);
			}
		}
	}

	bool	CheckFlag(T flag)const
	{
		return ( m_BitField & flag ) != 0;
	}

	void	SetBit(bool state, unsigned int bit)
	{
		T b = 1 << bit;

		if(state)
			m_BitField |= b;
		else
			m_BitField &= ~b;		
	}

	bool	CheckBit(unsigned int bit)const
	{
		T b = 1 << bit;
		return ( m_BitField & b ) != 0;	
	}

	void	Reset()
	{
		m_BitField=0;
	}

	unsigned int NumBits()const
	{
		return	sizeof(m_BitField) * 8;
	}

	const T& GetBitField()const
	{
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

typedef ThFlags<ThU8> ThFlags8;

}//Thor
