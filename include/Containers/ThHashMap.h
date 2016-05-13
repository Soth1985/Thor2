#pragma once

#include <Thor/Framework/Containers/ThHash.h>

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
	m_Next(0)
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

	ThBool operator==(const ThHashMapItem& other)const
	{
		return (m_Key == other.Key()) && (m_Value == other.Value());
	}

	ThBool operator!=(const ThHashMapItem& other)const
	{
		return !operator==(other);
	}

private:

	KeyT m_Key;
	ValueT m_Value;
	ThHashMapItem* m_Next;

	friend class ThHashMap<KeyT, ValueT>;
	friend class ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> >;
};

template <class KeyT, class ValueT>
class ThHashMap : public ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> >
{
public:

	typedef ValueT MappedType;

	explicit ThHashMap()
		:
	ThHashContainerBase()
	{

	}

	explicit ThHashMap(SizeType numElems, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	ThHashContainerBase(numElems, numBuckets, growSize, flags)
	{
	}

	explicit ThHashMap(SizeType numBuckets, SizeType growSize = 1)
		:
	ThHashContainerBase(numBuckets, growSize)
	{
		
	}

	explicit ThHashMap(Pointer data, SizeType dataSize, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	ThHashContainerBase(data, dataSize, numBuckets, growSize, flags)
	{

	}

	ThHashMap(const ThHashMap& m)
		:
	ThHashContainerBase(m)
	{
		
	}

	ThBool Insert(const KeyT& key, const ValueT& value)
	{
		SizeType hash = GetHash(key);
		SizeType bucket = Bucket(hash);

		if (FindImpl(key, bucket) != End())
			return false;

		ThBool itemsFull = m_Items.Size() == m_Items.Capacity();

		if (itemsFull)
			m_Items.Reserve(m_Items.Size() + GrowSize());

		m_Items.PushBack(ValueType(key, value));

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

	void Swap(ThHashMap& other)
	{
		ThHashContainerBase::Swap(other);
	}

	MappedType& operator[](const KeyT& key)
	{
		Iterator it = Find(key);

		if (it != End())
			return it->Value();

		Insert(key, MappedType());
		return m_Items.Back().Value();
	}

	const MappedType& operator[](const KeyT& key)const
	{
		ConstIterator it = Find(key);

		if (it != End())
			return it->Value();

		Insert(key, MappedType());
		return m_Items.Back().Value();
	}
};

template <class KeyT, class ValueT>
ThBool operator==(const ThHashMap<KeyT, ValueT>& x, const ThHashMap<KeyT, ValueT>& y)
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

template <class KeyT, class ValueT>
ThBool operator!=(const ThHashMap<KeyT, ValueT>& x, const ThHashMap<KeyT, ValueT>& y)
{
	return !(x == y);
}

}