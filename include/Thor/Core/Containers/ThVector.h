#pragma once

#include <Thor/Core/Memory/ThMemory.h>
#include <Thor/Core/Debug/ThAssert.h>
#include <Thor/Core/ThFlags.h>

#include <type_traits>

namespace Thor {
//----------------------------------------------------------------------------------------
//
//					eThVectorFlags
//
//----------------------------------------------------------------------------------------
struct eThVectorFlags
{
	enum Val
	{
		eStaticArray = 1 << 0,
		eExternalStorageBuffer = 1 << 1,
		eCallCopyConstructorOnMove = 1 << 2,
		//eCallDestructorOnMove = 1 << 3
	};
};
    
namespace Private
{
    template <bool IsClass, class SizeType, class Pointer>
    struct DestroyRangeImpl;
    
    template <class SizeType, class Pointer>
    struct DestroyRangeImpl<true, SizeType, Pointer>
    {
        void operator()(SizeType position, SizeType numElems, Pointer target)
        {
            for (SizeType i = position; i < position + numElems; ++i)
            {
                Pointer temp = &target[i];
                
                temp->~T();
            }
        }
    };
    
    template <class SizeType, class Pointer>
    struct DestroyRangeImpl<false, SizeType, Pointer>
    {
        void operator()(SizeType position, SizeType numElems, Pointer target)
        {
            
        }
    };
    
    template <bool IsClass, class SizeType, class Pointer>
    struct DefaultConstructRangeImpl;
    
    template <class SizeType, class Pointer>
    struct DefaultConstructRangeImpl<true, SizeType, Pointer>
    {
        void operator()(SizeType numElems, Pointer start)
        {
            for (SizeType i = 0; i < numElems; ++i)
            {
                new(start) typename std::remove_pointer<Pointer>::type();
                
                ++start;
            }
        }
    };
    
    
    template <class SizeType, class Pointer>
    struct DefaultConstructRangeImpl<false, SizeType, Pointer>
    {
        void operator()(SizeType numElems, Pointer start)
        {
            
        }
    };
}
    
    //----------------------------------------------------------------------------------------
//
//					ThVector
//
//----------------------------------------------------------------------------------------
template <class T>
class ThVector 
{
public:
	// types:
	typedef T* Pointer;
	typedef const Pointer ConstPointer;
	typedef T ValueType;
	typedef ValueType& Reference;
	typedef const ValueType& ConstReference;
	typedef Pointer Iterator; 
	typedef const T* ConstIterator;
	typedef ThSize SizeType;
	typedef ThSize DifferenceType;
		
	//construct/copy/destroy:
	explicit ThVector()
		:
	m_Data(0),
	m_Size(0),
	m_Capacity(0),
	m_Flags(eThVectorFlags::eCallCopyConstructorOnMove)
	{

	}

	explicit ThVector(SizeType n, const ThFlags32& flags = 0)
		:
	m_Size(0),
	m_Data(0),
	m_Capacity(0),
	m_Flags(eThVectorFlags::eCallCopyConstructorOnMove)
	{
		if (flags.GetBitField() != 0)
			m_Flags = flags;

		Resize(n);
	}

	ThVector(SizeType n, const T& value, const ThFlags32& flags = 0)
		:
	m_Size(0),
	m_Data(0),
	m_Capacity(0),
	m_Flags(eThVectorFlags::eCallCopyConstructorOnMove)
	{
		if (flags.GetBitField() != 0)
			m_Flags = flags;

		Resize(n, value);
	}

	ThVector(Pointer data, SizeType size, const ThFlags32& flags = 0)
		:
	m_Data(data),
	m_Size(size),
	m_Capacity(size),
	m_Flags(eThVectorFlags::eCallCopyConstructorOnMove)
	{
		if (flags.GetBitField() != 0)
			m_Flags = flags;
	}

	template <class InputIterator>
	ThVector(InputIterator first, InputIterator last, const ThFlags32& flags = 0)
		:
	m_Size(0),
	m_Data(0),
	m_Capacity(0),
	m_Flags(eThVectorFlags::eCallCopyConstructorOnMove)
	{
		if (flags.GetBitField() != 0)
			m_Flags = flags;

		Assign(first, last);
	}

	ThVector(const ThVector& v)
		:
	m_Size(0),
	m_Data(0),
	m_Capacity(0),
	m_Flags(eThVectorFlags::eCallCopyConstructorOnMove)
	{
		*this = v;
	}

