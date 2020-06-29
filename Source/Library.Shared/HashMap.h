#pragma once

#include <functional>
#include <algorithm>
#include <utility>
#include <initializer_list>
#include "Vector.h"
#include "SList.h"
#include "DefaultHash.h"
#include "DefaultEquality.h"

namespace Library
{
	template<typename TKey, typename TData>
	class HashMap final
	{
	public:
		using HashFunctor = std::function<size_t(const TKey&)>;
		using KeyEqualityFunctor = std::function<bool(const TKey & lhs, const TKey & rhs)>;
		using PairType = std::pair<const TKey, TData>;
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;

#pragma region Iterator
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		private:
			/// <summary>
			/// A pointer to the list that owns this iterator
			/// </summary>
			HashMap* mOwner = nullptr;
			/// <summary>
			/// The current node that this iterator points to
			/// </summary>
			size_t mIndex = 0;
			/// <summary>
			/// The iterator used to step through the ChainList at mIndex
			/// </summary>
			typename ChainType::Iterator mChainIterator;

			/// Default constructor with the data index and list owner
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="index">index for mIndex to be set to</param>
			/// <param name="chainIterator">The iterator of the current chain list that the data is contained</param>
			Iterator(HashMap& owner, size_t index, const typename ChainType::Iterator& chainIterator);

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = PairType;
			using pointer = PairType*;
			using reference = PairType;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor with no arguments using compiler default
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="CopyIterator">Iterator to copy</param>
			Iterator(const Iterator&) = default;
			/// <summary>
			/// Default move operator
			/// </summary>
			/// <param name="MoveIterator">The iterator to be moved</param>
			Iterator(Iterator&&) noexcept = default;
			/// <summary>
			/// Copy assignment operator
			/// </summary>
			/// <param name="CopyIterator">The iterator on the rhs of the = operator that will be copied</param>
			/// <returns>A copied version of the passed in iterator</returns>
			Iterator& operator=(const Iterator&) = default;
			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="MoveIterator">Iterator that the information will be moved from</param>
			/// <returns>A reference to the new list</returns>
			Iterator& operator=(Iterator&&) noexcept = default;
			/// <summary>
			/// Default destructor
			/// </summary>
			~Iterator() = default;


			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>A reference to the data contained within mChainIterator</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is >= size </exception>
			PairType& operator*() const;
			/// <summary>
			/// Dereference arrow operator
			/// </summary>
			/// <returns>Address of a reference to the data contained within mChainIterator</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is >= size </exception>
			PairType* operator->() const;
			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="it">The iterator that this iterator is being compared to</param>
			/// <returns>True if the iterators are the same, false otherwise</returns>
			bool operator==(const Iterator& it) const;
			/// <summary>
			/// Compares two iterators for inequality
			/// </summary>
			/// <param name="it">The iterator that this iterator is being compared to</param>
			/// <returns>True if the iterators are not equal, false otherwise</returns>
			bool operator!=(const Iterator& it) const;
			/// <summary>
			/// Prefix increment operator. Moves the iterator to the next index in the list or to the next index in the current SList if the current bucket has a collision
			/// </summary>
			/// <returns>A reference to the current iterator after it has been mutated to the next index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is greater than or equal to size and mChainIterator = end()</exception>
			Iterator& operator++();
			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix increment operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the next index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is greater than or equal to size and mChainIterator = end()</exception>
			Iterator operator++(int);
		};
#pragma endregion

#pragma region ConstIterator
		class ConstIterator final
		{
			friend HashMap;

		private:
			/// <summary>
			/// A pointer to the list that owns this ConstIterator
			/// </summary>
			const HashMap* mOwner = nullptr;
			/// <summary>
			/// The current node that this ConstIterator points to
			/// </summary>
			size_t mIndex = 0;
			/// <summary>
			/// The ConstIterator used to step through the ChainList at mIndex
			/// </summary>
			typename ChainType::ConstIterator mChainIterator;

