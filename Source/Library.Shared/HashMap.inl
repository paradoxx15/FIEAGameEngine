#pragma once

#include "HashMap.h"

namespace Library
{
#pragma region Iterator
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, size_t index, const typename HashMap<TKey, TData>::ChainType::Iterator& chainIterator) : mOwner(&owner), mIndex(index), mChainIterator(chainIterator) {}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::Iterator::operator*() const
	{
		if (mOwner == nullptr || mOwner->Size() == 0)
		{
			throw std::runtime_error("This iterator does not belong to a HashMap");
		}

		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::Iterator::operator->() const
	{
		return &operator*();
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& it) const
	{
		return !(*this != it);
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& it) const
	{
		return (mOwner != it.mOwner || mIndex != it.mIndex || mChainIterator != it.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if (mOwner == nullptr || mIndex >= mOwner->mBuckets.Size())
		{
			throw std::runtime_error("You cannot increment the end iterator");
		}

		if (++mChainIterator == mOwner->mBuckets[mIndex].end())
		{
			do 
			{
				++mIndex;
				if (mIndex >= mOwner->mBuckets.Size())
				{
					mChainIterator = ChainType::Iterator();
					return *this;
				}
			} while (mOwner->mBuckets[mIndex].IsEmpty());

			mChainIterator = mOwner->mBuckets[mIndex].begin();
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}
#pragma endregion

#pragma region ConstIterator
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& owner, size_t index, const typename HashMap<TKey, TData>::ChainType::ConstIterator& chainIterator) : mOwner(&owner), mIndex(index), mChainIterator(chainIterator) {}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& it) :
		mOwner(it.mOwner), mIndex(it.mIndex), mChainIterator(it.mChainIterator) {}

	template<typename TKey, typename TData>
	inline typename const HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr || mOwner->Size() == 0)
		{
			throw std::runtime_error("This ConstIterator does not belong to a HashMap");
		}

		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline typename const HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::ConstIterator::operator->() const
	{
		return &operator*();
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& it) const
	{
		return !(*this != it);
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& it) const
	{
		return (mOwner != it.mOwner || mIndex != it.mIndex || mChainIterator != it.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++()
	{
		if (mOwner == nullptr || mIndex >= mOwner->mBuckets.Size())
		{
			throw std::runtime_error("You cannot increment the end iterator");
		}

		if (++mChainIterator == mOwner->mBuckets[mIndex].end())
		{
			do
			{
				++mIndex;
				if (mIndex >= mOwner->mBuckets.Size())
				{
					mChainIterator = ChainType::Iterator();
					return *this;
				}
			} while (mOwner->mBuckets[mIndex].IsEmpty());

			mChainIterator = mOwner->mBuckets[mIndex].begin();
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		operator++();
		return it;
	}
#pragma endregion

#pragma region MemberMethods
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(size_t bucketSize, HashFunctor hashFunctor, KeyEqualityFunctor keyEquality) :
		mSize(0), mHashFunctor(hashFunctor), mKeyEquality(keyEquality)
	{
		assert(bucketSize != 0);
		mBuckets.Resize(bucketSize);
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(std::initializer_list<PairType> list, size_t bucketSize, HashFunctor hashFunctor, KeyEqualityFunctor keyEquality)
	{
		if (bucketSize == 0)
		{
			mBuckets.Resize(list.size());
		}
		else
		{
			mBuckets.Resize(bucketSize);
		}

		for (const auto& item : list)
		{
			Insert(item);
		}
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(const HashMap& rhs) :
		mSize(rhs.mSize), mBuckets(rhs.mBuckets), mHashFunctor(rhs.mHashFunctor), mKeyEquality(rhs.mKeyEquality)
	{
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(HashMap&& rhs) noexcept :
		mSize(rhs.mSize), mBuckets(std::move(rhs.mBuckets)), mHashFunctor(rhs.mHashFunctor), mKeyEquality(rhs.mKeyEquality)
	{
		rhs.mSize = 0;
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(const HashMap& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mSize = rhs.mSize;
			mBuckets = rhs.mBuckets;
			mHashFunctor = rhs.mHashFunctor;
			mKeyEquality = rhs.mKeyEquality;
		}

		return *this;
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(HashMap&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			mSize = rhs.mSize;
			mBuckets = std::move(rhs.mBuckets);
			mHashFunctor = rhs.mHashFunctor;
			mKeyEquality = rhs.mKeyEquality;

			rhs.mSize = 0;
		}

		return *this;
	}
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(std::initializer_list<PairType> list)
	{
		Clear();
		for (const auto& item : list)
		{
			Insert(item);
		}
	}
#pragma endregion

#pragma region ElementAccess
  	template<typename TKey, typename TData>
  	inline TData& HashMap<TKey, TData>::At(const TKey& key)
  	{
		auto it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("There is no element at that key");
		}

		return it->second;
  	}
  
   	template<typename TKey, typename TData>
   	inline const TData& HashMap<TKey, TData>::At(const TKey& key) const
   	{
		auto it = Find(key);
		if (it == cend())
		{
			throw std::runtime_error("There is no element at that key");
		}

		return it->second;
   	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key, size_t& index)
	{
		index = mHashFunctor(key) % mBuckets.Size();
		for (typename ChainType::Iterator it = mBuckets[index].begin(); it != mBuckets[index].end(); ++it)
		{
			if (mKeyEquality((*it).first, key))
			{
				return Iterator(*this, index, it);
			}
		}

		return end();
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		size_t index;
		return Find(key, index);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		size_t index;
		return ConstIterator(const_cast<HashMap&>(*this).Find(key, index));
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return (Find(key) != end());
	}

	template<typename TKey, typename TData>
	inline size_t HashMap<TKey, TData>::Size() const
	{
		return mSize;
	}

	template<typename TKey, typename TData>
	inline size_t HashMap<TKey, TData>::BucketSize() const
	{
		return mBuckets.Size();
	}
#pragma endregion

#pragma region Modifiers
	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		auto [ret, inserted] = Insert(PairType(key, TData()));
		return ret->second;
	}

	template<typename TKey, typename TData>
	inline const TData& HashMap<TKey, TData>::operator[](const TKey& key) const
	{
		return At(key);
	}

	template<typename TKey, typename TData>
	inline std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const PairType& data)
	{
		size_t index;

		Iterator foundIt = Find(data.first, index);
		if (foundIt == end())
		{
			auto it = mBuckets[index].PushBack(data);
			mSize++;
			return std::pair(Iterator(*this, index, it), true);
		}

		return std::pair(foundIt, false);
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const TKey& key)
	{
		return Remove(Find(key));
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Remove(const Iterator& it)
	{
		if (it.mOwner == nullptr || it.mIndex >= mBuckets.Size())
		{
			return false;
		}

		--mSize;
		return mBuckets[it.mIndex].Remove(it.mChainIterator);
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Resize(const size_t bucketSize)
	{
		assert(bucketSize != 0);

		HashMap map(bucketSize);

		for (auto& item : *this)
		{
			map.Insert(item);
		}

		*this = std::move(map);
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear()
	{
		for (size_t i = 0; i < mBuckets.Size(); ++i)
		{
			mBuckets[i].Clear();
		}
		mSize = 0;
	}
#pragma endregion

#pragma region BeginEnd
	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		size_t index = 0;
		
		while (mBuckets[index].IsEmpty())
		{
			++index;

			if (index >= mBuckets.Size())
			{
				return end();
			}
		}
		
		return Iterator(*this, index, mBuckets[index].begin());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const
	{
		return cbegin();
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		size_t index = 0;

		while (mBuckets[index].IsEmpty())
		{
			++index;

			if (index >= mBuckets.Size())
			{
				return cend();
			}
		}

		return ConstIterator(*this, index, mBuckets[index].begin());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator(*this, mBuckets.Size(), ChainType::Iterator());
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const
	{
		return cend();
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return ConstIterator(*this, mBuckets.Size(), ChainType::ConstIterator());
	}
#pragma endregion
}