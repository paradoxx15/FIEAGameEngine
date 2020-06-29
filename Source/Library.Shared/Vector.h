#pragma once

#include <functional>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <cassert>
#include "DefaultEquality.h"

namespace Library
{
	template <typename T>
	class Vector
	{
	public:
		using ReserveStrategy = typename std::function<size_t(const size_t, const size_t)>;
		using value_type = T;

#pragma region Iterator
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		private:
			/// <summary>
			/// A pointer to the list that owns this iterator
			/// </summary>
			Vector* mOwner = nullptr;
			/// <summary>
			/// The current index that this iterator points to
			/// </summary>
			size_t mIndex = 0;

			/// <summary>
			/// Default constructor with the data index and list owner
			/// </summary>
			/// <param name="node">node for mCurrentNode to be set to</param>
			/// <param name="owner">the owner of this iterator</param>
			Iterator(size_t index, Vector& owner);

		public:
			using size_type = size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T;
			using iterator_category = std::random_access_iterator_tag;

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
			/// <returns>A reference to the data contained at mIndex</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is >= size </exception>
			T& operator*() const;
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
			/// Prefix increment operator. Moves the iterator to the next index in the list
			/// </summary>
			/// <returns>A reference to the current iterator after it has been mutated to the next index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is greater than or equal to size</exception>
			Iterator& operator++();
			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix increment operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the next index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is greater than or equal to size</exception>
			Iterator operator++(int);
			/// <summary>
			/// Prefix decrement operator. Moves the iterator to the previous index in the list
			/// </summary>
			/// <returns>A reference to the current iterator after it has been mutated to the previous index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is equal to zero</exception>
			Iterator& operator--();
			/// <summary>
			/// Postfix decrement operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix decrement operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the previous index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is equal to zero</exception>
			Iterator operator--(int);
		};
#pragma endregion

#pragma region ConstIterator
		class ConstIterator final
		{
			friend Vector;

		private:
			/// <summary>
			/// A pointer to the list that owns this ConstIterator
			/// </summary>
			const Vector* mOwner = nullptr;
			/// <summary>
			/// The current node that this ConstIterator points to
			/// </summary>
			size_t mIndex = 0;

