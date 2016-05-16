#pragma once

#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

namespace Private
{
	template<class T> 
	inline size_t HashFunc(const T& val)
	{	
		static const ThSize hashSeed = (ThSize)0xdeadbeef;
		return ((size_t)val ^ hashSeed);
	}

	template <class IterT>
	inline ThSize HashRange(IterT begin, IterT end)
	{
		ThSize result = 2166136261U;

		while (begin != end)
			result = 16777619U * result ^ (size_t)*begin++;
		return result;
	}

	template<class CharT, class CharTraits,	class AllocT>
	inline ThSize HashFunc(const std::basic_string<CharT, CharTraits, AllocT>& str)
	{
		const CharT *cstr = str.c_str();

		return (HashRange(cstr, cstr + str.size()));
	}

	inline ThSize HashFunc(const char* str)
	{
		return (HashRange(str, str + strlen(str)));
	}

	inline ThSize HashFunc(const wchar_t* str)
	{
		return (HashRange(str, str + wcslen(str)));
	}

	inline ThSize HashFunc(char* str)
	{
		return (HashRange(str, str + strlen(str)));
	}

	inline ThSize HashFunc(wchar_t* str)
	{
		return (HashRange(str, str + wcslen(str)));
	}

	template <class T>
	inline ThBool AreEqual(const T& a, const T& b)
	{
		return a == b;
	}

	inline ThBool AreEqual(const char* a, const char* b)
	{
		return strcmp(a,b) == 0;
	}

	inline ThBool AreEqual(const wchar_t* a, const wchar_t* b)
	{
		return wcscmp(a, b) == 0;
	}

	inline ThBool AreEqual(char* a, char* b)
	{
		return strcmp(a,b) == 0;
	}

	inline ThBool AreEqual(wchar_t* a, wchar_t* b)
	{
		return wcscmp(a, b) == 0;
	}
}

template <class T>
struct ThHash
{
	static inline ThSize HashCode(const T& val)
	{
		return Private::HashFunc(val);
	}
};

template <class T>
struct ThKeyCompare
{
	static inline ThBool AreEqual(const T& a, const T& b)
	{
		return Private::AreEqual(a, b);
	}
};

template <class KeyT, class ItemT>
class ThHashContainerBase
{
public:

	typedef KeyT KeyType;
	typedef ThVector< ItemT > ItemsList;
	typedef ThVector< ItemT* > BucketsList;

	typedef typename ItemsList::Pointer			Pointer;
	typedef typename ItemsList::ConstPointer	ConstPointer;
	typedef typename ItemsList::ValueType		ValueType;
	typedef typename ItemsList::Reference		Reference;
	typedef typename ItemsList::ConstReference	ConstReference;
	typedef typename ItemsList::Iterator		Iterator; 
	typedef typename ItemsList::ConstIterator	ConstIterator;
	typedef typename ItemsList::SizeType		SizeType;
	typedef typename ItemsList::DifferenceType	DifferenceType;

	typedef ThHash<KeyT> Hasher;
	typedef ThKeyCompare<KeyT> KeyCompare;

	explicit ThHashContainerBase()
		:
	m_Buckets(16, Pointer(0)),
	m_MaxLoadFactor(0.75f),
	m_GrowSize(1)
	{

	}

