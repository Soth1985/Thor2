#pragma once

#include <Thor/Core/Containers/ThHash.h>

namespace Thor
{

template <class KeyT, class ValueT>
class ThHashMap;

template <class KeyT, class ValueT>
class ThHashMapItem
{
public:

	ThHashMapItem(const KeyT& key, const ValueT& value)
		:
	m_Key(key),
	m_Value(value),
	m_Next(-1)
	{

	}

	const KeyT& Key()const
	{
		return m_Key;
	}

	const ValueT& Value()const
	{
		return m_Value;
	}

	ValueT& Value()
	{
		return m_Value;
	}

	bool operator==(const ThHashMapItem& other)const
	{
		return (m_Key == other.Key()) && (m_Value == other.Value());
	}

	bool operator!=(const ThHashMapItem& other)const
	{
		return !operator==(other);
	}

private:

	KeyT m_Key;
	ValueT m_Value;
	ThU64 m_Next;

	friend class ThHashMap<KeyT, ValueT>;
	friend class ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> >;
};

template <class KeyT, class ValueT>
class ThHashMap : public ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> >
{
public:

    typedef ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> > BaseType;
    
    typedef typename BaseType::Pointer Pointer;
    typedef typename BaseType::SizeType	SizeType;
    typedef typename BaseType::ValueType ValueType;
    typedef typename BaseType::Iterator	Iterator;
    typedef typename BaseType::ConstIterator ConstIterator;
    
    /*typedef typename ItemsList::Reference	Reference;
    typedef typename ItemsList::ConstReference ConstReference;
    
    typedef typename ItemsList::ConstIterator ConstIterator;
    
    typedef typename ItemsList::DifferenceType DifferenceType;*/
    typedef ValueT MappedType;

	explicit ThHashMap(ThiMemoryAllocator* allocator = nullptr)
		:
	BaseType(allocator)
	{

	}

	explicit ThHashMap(SizeType numElems, SizeType numBuckets = 16, ThiMemoryAllocator* allocator = nullptr)
		:
	BaseType(numElems, numBuckets, allocator)
	{
	}
    
	explicit ThHashMap(SizeType numBuckets, ThiMemoryAllocator* allocator = nullptr)
		:
	BaseType(numBuckets, allocator)
	{
		
	}
    
    ThHashMap(std::initializer_list<ThPair<KeyT, ValueT>> items, SizeType numBuckets = 16, ThiMemoryAllocator* allocator = nullptr)
        :
    BaseType(items.size(), numBuckets, allocator)
    {
        for (auto it : items)
        {
            Insert(it.First(), it.Second());
        }
    }

	bool Insert(const KeyT& key, const ValueT& value)
	{
        ThU64 hash = BaseType::GetHash(key);
        SizeType bucket = BaseType::Bucket(hash);
        ThU64 prevIndex;

        if (BaseType::FindImpl(key, bucket, prevIndex) != BaseType::End())
			return false;
        
        if (BaseType::m_Items.Size() == BaseType::m_Items.Capacity())
            BaseType::m_Items.Reserve(BaseType::m_Items.Capacity() * 2 + 10);

        BaseType::m_Items.EmplaceBack(key, value);
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

	MappedType& operator[](const KeyT& key)
	{
		Iterator it = BaseType::Find(key);

		if (it != BaseType::End())
			return it->Value();

		Insert(key, MappedType());
		return BaseType::m_Items.Back().Value();
	}

	const MappedType& operator[](const KeyT& key)const
	{
		ConstIterator it = BaseType::Find(key);

		if (it != BaseType::End())
			return it->Value();

		Insert(key, MappedType());
		return BaseType::m_Items.Back().Value();
	}
};

}
