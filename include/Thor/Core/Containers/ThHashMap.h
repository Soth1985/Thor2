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
	ThHashMapItem* m_Next;

	friend class ThHashMap<KeyT, ValueT>;
	friend class ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> >;
};

template <class KeyT, class ValueT>
class ThHashMap : public ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> >
{
public:

    typedef ThHashContainerBase< KeyT, ThHashMapItem<KeyT, ValueT> > BaseType;
    
    typedef typename BaseType::Pointer			Pointer;
    typedef typename BaseType::SizeType		SizeType;
    typedef typename BaseType::ValueType		ValueType;
    typedef typename BaseType::Iterator		Iterator;
    typedef typename BaseType::ConstIterator	ConstIterator;
    
    /*typedef typename ItemsList::Reference		Reference;
    typedef typename ItemsList::ConstReference	ConstReference;
    
    typedef typename ItemsList::ConstIterator	ConstIterator;
    
    typedef typename ItemsList::DifferenceType	DifferenceType;*/
    typedef ValueT MappedType;

	explicit ThHashMap()
		:
	BaseType()
	{

	}

	explicit ThHashMap(SizeType numElems, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	BaseType(numElems, numBuckets, growSize, flags)
	{
	}

	explicit ThHashMap(SizeType numBuckets, SizeType growSize = 1)
		:
	BaseType(numBuckets, growSize)
	{
		
	}

	explicit ThHashMap(Pointer data, SizeType dataSize, SizeType numBuckets, SizeType growSize = 1, const ThFlags32& flags = 0)
		:
	BaseType(data, dataSize, numBuckets, growSize, flags)
	{

	}

	ThHashMap(const ThHashMap& m)
		:
	BaseType(m)
	{
		
	}

	bool Insert(const KeyT& key, const ValueT& value)
	{
        SizeType hash = BaseType::GetHash(key);
        SizeType bucket = BaseType::Bucket(hash);

        if (BaseType::FindImpl(key, bucket) != BaseType::End())
			return false;

        bool itemsFull = BaseType::m_Items.Size() == BaseType::m_Items.Capacity();

		if (itemsFull)
            BaseType::m_Items.Reserve(BaseType::m_Items.Size() + BaseType::GrowSize());

        BaseType::m_Items.PushBack(ValueType(key, value));

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

	void Swap(ThHashMap& other)
	{
		BaseType::Swap(other);
	}

	MappedType& operator[](const KeyT& key)
	{
		Iterator it = Find(key);

		if (it != BaseType::End())
			return it->Value();

		Insert(key, MappedType());
		return BaseType::m_Items.Back().Value();
	}

	const MappedType& operator[](const KeyT& key)const
	{
		ConstIterator it = Find(key);

		if (it != BaseType::End())
			return it->Value();

		Insert(key, MappedType());
		return BaseType::m_Items.Back().Value();
	}
};

template <class KeyT, class ValueT>
bool operator==(const ThHashMap<KeyT, ValueT>& x, const ThHashMap<KeyT, ValueT>& y)
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
bool operator!=(const ThHashMap<KeyT, ValueT>& x, const ThHashMap<KeyT, ValueT>& y)
{
	return !(x == y);
}

}