#pragma once

#include <Thor/Core/Hash/ThIntHash.h>
#include <Thor/Core/Hash/ThMurmurHash3.h>
#include <Thor/Core/Containers/ThVector.h>

namespace Thor
{

namespace Private
{
	template<class T> 
	inline ThU64 HashFunc(const T& val)
	{
        return Hash::HashInt64((ThU64)val);
	}

	template <class IterT>
	inline ThU64 HashRange(IterT begin, IterT end)
	{
		/*ThU64 result = ThU64(2166136261U);

		while (begin != end)
			result = 16777619U * result ^ (size_t)*begin++;
		return result;*/
        return Thor::Hash::Murmur3((ThU8*)begin, end - begin, 0);
	}

	template<class CharT, class CharTraits,	class AllocT>
	inline ThU64 HashFunc(const std::basic_string<CharT, CharTraits, AllocT>& str)
	{
		const CharT *cstr = str.c_str();

		return (HashRange(cstr, cstr + str.size()));
	}

	inline ThU64 HashFunc(const char* str)
	{
		return (HashRange(str, str + strlen(str)));
	}

	inline ThU64 HashFunc(const wchar_t* str)
	{
		return (HashRange(str, str + wcslen(str)));
	}

	inline ThU64 HashFunc(char* str)
	{
		return (HashRange(str, str + strlen(str)));
	}

	inline ThU64 HashFunc(wchar_t* str)
	{
		return (HashRange(str, str + wcslen(str)));
	}

	template <class T>
	inline bool AreEqual(const T& a, const T& b)
	{
		return a == b;
	}

	inline bool AreEqual(const char* a, const char* b)
	{
		return strcmp(a,b) == 0;
	}

	inline bool AreEqual(const wchar_t* a, const wchar_t* b)
	{
		return wcscmp(a, b) == 0;
	}

	inline bool AreEqual(char* a, char* b)
	{
		return strcmp(a,b) == 0;
	}

	inline bool AreEqual(wchar_t* a, wchar_t* b)
	{
		return wcscmp(a, b) == 0;
	}
    
    constexpr ThU64 InvalidID()
    {
        return -1;
    }
}

template <class T>
struct ThHash
{
	static inline ThU64 HashCode(const T& val)
	{
		return Private::HashFunc(val);
	}
};

template <class T>
struct ThKeyCompare
{
	static inline bool AreEqual(const T& a, const T& b)
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
	typedef ThVector< ThU64 > BucketsList;

	typedef typename ItemsList::Pointer	Pointer;
	typedef typename ItemsList::ConstPointer ConstPointer;
	typedef typename ItemsList::ValueType ValueType;
	typedef typename ItemsList::Reference Reference;
	typedef typename ItemsList::ConstReference ConstReference;
	typedef typename ItemsList::Iterator Iterator;
	typedef typename ItemsList::ConstIterator ConstIterator;
	typedef typename ItemsList::SizeType SizeType;
	typedef typename ItemsList::DifferenceType DifferenceType;

	typedef ThHash<KeyT> Hasher;
	typedef ThKeyCompare<KeyT> KeyCompare;

	explicit ThHashContainerBase(ThiMemoryAllocator* allocator = nullptr)
		:
    m_Items(allocator),
    m_Buckets(SizeType(16), Private::InvalidID(), allocator),
	m_MaxLoadFactor(0.75f)
	{

	}

	explicit ThHashContainerBase(SizeType numElems, SizeType numBuckets = 16, ThiMemoryAllocator* allocator = nullptr)
		:
    m_Items(allocator),
	m_Buckets(ComputeNumBuckets(numBuckets), Private::InvalidID(), allocator),
	m_MaxLoadFactor(0.75f)
	{
		m_Items.Reserve(numElems);
	}

	explicit ThHashContainerBase(SizeType numBuckets, ThiMemoryAllocator* allocator = nullptr)
		:
    m_Items(allocator),
	m_Buckets(ComputeNumBuckets(numBuckets), Private::InvalidID(), allocator),
	m_MaxLoadFactor(0.75f)
	{
		
	}

	ThU64 GetHash(const KeyT& key)const
	{
		return Hasher::HashCode(key);
	}

	bool AreKeysEqual(const KeyT& key1, const KeyT& key2)const
	{
		return KeyCompare::AreEqual(key1, key2);
	}

	SizeType BucketCount()const
	{
		return m_Buckets.Size();
	}

	SizeType Bucket(ThU64 hash)const
	{
		/*hash ^= (hash >> 20) ^ (hash >> 12);
		hash = hash ^ (hash >> 7) ^ (hash >> 4);*/

		return hash & (m_Buckets.Size() - 1);
	}