			/// Default constructor with the data index and list owner
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="index">index for mIndex to be set to</param>
			/// <param name="chainIterator">The ConstIterator of the current chain list that the data is contained</param>
			ConstIterator(const HashMap& owner, size_t index, const typename ChainType::ConstIterator& chainIterator);

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = PairType;
			using pointer = PairType*;
			using reference = PairType;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor with no arguments using compiler default
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Constructor that makes a new ConstIterator of a passed in Iterator
			/// </summary>
			/// <param name="it"></param>
			ConstIterator(const Iterator& it);
			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="CopyIterator">ConstIterator to copy</param>
			ConstIterator(const ConstIterator&) = default;
			/// <summary>
			/// Default move operator
			/// </summary>
			/// <param name="MoveIterator">The iterator to be moved</param>
			ConstIterator(ConstIterator&&) noexcept = default;
			/// <summary>
			/// Copy assignment operator
			/// </summary>
			/// <param name="CopyIterator">The ConstIterator on the rhs of the = operator that will be copied</param>
			/// <returns>A copied version of the passed in ConstIterator</returns>
			ConstIterator& operator=(const ConstIterator&) = default;
			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="MoveIterator">ConstIterator that the information will be moved from</param>
			/// <returns>A reference to the new list</returns>
			ConstIterator& operator=(ConstIterator&&) noexcept = default;
			/// <summary>
			/// Default destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>A constant reference to the data contained at mIndex</returns>
			const PairType& operator*() const;
			/// <summary>
			/// Dereference arrow operator
			/// </summary>
			/// <returns>Address of a reference to the data contained within mChainIterator</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is >= size </exception>
			const PairType* operator->() const;
			/// <summary>
			/// Compares two ConstIterators for equality
			/// </summary>
			/// <param name="it">The ConstIterator that this ConstIterator is being compared to</param>
			/// <returns>True if the ConstIterators are the same, false otherwise</returns>
			bool operator==(const ConstIterator& it) const;
			/// <summary>
			/// Compares two ConstIterators for inequality
			/// </summary>
			/// <param name="it">The ConstIterator that this ConstIterator is being compared to</param>
			/// <returns>True if the ConstIterators are not equal, false otherwise</returns>
			bool operator!=(const ConstIterator& it) const;
			/// <summary>
			/// Prefix increment operator. Moves the iterator to the next index in the list or to the next index in the current SList if the current bucket has a collision
			/// </summary>
			/// <returns>A reference to the current iterator after it has been mutated to the next index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is greater than or equal to size and mChainIterator = end()</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix increment operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is greater than or equal to size and mChainIterator = end()</exception>
			ConstIterator operator++(int);
		};
#pragma endregion

#pragma region MemberMethods
		/// <summary>
		/// Default constructor that initializes the list to empty with size 0 or a user defined size
		/// </summary>
		/// <param name="size">User defined starting size for the hashmap. This will assert in debug mode if size is 0</param>
		/// <param name="hashFunctor">A user defined hash function for hashing the keys</param>
		/// <param name="keyEquality">A user defined equality comparison for keys</param>
		explicit HashMap(size_t bucketSize = 11, HashFunctor hashFunctor = DefaultHash<TKey>(), KeyEqualityFunctor keyEquality = DefaultEquality<TKey>());
		/// <summary>
		/// Initializer list constructor that initializes the map to contain the passed in elements with the specified bucket size.
		/// </summary>
		/// <param name="list">List of initial pairs to be put inside the map</param>
		/// <param name="size">User defined starting size for the hashmap. This will assert in debug mode if size is 0</param>
		/// <param name="hashFunctor">A user defined hash function for hashing the keys</param>
		/// <param name="keyEquality">A user defined equality comparison for keys</param>
		HashMap(std::initializer_list<PairType> list, size_t bucketSize = 0, HashFunctor hashFunctor = DefaultHash<TKey>(), KeyEqualityFunctor keyEquality = DefaultEquality<TKey>());
		/// /// <summary>
		/// Copy constructor that deep copies the HashMap passed in
		/// </summary>
		/// <param name="rhs">The HashMap to be deep copied</param>
		HashMap(const HashMap& rhs);
		/// <summary>
		/// Move constructor that moves the data of an r value HashMap into this map
		/// </summary>
		/// <param name="rhs">The r value HashMap to be moved</param>
		HashMap(HashMap&& rhs) noexcept;
		/// <summary>
		/// Default destructor that frees all memory used by the map
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Copy assignment operator that deep copies the right hand side HashMap into this map
		/// </summary>
		/// <param name="rhs">The HashMap to be deep copied</param>
		/// <returns>A reference to this HashMap</returns>
		HashMap& operator=(const HashMap& rhs);
		/// <summary>
		/// Move assignment operator that moves the data from the right hand side map into this map
		/// </summary>
		/// <param name="rhs">The HashMap to be moved</param>
		/// <returns>A reference to this HashMap</returns>
		HashMap& operator=(HashMap&& rhs) noexcept;

