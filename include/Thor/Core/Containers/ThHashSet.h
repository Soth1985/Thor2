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
	m_Next(0)
	{

	}

	const KeyT& Key()const
	{
		return m_Key;
	}

	ThBool operator==(const ThHashSetItem& other)const
	{
		return m_Key == other.Key();
	}

	ThBool operator!=(const ThHashSetItem& other)const
	{
		return !operator==(other);
	}

private:

	KeyT m_Key;
	ThHashSetItem* m_Next;

	friend class ThHashSet<KeyT>;
	friend class ThHashContainerBase< KeyT, ThHashSetItem<KeyT> >;
};

template <class KeyT>
class ThHashSet : public ThHashContainerBase< KeyT, ThHashSetItem<KeyT> >
{
public:
    
    typedef ThHashContainerBase< KeyT, ThHashSetItem<KeyT> > BaseType;
    
    typedef typename BaseType::Pointer			Pointer;
    typedef typename BaseType::SizeType		SizeType;
    typedef typename BaseType::ValueType		ValueType;
    typedef typename BaseType::Iterator		Iterator;
    typedef typename BaseType::ConstIterator	ConstIterator;

	explicit ThHashSet()
		:
	BaseType()
	{

	}

	explicit ThHashSet(SizeType numElems, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	BaseType(numElems, numBuckets, growSize, flags)
	{
	}

	explicit ThHashSet(SizeType numBuckets, SizeType growSize = 1)
		:
	BaseType(numBuckets, growSize)
	{
		
	}

	explicit ThHashSet(Pointer data, SizeType dataSize, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	BaseType(data, dataSize, numBuckets, growSize, flags)
	{

	}

	ThHashSet(const ThHashSet& m)
		:
	BaseType(m)
	{
		
	}

	ThBool Insert(const KeyT& key)
	{
        SizeType hash = BaseType::GetHash(key);
		SizeType bucket = BaseType::Bucket(hash);

		if (BaseType::FindImpl(key, bucket) != BaseType::End())
			return false;

		ThBool itemsFull = BaseType::m_Items.Size() == BaseType::m_Items.Capacity();

		if (itemsFull)
			BaseType::m_Items.Reserve(BaseType::m_Items.Size() + BaseType::GrowSize());

		BaseType::m_Items.PushBack(ValueType(key));

		if (BaseType::LoadFactor() > BaseType::MaxLoadFactor())
		{
			BaseType::Rehash( BaseType::BucketCount() * 2 );
			return true;
		}

		if (itemsFull)
		{
			BaseType::Rehash(BaseType::BucketCount());
			return true;
		}

		Iterator tail = BaseType::FindTail(bucket);

		if ( tail == 0)
			BaseType::m_Buckets[bucket] = &BaseType::m_Items.Back();
		else
			tail->m_Next = &BaseType::m_Items.Back();

		return true;
	}

	void Swap(ThHashSet& other)
	{
		BaseType::Swap(other);
	}
};

template <class KeyT>
ThBool operator==(const ThHashSet<KeyT>& x, const ThHashSet<KeyT>& y)
{
	if (x.Size() != y.Size())
	{
		return false;
	}

	for (ThSize i = 0; i < x.Size(); ++i)
	{
		if (x.GetItem(i) != y.GetItem(i))
			return false;
	}

	return true;
}

template <class KeyT>
ThBool operator!=(const ThHashSet<KeyT>& x, const ThHashSet<KeyT>& y)
{
	return !(x == y);
}

}