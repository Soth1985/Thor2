#pragma once

#include <Thor/Core/Containers/ThHash.h>

namespace Thor
{

template <class KeyT>
class ThHashSet;

template <class KeyT>
class ThHashSetItem
{
public:

	ThHashSetItem(const KeyT& key)
		:
	m_Key(key),
	m_Next(-1)
	{

	}

	const KeyT& Key()const
	{
		return m_Key;
	}

	bool operator==(const ThHashSetItem& other)const
	{
		return m_Key == other.Key();
	}

	bool operator!=(const ThHashSetItem& other)const
	{
		return !operator==(other);
	}

private:

	KeyT m_Key;
	ThU64 m_Next;

	friend class ThHashSet<KeyT>;
	friend class ThHashContainerBase< KeyT, ThHashSetItem<KeyT> >;
};

template <class KeyT>
class ThHashSet : public ThHashContainerBase< KeyT, ThHashSetItem<KeyT> >
{
public:
    
    typedef ThHashContainerBase< KeyT, ThHashSetItem<KeyT> > BaseType;
    
    typedef typename BaseType::Pointer Pointer;
    typedef typename BaseType::SizeType	SizeType;
    typedef typename BaseType::ValueType ValueType;
    typedef typename BaseType::Iterator	Iterator;
    typedef typename BaseType::ConstIterator ConstIterator;

	explicit ThHashSet(ThiMemoryAllocator* allocator = nullptr)
		:
	BaseType(allocator)
	{

	}

	explicit ThHashSet(SizeType numElems, SizeType numBuckets = 16, ThiMemoryAllocator* allocator = nullptr)
		:
	BaseType(numElems, numBuckets, allocator)
	{
	}

	explicit ThHashSet(SizeType numBuckets, ThiMemoryAllocator* allocator = nullptr)
		:
	BaseType(numBuckets, allocator)
	{
		
	}
    
    ThHashSet(std::initializer_list<KeyT> items, SizeType numBuckets = 16, ThiMemoryAllocator* allocator = nullptr)
        :
    BaseType(items.size(), numBuckets, allocator)
    {
        for (auto it : items)
        {
            Insert(it);
        }
    }

	bool Insert(const KeyT& key)
	{
        SizeType hash = BaseType::GetHash(key);
        SizeType bucket = BaseType::Bucket(hash);
        ThU64 prevIndex;

		if (BaseType::FindImpl(key, bucket, prevIndex) != BaseType::End())
			return false;
        
        if (BaseType::m_Items.Size() == BaseType::m_Items.Capacity())
            BaseType::m_Items.Reserve(BaseType::m_Items.Capacity() * 2 + 10);

		BaseType::m_Items.EmplaceBack(key);
        SizeType itemIndex = BaseType::m_Items.Size() - 1;

		if (BaseType::LoadFactor() > BaseType::MaxLoadFactor())
		{
			BaseType::Rehash( BaseType::BucketCount() * 2 );
			return true;
		}

		Iterator tail = BaseType::FindTail(bucket);

        if (tail == BaseType::End())
			BaseType::m_Buckets[bucket] = itemIndex;
		else
			tail->m_Next = itemIndex;

		return true;
	}
};

}