			/// <summary>
			/// Constructor with the data index and list owner
			/// </summary>
			/// <param name="node">node for mCurrentNode to be set to</param>
			/// <param name="owner">the owner of this iterator</param>
			ConstIterator(size_t index, const Vector& owner);

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T;
			using iterator_category = std::random_access_iterator_tag;

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
			const T& operator*() const;
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
			/// Prefix increment operator. Moves the ConstIterator to the next index in the vector
			/// </summary>
			/// <returns>A reference to the current ConstIterator after it has been mutated to the next index</returns>
			/// <exception cref="std::runtime_error">Throws exception if the ConstIterators current index is >= size</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix increment operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators current index is >= size</exception>
			ConstIterator operator++(int);
			/// <summary>
			/// Prefix decrement operator. Moves the ConstIterator to the previous index in the list
			/// </summary>
			/// <returns>A reference to the current ConstIterator after it has been mutated to the previous index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is equal to zero</exception>
			ConstIterator& operator--();
			/// <summary>
			/// Postfix decrement operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix decrement operator</param>
			/// <returns>A copy of the current ConstIterator before it gets mutated to the previous index</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators index is equal to zero</exception>
			ConstIterator operator--(int);
		};
#pragma endregion

#pragma region Vector

#pragma region MemberMethods
		/// <summary>
		/// Default constructor that initializes the list to empty
		/// </summary>
		explicit Vector(size_t capacity = 0, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Constructor using an initializer list to set the starting values of the vector
		/// </summary>
		/// <param name="list">List of data of type T to be used in the initialization of the vector</param>
		Vector(std::initializer_list<T> list);
		/// <summary>
		/// Copy constructor that deep copies the vector passed in
		/// </summary>
		/// <param name="rhs">The vector to be deep copied</param>
		Vector(const Vector& rhs);
		/// <summary>
		/// Move constructor that moves the data of an r value vector into this vector
		/// </summary>
		/// <param name="rhs">The r value vector to have its data moved</param>
		Vector(Vector&& rhs) noexcept;
		/// <summary>
		/// Destructor that clears the vector and frees any allocated memory
		/// </summary>
		virtual ~Vector();

		/// <summary>
		/// Copy assignment operator that copies the entire right hand side vector into this vector
		/// </summary>
		/// <param name="rhs">The list that is being compared for equality</param>
		/// <returns>A reference to this vector</returns>
		Vector& operator=(const Vector& rhs);
		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">The vector on the rhs of the assignment operator that is being moved</param>
		/// <returns>A reference to this vector</returns>
		Vector& operator=(Vector&& rhs) noexcept;
#pragma endregion

#pragma region ElementAccess
		/// <summary>
		/// Bracket operator that allows constant time access to any value in the data array
		/// </summary>
		/// <param name="index">The index of the value which should be returned</param>
		/// <returns>A reference to the T element contained in the vector at the specified index</returns>
		/// <exception cref="std::runtime_error">Thows a runtime error if the vector is not initialized or the passed in index is out of the vectors range</exception>
		T& operator[](size_t index);
		/// <summary>
		/// Const bracket operator that allows constant time access to any value in the data array
		/// </summary>
		/// <param name="index">The index of the value which should be returned</param>
		/// <returns>A constant reference to the T element contained in the vector at the specified index</returns>
		/// <exception cref="std::runtime_error">Thows a runtime error if the vector is not initialized or the passed in index is out of the vectors range</exception>
		const T& operator[](size_t index) const;

		/// <summary>
		/// Allows constant time access to any value in the data array
		/// </summary>
		/// <param name="index">The index of the value which should be returned</param>
		/// <returns>A reference to the T element contained in the vector at the specified index</returns>
		/// <exception cref="std::runtime_error">Thows a runtime error if the vector is not initialized or the passed in index is out of the vectors range</exception>
		T& At(size_t index);
		/// <summary>
		/// Allows constant time access to any value in the data array but does not allow mutation
		/// </summary>
		/// <param name="index">The index of the value which should be returned</param>
		/// <returns>A constant reference to the T element contained in the vector at the specified index</returns>
		/// <exception cref="std::runtime_error">Thows a runtime error if the vector is not initialized or the passed in index is out of the vectors range</exception>
		const T& At(size_t index) const;

		/// <summary>
		/// Gets a reference to the data contained at the first index of the vector
		/// </summary>
		/// <returns>Reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the vector is empty</exception>
		T& Front();
		/// <summary>
		/// Gets a reference to the data contained at the first index of the vector
		/// </summary>
		/// <returns>Const reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the vector is empty</exception>
		const T& Front() const;

		/// <summary>
		/// Gets a reference to the data contained at the last index of the vector
		/// </summary>
		/// <returns>Reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the vector is empty</exception>
		T& Back();
		/// <summary>
		/// Gets a reference to the data contained at the last index of the vector
		/// </summary>
		/// <returns>Const reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the vector is empty</exception>
		const T& Back() const;

		/// <summary>
		/// Finds the first index within the vector that contains the specified data and returns an Iterator pointing to it
		/// </summary>
		/// <param name="value">The value of the data you are looking for</param>
		/// <param name="EqualityCompare">An equality compare functor that returns a boolean value that represents equality between two variables of type T</param>
		/// <returns>Iterator pointing to the index that contains the specified data. Returns end() if the vector did not contain the data at all</returns>
		Iterator Find(const T& value, std::function<bool(const T & lhs, const T & rhs)> EqualityCompare = DefaultEquality<T>{});
		/// <summary>
		/// Finds the first index within the vector that contains the specified data and returns an Iterator pointing to it
		/// </summary>
		/// <param name="value">The value of the data you are looking for</param>
		/// <param name="EqualityCompare">An equality compare functor that returns a boolean value that represents equality between two variables of type T</param>
		/// <returns>ConstIterator pointing to the index that contains the specified data. Returns end() if the vector did not contain the data at all</returns>
		ConstIterator Find(const T& value, std::function<bool(const T & lhs, const T & rhs)> EqualityCompare = DefaultEquality<T>{}) const;
#pragma endregion

#pragma region Capacity
		/// <summary>
		/// Checks whether or not the vector contains any data
		/// </summary>
		/// <returns>True if size is equal to zero, false otherwise</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Allocates space for a user specified amount of data and moves over any existing data. Does not shrink the vector if the passed in capacity is smaller than the currently allocated capacity
		/// </summary>
		/// <param name="capacity">The new capacity the vector should have</param>
		/// <exception cref="std::runtime_error">Throws this exception if the method is unable to reallocate new memory for the vector</exception>
		void Reserve(size_t capacity);
		/// <summary>
		/// Increases or decreases both the size and capacity of the vector. If the size is increased it will initialize default constructed versions of T data in those spaces
		/// </summary>
		/// <param name="newSize">The new size and capacity that the vector should become</param>
		/// <exception cref="std::runtime_error">Throws this exception if the method is unable to reallocate new memory for the vector</exception>
		void Resize(size_t newSize);
		/// <summary>
		/// Resizes the capacity of the array to be equal to the size
		/// </summary>
		void ShrinkToFit();
		/// <summary>
		/// Returns a count of how many elements are in the vector
		/// </summary>
		/// <returns>The size of the vector</returns>
		size_t Size() const;
		/// <summary>
		/// Returns the capacity of how many elements can be contained within the vector
		/// </summary>
		/// <returns>The capacity of the vector</returns>
		size_t Capacity() const;
#pragma endregion

#pragma region Modifiers
		/// <summary>
		/// Destructs the last element within the vector. Does nothing if the vector is empty
		/// </summary>
		void PopBack();
		/// <summary>
		/// Pushes the user passed data into the vector at the back of the vector. Will increase the capacity of the vector if it is already filled. Uses a default or user defined reserve strategy for increasing capacity
		/// </summary>
		/// <param name="value">The value to be added to the vector</param>
		/// <returns>An iterator pointing to the index of the new value added to the vector</returns>
		Iterator PushBack(const T& value);
		/// <summary>
		/// Pushes the R-value user passed data into the vector at the back of the vector. Will increase the capacity of the vector if it is already filled. Uses a default or user defined reserve strategy for increasing capacity
		/// </summary>
		/// <param name="value">The value to be added to the vector</param>
		/// <returns>An iterator pointing to the index of the new value added to the vector</returns>
		Iterator PushBack(T&& value);

		/// <summary>
		/// Removes any element from the vector. Does not harm the vector if the passed in value is not contained within the vector
		/// </summary>
		/// <param name="value">The value of the element to be removed from the vector</param>
		/// <returns>True if the element was removed, false otherwise</returns>
		bool Remove(const T& value);
		/// <summary>
		/// Removes any element from the vector by being passed an iterator pointing to that elements index. Does not harm the vector if the passed in value is not contained within the vector
		/// </summary>
		/// <param name="value">The iterator pointing at the index of the element to be removed</param>
		/// <returns>True if the element was removed, false otherwise</returns>
		bool Remove(const Iterator& value);
		/// <summary>
		/// Removes a range of elements from the start Iterator to the end Iterator
		/// </summary>
		/// <param name="start">An iterator pointing to the start of the range of elements to be deleted (Inclusive)</param>
		/// <param name="end">An iterator pointing to the end of the range of elements to be deleted (Exclusive)</param>
		/// <returns>True if the range was successfully removed, false otherwise</returns>
		bool Remove(const Iterator& start, const Iterator& end);

		/// <summary>
		/// Destructs every element within the vector. Does not effect the capacity of the vector
		/// </summary>
		void Clear();
#pragma endregion
		
#pragma region IteratorMethods
		/// <summary>
		/// Creates and returns an iterator that points to the index of the first element within the vector
		/// </summary>
		/// <returns>Iterator that points to the first index in the vector</returns>
		Iterator begin();
		/// <summary>
		/// Constant version of the begin method. Calls cbegin()
		/// </summary>
		/// <returns>ConstIterator that points to the first index in the list</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Creates and returns a ConstIterator that points to the firs index in the vector
		/// </summary>
		/// <returns>ConstIterator that points to the first index in the vector</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Creates and returns an iterator whose index is the size of the vector 
		/// </summary>
		/// <returns>Iterator that points past the last index of the vector, equal to mSize</returns>
		Iterator end();
		/// <summary>
		/// Constant version of the end method. Calls cend()
		/// </summary>
		/// <returns>ConstIterator that points past the last index of the vector, equal to mSize</returns>
		ConstIterator end() const;
		/// <summary>
		/// Creates and returns a ConstIterator whose index is the size of the vector 
		/// </summary>
		/// <returns>Iterator that points past the last index of the vector, equal to mSize</returns>
		ConstIterator cend() const;
#pragma endregion

	private:
		/// <summary>
		/// A default reserve strategy functor that increases the capacity by a factor of 1.5
		/// </summary>
		struct DefaultRerserveStrategy
		{
			/// <summary>
			/// Returns a new capacity that is increased by a factor of 1.5
			/// </summary>
			/// <param>Empty param not used for the default reserve strategy</param>
			/// <param name="capacity">The current capacity of the vector</param>
			/// <returns>A size_t of the capacity multiplied by a factor of 1.5</returns>
			const size_t operator()(const size_t, const size_t capacity) const
			{
				return static_cast<size_t>(capacity * 1.5);
			}
		};

		/// <summary>
		/// The starting address of the array of data contained within the vector
		/// </summary>
		T* mData = nullptr;
		/// <summary>
		/// The number of elements in the vector
		/// </summary>
		size_t mSize = 0;
		/// <summary>
		/// The number of elements able to be contained within the vector
		/// </summary>
		size_t mCapacity = 0;

		ReserveStrategy mReserveStrategy = DefaultRerserveStrategy();
#pragma endregion
	};
}

#include "Vector.inl"