	explicit ThHashContainerBase(SizeType numElems, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	m_Buckets(ComputeNumBuckets(numBuckets), Pointer(0)),
	m_MaxLoadFactor(0.75f),
	m_GrowSize(growSize)
	{
		m_Items.Reserve(numElems);
	}

	explicit ThHashContainerBase(SizeType numBuckets, SizeType growSize = 1)
		:
	m_Buckets(ComputeNumBuckets(numBuckets), Pointer(0)),
	m_MaxLoadFactor(0.75f),
	m_GrowSize(growSize)
	{
		
	}

	explicit ThHashContainerBase(Pointer data, SizeType dataSize, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	m_Items(data, dataSize, flags),
	m_Buckets(ComputeNumBuckets(numBuckets), Pointer(0)),
	m_MaxLoadFactor(0.75f),
	m_GrowSize(growSize)
	{

	}

	ThHashContainerBase(const ThHashContainerBase& m)
	{
		m_Items = m.m_Items;
		m_MaxLoadFactor = m.m_MaxLoadFactor;
		m_GrowSize = m.m_GrowSize;
		Rehash(m.BucketCount());
	}

	SizeType GetHash(const KeyT& key)const
	{
		return Hasher::HashCode(key);
	}

	ThBool AreKeysEqual(const KeyT& key1, const KeyT& key2)const
	{
		return KeyCompare::AreEqual(key1, key2);
	}

	SizeType BucketCount()const
	{
		return m_Buckets.Size();
	}

	SizeType Bucket(ThSize hash)const
	{
		hash ^= (hash >> 20) ^ (hash >> 12);
		hash = hash ^ (hash >> 7) ^ (hash >> 4);

		return hash & (m_Buckets.Size() - 1);
	}

	void Rehash(SizeType numBuckets)
	{
		numBuckets = NextPowerOf2(numBuckets);

		m_Buckets.Assign(numBuckets, Pointer(0));

		for(SizeType i = 0; i < m_Items.Size(); ++i)
		{
			m_Items[i].m_Next = 0;
		}

		for(SizeType i = 0; i < m_Items.Size(); ++i)
		{
			SizeType hash = GetHash(m_Items[i].Key());
			SizeType bucket = Bucket(hash);

			Iterator tail = FindTail(bucket);

			if ( tail == 0)
				m_Buckets[bucket] = &m_Items[i];
			else
				tail->m_Next = &m_Items[i];
		}
	}

	ThF32 LoadFactor()const
	{
		return (ThF32)m_Items.Size() / (ThF32)m_Buckets.Size();
	}

	ThF32 MaxLoadFactor()const
	{
		return m_MaxLoadFactor;
	}

	void MaxLoadFactor(ThF32 loadFactor)
	{
		m_MaxLoadFactor = loadFactor;
	}

	void Reserve(SizeType numItems)
	{
		m_Items.Reserve(numItems);
		Rehash(BucketCount());
	}

	Iterator Find(const KeyT& key)
	{
		SizeType hash = GetHash(key);
		SizeType bucket = Bucket(hash);
		return FindImpl(key, bucket);
	}

	ConstIterator Find(const KeyT& key)const
	{
		SizeType hash = GetHash(key);
		SizeType bucket = Bucket(hash);
		return FindImpl(key, bucket);
	}

	Iterator Begin()
	{
		return m_Items.Begin();
	}

	Iterator End()
	{
		return m_Items.End();
	}

	ConstIterator Begin()const
	{
		return m_Items.Begin();
	}

	ConstIterator End()const
	{
		return m_Items.End();
	}

	ConstIterator CBegin()const
	{
		return m_Items.Begin();
	}

	ConstIterator CEnd()const
	{
		return m_Items.End();
	}

	SizeType Size()const
	{
		return m_Items.Size();
	}

	Reference GetItem(SizeType index)
	{
		return m_Items[index];
	}

	ConstReference GetItem(SizeType index)const
	{
		return m_Items[index];
	}

	ThBool Erase(const KeyT& key)
	{
		SizeType hash = GetHash(key);
		SizeType bucket = Bucket(hash);

		Iterator item = FindImpl(key, bucket);

		if (item != End())
		{
			Iterator next = m_Buckets[bucket]->m_Next;

			if (next == 0 && item == m_Buckets[bucket])
			{
				m_Buckets[bucket] = 0;
				//m_Items.MoveToBackAndRemove(item);
				m_Items.Erase(item);
				Rehash(BucketCount());
				return true;
			}

			Iterator left = m_Buckets[bucket];
			next = left;
			Iterator right = 0;

			while (next != 0)
			{
				if (next == item)
				{
					right = next->m_Next;
					break;
				}
				
				left = next;
				next = next->m_Next;
			}

			if (left == m_Buckets[bucket])
				m_Buckets[bucket] = right;
			else
				left->m_Next = right;

			//m_Items.MoveToBackAndRemove(item);
			m_Items.Erase(item);
			Rehash(BucketCount());

			return true;
		}

		return false;
	}

	ThBool Erase(Iterator it)
	{
		return Erase(it->Key());
	}

	ThBool Erase(SizeType index)
	{
		return Erase(m_Items[index].Key());
	}

	void Clear()
	{
		m_Items.Clear();
		m_Buckets.Assign(BucketCount(), Pointer(0));
	}

	void ShrinkToFit()
	{
		m_Items.ShrinkToFit();
		Rehash(BucketCount());
	}

	SizeType GrowSize()const
	{
		return m_GrowSize;
	}

	void GrowSize(SizeType size)
	{
		m_GrowSize = size;
	}

	void Swap(ThHashContainerBase& other)
	{
		m_Items.Swap(other.m_Items);
		m_Buckets.Swap(other.m_Buckets);
		Thor::Swap(m_MaxLoadFactor, other.m_MaxLoadFactor);
		Thor::Swap(m_GrowSize, other.m_GrowSize);
	}

protected:

	SizeType ComputeNumBuckets(SizeType numBuckets)const
	{
		numBuckets = NextPowerOf2(numBuckets);

		if (numBuckets < 16)
			numBuckets = 16;

		return numBuckets;
	}

	Iterator FindTail(SizeType bucket)
	{
		Iterator result = m_Buckets[bucket];

		if (result == 0)
			return 0;

		Iterator next = result->m_Next;

		while (next != 0)
		{
			result = next;
			next = next->m_Next;
		}

		return result;
	}

	Iterator FindImpl(const KeyT& key, SizeType bucket)
	{
		if (m_Buckets[bucket] == 0)
			return End();

		Iterator curItem = m_Buckets[bucket];

		while(curItem != 0)
		{
			if (AreKeysEqual(key, curItem->Key()))
				return curItem;

			curItem = curItem->m_Next;
		}
		
		return End();
	}

	ConstIterator FindImpl(const KeyT& key, SizeType bucket)const
	{
		if (m_Buckets[bucket] == 0)
			return End();

		Iterator curItem = m_Buckets[bucket];

		while(curItem != 0)
		{
			if (AreKeysEqual(key, curItem->Key()))
				return curItem;

			curItem = curItem->m_Next;
		}

		return End();
	}

	ItemsList m_Items;
	BucketsList m_Buckets;
	float m_MaxLoadFactor;
	ThSize m_GrowSize;
};

}