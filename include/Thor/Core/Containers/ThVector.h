#pragma once

#include <Thor/Core/Memory/ThAllocators.h>
#include <Thor/Core/Debug/ThAssert.h>
#include <Thor/Core/ThFlags.h>

#include <type_traits>

namespace Thor {

namespace Private
{
    template <bool IsClass, class SizeType, class Pointer>
    struct DestroyRangeImpl;
    
    template <class SizeType, class Pointer>
    struct DestroyRangeImpl<true, SizeType, Pointer>
    {
        void operator()(SizeType position, SizeType numElems, Pointer target)
        {
            typedef typename std::remove_pointer<Pointer>::type Type;
            for (SizeType i = position; i < position + numElems; ++i)
            {
                Pointer temp = &target[i];
                
                temp->~Type();
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

	ThVector(const ThVector& v, ThiMemoryAllocator* allocator = nullptr)
		:
    ThVector(allocator)
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
        {
			Assign(x.Begin(), x.End());
        }
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
		if (n != m_Capacity)
		{
			SizeType bound = Thor::Min(n, m_Size);

			T* prevBuffer = Realloc(n);

			if (m_Data && prevBuffer)
			{
				//if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
				{
					ConstructRange(&prevBuffer[0], &prevBuffer[bound], m_Data);
					DestroyRange(0, bound, prevBuffer);
				}
				//else
				//	ThMemory::MemoryCopy(m_Data, prevBuffer, bound * sizeof(T));

				//if (m_Flags.CheckFlag(eThVectorFlags::eCallDestructorOnMove))
				//	DestroyRange(0, m_Size, prevBuffer);
			}

			SafeDelete(prevBuffer);
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
		SizeType newSize = m_Size + n;
		SizeType index = position - m_Data;

		T* prevBuffer = InsertShared(position, n);
		ConstructRange(index, n, value);

		if (prevBuffer != m_Data)
		{
			SafeDelete(prevBuffer);
		}

		m_Size = newSize;
		return m_Data + index;
	}

	template <class InputIterator>
	Iterator Insert(ConstIterator position, InputIterator first, InputIterator last)
	{
		SizeType newSize = m_Size + last - first;
		SizeType index = position - m_Data;

		T* prevBuffer = InsertShared(position, last - first);
		ConstructRange(first, last, m_Data + index);

		if (prevBuffer && prevBuffer != m_Data)
		{
            SafeDelete(prevBuffer);
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
		SizeType numElems = last - first;
		SizeType firstIndex = first - m_Data;
		SizeType lastIndex = last - m_Data;
		
		if (numElems == m_Size)
			Clear();
		else
		{
			SizeType newSize = m_Size - numElems;		

			T* prevBuffer = Realloc(newSize);

			//if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
			{
				ConstructRange(&prevBuffer[0], &prevBuffer[firstIndex], m_Data);
				ConstructRange(&prevBuffer[lastIndex], &prevBuffer[m_Size], &m_Data[firstIndex]);
			}
			//else
			//{
			//	ThMemory::MemoryCopy(m_Data, prevBuffer, firstIndex * sizeof (T));
			//	ThMemory::MemoryCopy(&m_Data[firstIndex], &prevBuffer[lastIndex], (m_Size - lastIndex) * sizeof (T));
			//}

			DestroyRange(0, m_Size, prevBuffer);
            SafeDelete(prevBuffer);

			m_Size = newSize;
		}

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
			DestroyRange(index, 1, m_Data);

			//if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
				ConstructRange(&m_Data[m_Size - 1], &m_Data[m_Size], &m_Data[index]);
			//else
			//	ThMemory::MemoryCopy(&m_Data[index], &m_Data[m_Size - 1], sizeof (T));

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
    
    void SetData(Pointer data, SizeType size, ThiMemoryAllocator* allocator = nullptr)
    {
        FreeMemory();
        m_Data = data;
        m_Size = size;
        m_Capacity = size;
        m_Allocator = allocator;
    }

private:
	Pointer	m_Data;
	SizeType m_Size;
	SizeType m_Capacity;
    ThiMemoryAllocator* m_Allocator;
    

	void DestroyRange(SizeType position, SizeType numElems, Pointer target)
	{
        Private::DestroyRangeImpl< std::is_class<T>::value, SizeType, Pointer > impl;
        impl(position, numElems, target);
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
		T* prevBuffer = m_Data;

		if (n != m_Capacity)
		{
			ThI8* newBuffer = 0;

			if (n)
			{
				newBuffer = (ThI8*)m_Allocator->Allocate(n * sizeof (T));
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
				
				//if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
				{
					ConstructRange(prevBuffer, prevBuffer + gap, m_Data);
					DestroyRange(0, gap, prevBuffer);
				}
				//else
				//	ThMemory::MemoryCopy(m_Data, prevBuffer, gap * sizeof (T));

				if(index + numElems < m_Size)
				{
					//if (m_Flags.CheckFlag(eThVectorFlags::eCallCopyConstructorOnMove))
					{
						ConstructRange(&prevBuffer[index], &prevBuffer[index + numElems - m_Size], &m_Data[index + numElems]);
						DestroyRange(index, index + numElems - m_Size, prevBuffer);
					}
					//else
					//	ThMemory::MemoryCopy(&m_Data[index + numElems], &prevBuffer[index], (index + numElems - m_Size) * sizeof (T));
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
		if (m_Data)
		{
			DestroyRange(0, m_Size, m_Data);
			SafeDelete(m_Data);
		}

		m_Data = 0;
		m_Capacity = 0;
		m_Size = 0;
	}

	void SafeDelete(T* buffer)
	{
        if (m_Allocator)
        {
            ThI8* rawBuffer = (ThI8*)buffer;
            m_Allocator->Deallocate(rawBuffer);
        }		
	}
};

template <class T>
bool operator==(const ThVector<T>& x, const ThVector<T>& y)
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
bool operator!=(const ThVector<T>& x, const ThVector<T>& y)
{
	return !(x == y);
}

}//Thor