	~ThVector()
	{
		FreeMemory();
	}

	ThVector& operator=(const ThVector& x)
	{
		if (!x.Empty())
			Assign(x.Begin(), x.End());
		else
			FreeMemory();
		
		return *this;
	}

	template <class InputIterator>
	void Assign(InputIterator first, InputIterator last)
	{
		FreeMemory();

		SizeType size = last - first;
		Reserve(size);
		m_Size = size;

		ConstructRange(first, last, m_Data);
	}
		
	void Assign(SizeType n, const T& value)
	{
		FreeMemory();
		Resize(n, value);
	}
	// iterators:
	Iterator Begin()
	{
		return m_Data;
	}

	ConstIterator Begin() const
	{
		return m_Data;
	}

	ConstIterator CBegin() const
	{
		return m_Data;
	}

	Iterator End()
	{
		return m_Data + m_Size;
	}

	ConstIterator End() const
	{
		return m_Data + m_Size;
	}

	ConstIterator CEnd() const
	{
		return m_Data + m_Size;
	}

	// capacity:
	ThSize Size()const
	{
		return m_Size;
	}
		
	ThSize MaxSize()const
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
			return m_Capacity;
		else
		{
			ThSize result = 0;
			return ~result;
		}
	}
		
	void Resize(SizeType size)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector size");
			return;
		}

		if (size > Size())
			Insert(End(), size - Size(), T());
		else if (size < Size())
			Erase(Begin() + size, End());
	}

	void Resize(SizeType size, const T& value)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector size");
			return;
		}

		if (size > Size())
			Insert(End(), size - Size(), value);
		else if (size < Size())
			Erase(Begin() + size, End());
	}
		
	SizeType Capacity()const
	{
		return m_Capacity;		
	}

	ThBool Empty()const
	{
		return m_Size == 0;
	}

	void Reserve(SizeType n)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector capacity");
			return;
		}

		if (n != m_Capacity)
		{
			SizeType bound = Thor::Min(n, m_Size);

			T* prevBuffer = Realloc(n);

			if (m_Data && prevBuffer)
			{
				if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
				{
					ConstructRange(&prevBuffer[0], &prevBuffer[bound], m_Data);
					DestroyRange(0, bound, prevBuffer);
				}
				else
					ThMemory::MemoryCopy(m_Data, prevBuffer, bound * sizeof(T));

				//if (m_Flags.CheckFlag(eThVectorFlags::eCallDestructorOnMove))
				//	DestroyRange(0, m_Size, prevBuffer);
			}

			if (!m_Flags.CheckFlag(eThVectorFlags::eExternalStorageBuffer))
			{
				SafeDelete(prevBuffer);
			}

			m_Flags.SetFlag(false, eThVectorFlags::eExternalStorageBuffer);
		}
	}

	void ShrinkToFit()
	{
		Reserve(m_Size);
	}

	// element access:
	Reference operator[](SizeType n)
	{
		THOR_ASSERT(n < m_Size, "Index out of bounds");

		return m_Data[n]; 
	}

	ConstReference operator[](SizeType n)const
	{
		THOR_ASSERT(n < m_Size, "Index out of bounds");

		return m_Data[n]; 
	}

	Reference Front()
	{
		THOR_ASSERT(0 < m_Size, "Index out of bounds");

		return m_Data[0];
	}

	ConstReference Front() const
	{
		THOR_ASSERT(0 < m_Size, "Index out of bounds");

		return m_Data[0];
	}

	Reference Back()
	{
		THOR_ASSERT(0 < m_Size, "Index out of bounds");

		return m_Data[m_Size - 1];
	}

	ConstReference Back()const
	{
		THOR_ASSERT(0 < m_Size, "Index out of bounds");

		return m_Data[m_Size - 1];
	}
	// data access
	Pointer Data()
	{
		return m_Data;
	}

	const Pointer Data() const
	{
		return m_Data;
	}
	// modifiers:
	void PushBack(const T& x)
	{
		Insert(End(), x);
	}

	void PopBack()
	{
		if (m_Size)
			Erase(&m_Data[m_Size - 1]);
	}

	Iterator Insert(ConstIterator position, const T& value)
	{
		const T* ptr = &value;
		return Insert(position, ptr, ptr + 1);
	}

	Iterator Insert(ConstIterator position, SizeType n, const T& value)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector");
			return Iterator(position);
		}

		SizeType newSize = m_Size + n;
		SizeType index = position - m_Data;

		T* prevBuffer = InsertShared(position, n);
		ConstructRange(index, n, value);

		if (prevBuffer != m_Data)
		{
			if (!m_Flags.CheckFlag(eThVectorFlags::eExternalStorageBuffer))
			{
				SafeDelete(prevBuffer);
			}

			m_Flags.SetFlag(false, eThVectorFlags::eExternalStorageBuffer);
		}

		m_Size = newSize;
		return m_Data + index;
	}

	template <class InputIterator>
	Iterator Insert(ConstIterator position, InputIterator first, InputIterator last)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector");
			return Iterator(position);
		}

		SizeType newSize = m_Size + last - first;
		SizeType index = position - m_Data;

		T* prevBuffer = InsertShared(position, last - first);
		ConstructRange(first, last, m_Data + index);

		if (prevBuffer && prevBuffer != m_Data)
		{
			if (!m_Flags.CheckFlag(eThVectorFlags::eExternalStorageBuffer))
			{
				SafeDelete(prevBuffer);
			}

			m_Flags.SetFlag(false, eThVectorFlags::eExternalStorageBuffer);
		}

		m_Size = newSize;
		return m_Data + index;
	}

	Iterator Erase(ConstIterator position)
	{
		return Erase(position, position + 1);
	}

	Iterator Erase(ConstIterator first, ConstIterator last)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector");
			return Iterator(first);
		}

		SizeType numElems = last - first;
		SizeType firstIndex = first - m_Data;
		SizeType lastIndex = last - m_Data;
		
		if (numElems == m_Size)
			Clear();
		else
		{
			SizeType newSize = m_Size - numElems;		

			T* prevBuffer = Realloc(newSize);

			if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
			{
				ConstructRange(&prevBuffer[0], &prevBuffer[firstIndex], m_Data);
				ConstructRange(&prevBuffer[lastIndex], &prevBuffer[m_Size], &m_Data[firstIndex]);
			}
			else
			{
				ThMemory::MemoryCopy(m_Data, prevBuffer, firstIndex * sizeof (T));
				ThMemory::MemoryCopy(&m_Data[firstIndex], &prevBuffer[lastIndex], (m_Size - lastIndex) * sizeof (T));
			}

			if (!m_Flags.CheckFlag(eThVectorFlags::eExternalStorageBuffer))
			{
				DestroyRange(0, m_Size, prevBuffer);
				SafeDelete(prevBuffer);				
			}

			m_Flags.SetFlag(false, eThVectorFlags::eExternalStorageBuffer);

			m_Size = newSize;
		}

		return &m_Data[firstIndex + 1];
	}
		
	void Swap(ThVector& rhs)
	{
		Thor::Swap(m_Capacity, rhs.m_Capacity);
		Thor::Swap(m_Data, rhs.m_Data);
		Thor::Swap(m_Flags, rhs.m_Flags);
		Thor::Swap(m_Size, rhs.m_Size);
	}

	void MoveToBackAndRemove(SizeType index)
	{
		if (m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector");
			return;
		}

		if (m_Size > 1)
		{
			DestroyRange(index, 1, m_Data);

			if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
				ConstructRange(&m_Data[m_Size - 1], &m_Data[m_Size], &m_Data[index]);
			else
				ThMemory::MemoryCopy(&m_Data[index], &m_Data[m_Size - 1], sizeof (T));

			DestroyRange(m_Size - 1, 1, m_Data);

			--m_Size;
		}
		else
		{
			DestroyRange(0, 1, m_Data);
			m_Size = 0;
		}
	}

	void MoveToBackAndRemove(Iterator position)
	{
		MoveToBackAndRemove(position - m_Data);
	}
		
	void Clear()
	{
		FreeMemory();
	}

	//extensions
	const ThFlags32& GetFlags()const
	{
		return m_Flags;
	}