		/// <summary>
		/// Initializer list assignment operator that sets the content of the map to the rhs list
		/// </summary>
		/// <param name="list">List of initial pairs to be put inside the map</param>
		HashMap& operator=(std::initializer_list<PairType> list);
#pragma endregion

#pragma region ElementAccess
		/// <summary>
		/// Finds the data associated with the given key
		/// </summary>
		/// <param name="key">The key whose associated data should be returned</param>
		/// <returns>A reference to the data associated with the key</returns>
		/// <exception cref="std::runtime_error">Throws an exception if there is no data associated with the key</exception>
		TData& At(const TKey& key);
		/// <summary>
		/// Const version of the At method that returns a constant ref of the data
		/// </summary>
		/// <param name="key">The key whose associated data should be returned</param>
		/// <returns>A const reference to the data associated with the key</returns>
		/// <exception cref="std::runtime_error">Throws an exception if there is no data associated with the key</exception>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Finds the key value pair within the HashMap that contains the specified data and returns an Iterator pointing to it.
		/// </summary>
		/// <param name="value">The key of the data you are looking for</param>
		/// <param name="index">Output param that returns the index of the found item</param>
		/// <returns>Iterator pointing to the key value pair that contains the specified data. Returns end() if the HashMap did not contain the key at all</returns>
		Iterator Find(const TKey& key, size_t& index);
		/// <summary>
		/// Finds the key value pair within the HashMap that contains the specified data and returns an Iterator pointing to it
		/// </summary>
		/// <param name="value">The key of the data you are looking for</param>
		/// <returns>Iterator pointing to the key value pair that contains the specified data. Returns end() if the HashMap did not contain the key at all</returns>
		Iterator Find(const TKey& key);
		/// <summary>
		/// Finds the key value pair within the HashMap that contains the specified data and returns a ConstIterator pointing to it
		/// </summary>
		/// <param name="value">The key of the data you are looking for</param>
		/// <returns>ConstIterator pointing to the key value pair that contains the specified data. Returns end() if the HashMap did not contain the key at all</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Checks if the HashMap contains a pair with the passed in key
		/// </summary>
		/// <param name="key">The key that is being checked if it is within the HashMap</param>
		/// <returns>True if the key is within the HashMap, false otherwise</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Returns the number of elements contained within the HashMap
		/// </summary>
		/// <returns>The size of the HashMap</returns>
		size_t Size() const;

		/// <summary>
		///Returns a number representing the number of slots (with chaining) contained within the HashMap
		/// </summary>
		/// <returns>The size of the internal vector</returns>
		size_t BucketSize() const;
#pragma endregion

#pragma region Modifiers
		/// <summary>
		/// Returns a reference to the data that the key is pointing to. If the key is not inside the HashMap it inserts a new pair with the key pointing to default data
		/// </summary>
		/// <param name="key">The key to be used to find existing data or insert a new key value pair with</param>
		/// <returns>A reference to the data associated with the key</returns>
		TData& operator[](const TKey& key);
		/// <summary>
		/// Returns a reference to the data that the key is pointing to
		/// </summary>
		/// <param name="key">The key to be used to find its associated data</param>
		/// <returns>A const reference to the data associated with the key</returns>
		/// <exception cref="std::runtime_error">Throws an exception if there is no data associated with the key</exception>
		const TData& operator[](const TKey& key) const;

