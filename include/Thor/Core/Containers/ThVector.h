#pragma once

#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThAssert.h>
#include <Thor/Core/ThFlags.h>

#include <type_traits>

namespace Thor {

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
	explicit ThVector(ThiMemoryAllocator* allocator = nullptr)
		:
	m_Data(0),
	m_Size(0),
	m_Capacity(0),
    m_Allocator(allocator)
	{
        if (!m_Allocator)
            m_Allocator = ThAllocators::Instance().GetSystemMemoryAllocator();
	}

	explicit ThVector(SizeType n, ThiMemoryAllocator* allocator = nullptr)
		:
    ThVector(allocator)
	{
        Resize(n);
	}

	ThVector(SizeType n, const T& value, ThiMemoryAllocator* allocator = nullptr)
		:
	ThVector(allocator)
    {
        Resize(n, value);
	}

	ThVector(Pointer data, SizeType size, ThiMemoryAllocator* allocator = nullptr)
		:
	ThVector(allocator),
    m_Data(data),
    m_Size(size),
    m_Capacity(size)
	{
        
	}

	template <class InputIterator>
	ThVector(InputIterator first, InputIterator last, ThiMemoryAllocator* allocator = nullptr)
		:
	ThVector(allocator)
	{
		Assign(first, last);
	}

	ThVector(const ThVector& copy)
		:
    ThVector(copy.GetAllocator())
	{
		*this = copy;
	}
    
    ThVector(ThVector&& copy)
        :
    ThVector(copy.GetAllocator())
    {
        m_Data = copy.m_Data;
        m_Size = copy.m_Size;
        m_Capacity = copy.m_Capacity;
        
        copy.m_Data = nullptr;
        copy.m_Size = 0;
        copy.m_Capacity = 0;
    }

	~ThVector()
	{
		Free();
	}

	ThVector& operator=(const ThVector& x)
	{
        Assign(x.Begin(), x.End());
		return *this;
	}

	template <class InputIterator>
	void Assign(InputIterator first, InputIterator last)
	{
		Free();
        Insert(nullptr, first, last);
	}
		
	void Assign(SizeType n, const T& value)
	{
		Free();
		Resize(n, value);
	}

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

	ThSize Size()const
	{
		return m_Size;
	}
		
	void Resize(SizeType size)
	{
		if (size > Size())
			Insert(End(), size - Size(), T());
		else if (size < Size())
			Erase(Begin() + size, End());
	}

	void Resize(SizeType size, const T& value)
    {
		if (size > Size())
			Insert(End(), size - Size(), value);
		else if (size < Size())
			Erase(Begin() + size, End());
	}
		
	SizeType Capacity()const
	{
		return m_Capacity;		
	}

	bool Empty()const
	{
		return m_Size == 0;
	}

	void Reserve(SizeType n)
	{
		if (n > m_Capacity)
		{
            Pointer newData = (Pointer)m_Allocator->Allocate(n * sizeof(T));
            MoveObjects(m_Data, newData, m_Size);
            m_Allocator->Deallocate(m_Data);
            m_Data = newData;
            m_Capacity = n;
		}
	}

	void ShrinkToFit()
	{
        if (m_Size)
        {
            Pointer newData = m_Allocator->Allocate(m_Size * sizeof(T));
            MoveObjects(m_Data, newData, m_Size);
            m_Allocator->Deallocate(m_Data);
            m_Data = newData;
            m_Capacity = m_Size;
        }
        else
        {
            Free();
        }
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
		return Insert(position, 1, value);
	}

	Iterator Insert(ConstIterator position, SizeType n, const T& value)
	{
        THOR_ASSERT(position >= m_Data, "Invalid position");
        SizeType newSize = m_Size + n;
        SizeType insertIndex = position - m_Data;
        ThSize endIndex = insertIndex + n;
        Reserve(newSize);        
        SizeType toMove = m_Size - insertIndex;
        
        MoveObjects(&m_Data[insertIndex], &m_Data[insertIndex + n], toMove);
        
        for (SizeType idx = insertIndex; idx < endIndex; ++idx)
        {
            CopyObject(&value, &m_Data[idx], 1);
        }
        
        m_Size = newSize;
        return m_Data + insertIndex;
    }

	template <class InputIterator>
	Iterator Insert(ConstIterator position, InputIterator first, InputIterator last)
	{
        THOR_ASSERT(last > first, "Invalid iterators");
        THOR_ASSERT(position >= m_Data, "Invalid position");
        SizeType numElems = last - first;
		SizeType newSize = m_Size + numElems;
        SizeType insertIndex = position - m_Data;
        SizeType toMove = m_Size - insertIndex;
        Reserve(newSize);		
        
        MoveObjects(&m_Data[insertIndex], &m_Data[insertIndex + numElems], toMove);
        
        /*SizeType idx = insertIndex;
        for (InputIterator i = first; i != last; ++i)
        {
            CopyObject(i, &m_Data[idx], 1);
            ++idx;
        }*/
        CopyObjects(first, &m_Data[insertIndex], numElems);

		m_Size = newSize;
		return m_Data + insertIndex;
	}

	Iterator Erase(ConstIterator position)
	{
		return Erase(position, position + 1);
	}

	Iterator Erase(ConstIterator first, ConstIterator last)
	{
        THOR_ASSERT(last > first, "Invalid iterators");
        
        if (Empty())
            return m_Data;
        
		SizeType numElems = last - first;
        
        if (numElems > m_Size)
            numElems = m_Size;
		
        SizeType firstIndex = first - m_Data;
		SizeType lastIndex = last - m_Data;
        SizeType newSize = m_Size - numElems;
        SizeType toMove = 0;
        
        if (lastIndex < m_Size)
            toMove = m_Size - lastIndex;
        
        DestroyObjects(&m_Data[firstIndex], numElems);
        MoveObjects(&m_Data[lastIndex], &m_Data[firstIndex], toMove);
		
        m_Size = newSize;
		return &m_Data[firstIndex + 1];
	}
		
	void Swap(ThVector& rhs)
	{
		Thor::Swap(m_Capacity, rhs.m_Capacity);
		Thor::Swap(m_Data, rhs.m_Data);
		Thor::Swap(m_Size, rhs.m_Size);
        Thor::Swap(m_Allocator, rhs.m_Allocator);
	}

	void MoveToBackAndRemove(SizeType index)
	{
		if (m_Size > 1)
		{
            DestroyObject(&m_Data[index]);
            MoveObject(&m_Data[m_Size - 1], &m_Data[index]);
			--m_Size;
		}
		else
		{
            Clear();
		}
	}

	void MoveToBackAndRemove(Iterator position)
	{
		MoveToBackAndRemove(position - m_Data);
	}
		
	void Clear()
	{
        DestroyObjects(m_Data, m_Size);
        m_Size = 0;
	}
    
    void Free()
    {
        if (m_Size)
            FreeObjects(m_Allocator, m_Data, m_Size);
        
        m_Size = 0;
        m_Capacity = 0;
        m_Data = nullptr;
    }
    
    void SetData(Pointer data, SizeType size, ThiMemoryAllocator* allocator)
    {
        Free();
        m_Data = data;
        m_Size = size;
        m_Capacity = size;
        m_Allocator = allocator;
    }
    
    ThiMemoryAllocator* GetAllocator()const
    {
        return m_Allocator;
    }

private:
	Pointer	m_Data;
	SizeType m_Size;
	SizeType m_Capacity;
    ThiMemoryAllocator* m_Allocator;
};

}//Thor