	void Rehash(SizeType numBuckets)
	{
		numBuckets = NextPowerOf2(ThU64(numBuckets));

		m_Buckets.Assign(numBuckets, Private::InvalidID());

		for(SizeType i = 0; i < m_Items.Size(); ++i)
		{
			m_Items[i].m_Next = Private::InvalidID();
		}

		for(SizeType i = 0; i < m_Items.Size(); ++i)
		{
			ThU64 hash = GetHash(m_Items[i].Key());
			SizeType bucket = Bucket(hash);

			Iterator tail = FindTail(bucket);

			if (tail == End())
				m_Buckets[bucket] = i;
			else
				tail->m_Next = i;
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
        SizeType numBucketsEstimate = numItems / MaxLoadFactor();
        SizeType numBuckets = ComputeNumBuckets(numBucketsEstimate);
		Rehash(numBuckets);
	}

	Iterator Find(const KeyT& key)
	{
		ThU64 hash = GetHash(key);
		SizeType bucket = Bucket(hash);
        ThU64 prevItem;
		return FindImpl(key, bucket, prevItem);
	}

	ConstIterator Find(const KeyT& key)const
	{
		ThU64 hash = GetHash(key);
		SizeType bucket = Bucket(hash);
        ThU64 prevItem;
		return FindImpl(key, bucket, prevItem);
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

	bool Erase(const KeyT& key)
	{
		ThU64 hash = GetHash(key);
		SizeType bucket = Bucket(hash);

        ThU64 prevItem;
		Iterator item = FindImpl(key, bucket, prevItem);

		if (item != End())
		{
            ThU64 itemIndex = item - m_Items.Data();
            ThU64 itemIndexLast = m_Items.Size() - 1;
            
            if (prevItem == Private::InvalidID())
                m_Buckets[bucket] = m_Items[itemIndex].m_Next;
            else
                m_Items[prevItem].m_Next = m_Items[itemIndex].m_Next;
            
            if (itemIndex != itemIndexLast)
            {
                ThU64 hashLast = GetHash(m_Items[itemIndexLast].Key());
                SizeType bucketLast = Bucket(hashLast);
                ThU64 prevItemLast;
                FindImpl(m_Items[itemIndexLast].Key(), bucketLast, prevItemLast);
                
                if (prevItemLast == Private::InvalidID())
                    m_Buckets[bucketLast] = itemIndex;
                else
                    m_Items[prevItemLast].m_Next = itemIndex;
            }
            
            m_Items.MoveToBackAndRemove(itemIndex);

			return true;
		}

		return false;
	}

	bool Erase(Iterator it)
	{
		return Erase(it->Key());
	}

	bool Erase(SizeType index)
	{
		return Erase(m_Items[index].Key());
	}

	void Clear()
	{
		m_Items.Clear();
		m_Buckets.Assign(BucketCount(), Private::InvalidID());
	}

	void ShrinkToFit()
	{
		m_Items.ShrinkToFit();
        SizeType numBucketsEstimate = m_Items.Capacity() / MaxLoadFactor();
        SizeType numBuckets = ComputeNumBuckets(numBucketsEstimate);
		Rehash(numBuckets);
        m_Buckets.ShrinkToFit();
	}

	void Swap(ThHashContainerBase& other)
	{
		m_Items.Swap(other.m_Items);
		m_Buckets.Swap(other.m_Buckets);
        Thor::Swap(m_MaxLoadFactor, other.m_MaxLoadFactor);
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
        ThU64 itemIndex = m_Buckets[bucket];
        
        if (itemIndex == Private::InvalidID())
            return End();
        
		Iterator result = &m_Items[itemIndex];
		itemIndex = result->m_Next;

		while (itemIndex != Private::InvalidID())
		{
			result = &m_Items[itemIndex];
			itemIndex = result->m_Next;
		}

		return result;
	}

	Iterator FindImpl(const KeyT& key, SizeType bucket, ThU64& prevItem)
	{
        prevItem = Private::InvalidID();
        ThU64 itemIndex = m_Buckets[bucket];

		while(itemIndex != Private::InvalidID())
		{
            Iterator curItem = &m_Items[itemIndex];
			if (AreKeysEqual(key, curItem->Key()))
				return curItem;

            prevItem = itemIndex;
			itemIndex = curItem->m_Next;
		}
		
		return End();
	}

	ConstIterator FindImpl(const KeyT& key, SizeType bucket, ThU64& prevItem)const
	{
        ThHashContainerBase* pThis = const_cast<ThHashContainerBase*>(this);
        return pThis->FindImpl(key, bucket, prevItem);
	}

	ItemsList m_Items;
	BucketsList m_Buckets;
	float m_MaxLoadFactor;
};

}