		/// <summary>
		/// Hash's the key of the PairType passed in and inserts the PairType at that index. 
		/// </summary>
		/// <param name="data">The key value pair to be inserted into the HashMap</param>
		/// <returns>A std::pair containing and Iterator pointing to the inserted pair or an already existing one with that key, and a bool that is true if the data was inserted or false if the key already existed</returns>
		std::pair<Iterator, bool> Insert(const PairType& data);

		/// <summary>
		/// Removes the std::pair in the HashMap that contains the passed in key
		/// </summary>
		/// <param name="key">The key of the pair of data to be removed</param>
		/// <returns>True if the key was removed, false if the key was not in the HashMap</returns>
		bool Remove(const TKey& key);
		/// <summary>
		/// Removes the std::pair in the HashMap that the passed in iterator is pointing to 
		/// </summary>
		/// <param name="it">The iterator that contains the data of what should be removed</param>
		/// <returns>True if the pair was removed, false if the pair was not in the HashMap</returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Resizes the number of slots within the HashMap. Rehashes all of the key value pairs to their new associated indices within the inner vector
		/// </summary>
		/// <param name="size">The new size of the HashMap. This will assert in debug mode if size is 0</param>
		/// <exception cref="std::runtime_error">Throws this exception if the method is unable to reallocate new memory for the inner vector</exception>
		void Resize(const size_t size);

		/// <summary>
		/// Clears all data contained within the HashMap. Does not affect the size of the map
		/// </summary>
		void Clear();
#pragma endregion

#pragma region IteratorMethods
		/// <summary>
		/// Creates and returns an iterator that points to the index and ChainType::Iterator of the first element within the HashMap
		/// </summary>
		/// <returns>Iterator that points to the first index and ChainType::Iterator of the HashMap</returns>
		Iterator begin();
		/// <summary>
		/// Creates and returns a ConstIterator that points to the index and ChainType::ConstIterator of the first element within the HashMap
		/// </summary>
		/// <returns>ConstIterator that points to the first index and ChainType::ConstIterator of the HashMap</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Creates and returns a ConstIterator that points to the index and ChainType::ConstIterator of the first element within the HashMap
		/// </summary>
		/// <returns>ConstIterator that points to the first index and ChainType::ConstIterator of the HashMap</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Creates and returns an iterator whose index is the size of the vector and ChainType::Iterator is defaulted
		/// </summary>
		/// <returns>Iterator that points past the last index of the vector, equal to mSize, and whose ChainType::Iterator is defaulted</returns>
		Iterator end();
		/// <summary>
		/// Creates and returns a ConstIterator whose index is the size of the vector and ChainType::ConstIterator is defaulted
		/// </summary>
		/// <returns>ConstIterator that points past the last index of the vector, equal to mSize, and whose ChainType::ConstIterator is defaulted</returns>
		ConstIterator end() const;
		/// <summary>
		/// Creates and returns a ConstIterator whose index is the size of the vector and ChainType::ConstIterator is defaulted
		/// </summary>
		/// <returns>ConstIterator that points past the last index of the vector, equal to mSize, and whose ChainType::ConstIterator is defaulted</returns>
		ConstIterator cend() const;
#pragma endregion

	private:
		/// <summary>
		/// The number of elements contained within the HashMap.
		/// </summary>
		size_t mSize = 0;
		/// <summary>
		/// The vector that contains all the key value pairs within the HashMap
		/// </summary>
		BucketType mBuckets;
		/// <summary>
		/// The functor used to hash the key values into indices
		/// </summary>
		HashFunctor mHashFunctor = DefaultHash<const TKey>();
		/// <summary>
		/// The functor used to compare keys for equality
		/// </summary>
		KeyEqualityFunctor mKeyEquality = DefaultEquality<const TKey>();
	};
}

#include "HashMap.inl"