private:
	Pointer		m_Data;
	SizeType	m_Size;
	SizeType	m_Capacity;
	ThFlags32	m_Flags;

	void DestroyRange(SizeType position, SizeType numElems, Pointer target)
	{
        Private::DestroyRangeImpl< std::is_class<T>::value, SizeType, Pointer >(position, numElems, target);
	}

	void ConstructRange(SizeType position, SizeType numElems)
	{
		Private::DefaultConstructRangeImpl<std::is_class<T>::value, SizeType, Pointer>(numElems, &m_Data[position]);
	}

	void ConstructRange(SizeType position, SizeType numElems, const T& value)
	{
		Pointer start = &m_Data[position];

		for (SizeType i = position; i < position + numElems; ++i)
		{
			new(start) T(value);

			++start;
		}
	}

	template <class InputIterator>
	void ConstructRange(InputIterator first, InputIterator last, Pointer target)
	{
		Pointer memory = target;
		InputIterator curIter = first;

		for (; curIter < last; ++curIter)
		{
			new(memory) T(*curIter);

			++memory;
		}
	}

	T* Realloc(SizeType n)
	{
		if (m_Flags.CheckBit(eThVectorFlags::eStaticArray))
		{
			THOR_ASSERT(0, "Cannot modify static vector capacity");
			return m_Data;
		}

		T* prevBuffer = m_Data;

		if (n != m_Capacity)
		{
			ThI8* newBuffer = 0;

			if (n)
			{
				newBuffer = (ThI8*)ThMemory::Malloc(n * sizeof (T));
				//newBuffer = new ThI8[ n * sizeof T ];
			}

			m_Data = (T*)newBuffer;
			m_Capacity = n;
		}

		return prevBuffer;
	}

	T* InsertShared(ConstIterator position, SizeType numElems)
	{
		//create a gap numElems wide at position
		SizeType newSize = m_Size + numElems;
		SizeType index = position - m_Data;

		T* prevBuffer = m_Data;

		if (newSize > m_Capacity)
			prevBuffer = Realloc(newSize);

		if (prevBuffer)
		{
			if (m_Data != prevBuffer)
			{
				SizeType gap = Min(m_Size, index);
				
				if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
				{
					ConstructRange(prevBuffer, prevBuffer + gap, m_Data);
					DestroyRange(0, gap, prevBuffer);
				}
				else
					ThMemory::MemoryCopy(m_Data, prevBuffer, gap * sizeof (T));

				if(index + numElems < m_Size)
				{
					if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
					{
						ConstructRange(&prevBuffer[index], &prevBuffer[index + numElems - m_Size], &m_Data[index + numElems]);
						DestroyRange(index, index + numElems - m_Size, prevBuffer);
					}
					else
						ThMemory::MemoryCopy(&m_Data[index + numElems], &prevBuffer[index], (index + numElems - m_Size) * sizeof (T));
				}
			}
			else
			{
				for (SizeType i = index; i < Min(numElems + index, m_Size); ++i)
				{
					m_Data[numElems + i] = m_Data[i];
				}
			}
		}		

		return prevBuffer;
	}

	void FreeMemory()
	{
		if (m_Data && !m_Flags.CheckFlag(eThVectorFlags::eExternalStorageBuffer) && !m_Flags.CheckFlag(eThVectorFlags::eStaticArray))
		{
			DestroyRange(0, m_Size, m_Data);
			SafeDelete(m_Data);
		}

		m_Flags.SetFlag(false, eThVectorFlags::eExternalStorageBuffer);
		m_Flags.SetFlag(false, eThVectorFlags::eStaticArray);

		m_Data = 0;
		m_Capacity = 0;
		m_Size = 0;
	}

	void SafeDelete(T* buffer)
	{
		ThI8* rawBuffer = (ThI8*)buffer;
		ThMemory::Free(rawBuffer);
		//delete[] rawBuffer;
	}
};

template <class T>
ThBool operator==(const ThVector<T>& x, const ThVector<T>& y)
{
	if (x.Size() != y.Size())
	{
		return false;
	}

	for (ThSize i = 0; i < x.Size(); ++i)
	{
		if (x[i] != y[i])
			return false;
	}

	return true;
}

template <class T>
ThBool operator!=(const ThVector<T>& x, const ThVector<T>& y)
{
	return !(x == y);
}

/*template <class T>
ThBool operator< (const ThVector<T>& x, const ThVector<T>& y)
{

}

template <class T>
ThBool operator> (const ThVector<T>& x, const ThVector<T>& y)
{

}

template <class T>
ThBool operator>=(const ThVector<T>& x, const ThVector<T>& y)
{

}
	
template <class T>
ThBool operator<=(const ThVector<T>& x, const ThVector<T>& y)
{

}*/

}//Thor