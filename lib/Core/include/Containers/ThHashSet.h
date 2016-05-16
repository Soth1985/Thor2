#pragma once

#include <Thor/Framework/Containers/ThHash.h>

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

	explicit ThHashSet()
		:
	ThHashContainerBase()
	{

	}

	explicit ThHashSet(SizeType numElems, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	ThHashContainerBase(numElems, numBuckets, growSize, flags)
	{
	}

	explicit ThHashSet(SizeType numBuckets, SizeType growSize = 1)
		:
	ThHashContainerBase(numBuckets, growSize)
	{
		
	}

	explicit ThHashSet(Pointer data, SizeType dataSize, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	ThHashContainerBase(data, dataSize, numBuckets, growSize, flags)
	{

	}

	ThHashSet(const ThHashSet& m)
		:
	ThHashContainerBase(m)
	{
		
	}

	ThBool Insert(const KeyT& key)
	{
		SizeType hash = GetHash(key);
		SizeType bucket = Bucket(hash);

		if (FindImpl(key, bucket) != End())
			return false;

		ThBool itemsFull = m_Items.Size() == m_Items.Capacity();

		if (itemsFull)
			m_Items.Reserve(m_Items.Size() + GrowSize());

		m_Items.PushBack(ValueType(key));

		if (LoadFactor() > MaxLoadFactor())
		{
			Rehash( BucketCount() * 2 );
			return true;
		}

		if (itemsFull)
		{
			Rehash(BucketCount());
			return true;
		}

		Iterator tail = FindTail(bucket);

		if ( tail == 0)
			m_Buckets[bucket] = &m_Items.Back();
		else
			tail->m_Next = &m_Items.Back();

		return true;
	}

	void Swap(ThHashSet& other)
	{
		ThHashContainerBase::Swap(other);
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