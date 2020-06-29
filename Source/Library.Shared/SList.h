#pragma once

#include <functional>
#include <initializer_list>
#include "DefaultEquality.h"

namespace Library
{
	/// <summary>
	/// Represents a generic singly linked list
	/// </summary>
	template <typename T>
	class SList
	{
	private:

#pragma region Node
		/// <summary>
		/// Represents a node of data within a list. Contains the pointer to the next node.
		/// </summary>
		struct Node final
		{
			/// <summary>
			/// Data of type T contained within this Node
			/// </summary>
			T Data;
			/// <summary>
			/// Pointer to the next Node in the list
			/// </summary>
			Node* Next;

			/// <summary>
			/// Constructor for a Node with passed initial parameters
			/// </summary>
			/// <param name="data">The data contained within the node</param>
			/// <param name="next">Pointer to the next node</param>
			Node(const T& data, Node* next = nullptr);
		};
#pragma endregion Node

	public:
#pragma region Iterator
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;

		private:
			/// <summary>
			/// A pointer to the list that owns this iterator
			/// </summary>
			const SList* mOwner = nullptr;
			/// <summary>
			/// The current node that this iterator points to
			/// </summary>
			Node* mNode = nullptr;

			/// <summary>
			/// Default constructor with the current node and list owner
			/// </summary>
			/// <param name="node">node for mCurrentNode to be set to</param>
			/// <param name="owner">the owner of this iterator</param>
			Iterator(Node* node, const SList* owner);

		public:
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
			/// <returns>A reference to the data contained within mCurrentNode</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators current node is a nullptr</exception>
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
			/// Prefix increment operator. Moves the iterator to the next node in the list
			/// </summary>
			/// <returns>A reference to the current iterator after it has been mutated to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators current node is a nullptr</exception>
			Iterator& operator++();
			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix increment operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators current node is a nullptr</exception>
			Iterator operator++(int);
		};
#pragma endregion

#pragma region ConstIterator
		class ConstIterator final
		{
			friend SList;

		private:
			/// <summary>
			/// A pointer to the list that owns this iterator
			/// </summary>
			const SList* mOwner = nullptr;
			/// <summary>
			/// The current node that this iterator points to
			/// </summary>
			const Node* mNode = nullptr;

			/// <summary>
			/// Constructor with the current node and list owner
			/// </summary>
			/// <param name="node">node for mCurrentNode to be set to</param>
			/// <param name="owner">the owner of this iterator</param>
			ConstIterator(const Node* node, const SList* owner);

		public:
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
			/// <returns>A constant reference to the data contained within mCurrentNode</returns>
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
			/// Prefix increment operator. Moves the ConstIterator to the next node in the list
			/// </summary>
			/// <returns>A reference to the current ConstIterator after it has been mutated to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if the ConstIterators current node is a nullptr</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Postfix increment operator
			/// </summary>
			/// <param>int used to differentiate this operator from the prefix increment operator</param>
			/// <returns>A copy of the current iterator before it gets mutated to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if iterators current node is a nullptr</exception>
			ConstIterator operator++(int);
		};
#pragma endregion

		/// <summary>
		/// Default constructor that initializes the list to empty
		/// </summary>
		SList() = default;
		/// <summary>
		/// Copy constructor that deep copies the list passed in
		/// </summary>
		/// <param name="list">The list to be deep copied into the current instance of SList</param>
		SList(const SList& rhs);
		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">The object being moved</param>
		SList(SList&& rhs) noexcept;
		/// <summary>
		/// Destructor that assures there are no memory leaks
		/// </summary>
		~SList();

		/// <summary>
		/// Assignment operator overload that copies the entire list into the new list
		/// </summary>
		/// <param name="list">The list that is being copied to this list</param>
		/// <returns>A reference to the new list</returns>
		SList& operator=(const SList& rhs);
		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">The list on the rhs of the assignment operator that is being moved</param>
		/// <returns>A reference to the new list</returns>
		SList& operator=(SList&& rhs) noexcept;


		/// <summary>
		/// Returns the boolean value of whether or not the list contains any nodes or not
		/// </summary>
		/// <returns>True if the list is empty, False if the list has Nodes</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Gets the number of Nodes contained within the list
		/// </summary>
		/// <returns>The number of Nodes within the list</returns>
		size_t Size() const;

		/// <summary>
		/// Gets a reference to the data contained within the front Node of the list
		/// </summary>
		/// <returns>Reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the list is empty</exception>
		T& Front();
		/// <summary>
		/// Gets a constant reference to the data contained within the front Node of the list
		/// </summary>
		/// <returns>Constant reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the list is empty</exception>
		const T& Front() const;
		/// <summary>
		/// Gets a reference to the data contained within the back Node of the list
		/// </summary>
		/// <returns>Reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the list is empty</exception>
		T& Back();
		/// <summary>
		/// Gets a constant reference to the data contained within the back Node of the list
		/// </summary>
		/// <returns>Constant reference to data of type T</returns>
		/// <exception cref="runtime_error">Throws a runtime error if the list is empty</exception>
		const T& Back() const;
		
		/// <summary>
		/// Pushes a reference to data of type T into the front of the list
		/// </summary>
		/// <param name="data">The data being pushed into the list</param>
		Iterator PushFront(const T& data);
		/// <summary>
		/// Pushes a reference to data of type T into the back of the list
		/// </summary>
		/// <param name="data">The data being pushed into the list</param>
		Iterator PushBack(const T& data);
		/// <summary>
		/// Pops the front node off the list and deletes it. Sets mFront to the next Node in the list
		/// </summary>
		void PopFront();
		/// <summary>
		/// Pops the back node off the list and deletes it. Sets mBack to the previous Node in the list
		/// </summary>
		void PopBack();
		/// <summary>
		/// Removes all items in the list
		/// </summary>
		void Clear();

		/// <summary>
		/// Creates and returns an iterator that points to the first node in the SList
		/// </summary>
		/// <returns>Iterator that points to the first node in the list</returns>
		Iterator begin();
		/// <summary>
		/// Constant version of the begin method. Calls cbegin()
		/// </summary>
		/// <returns>ConstIterator that points to the first node in the list</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Creates and returns a ConstIterator that points to the first node in the SList
		/// </summary>
		/// <returns>ConstIterator that points to the first node in the list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Creates and returns an iterator that points to the node past the last node of the list (nullptr) 
		/// </summary>
		/// <returns>Iterator that points to the node past the last node of the list</returns>
		Iterator end();
		/// <summary>
		/// Constant version of the begin method. Calls cbegin()
		/// </summary>
		/// <returns>ConstIterator that points to the first node in the list</returns>
		ConstIterator end() const;
		/// <summary>
		/// Creates and returns a ConstIterator that points to the node past the last node of the list (nullptr) 
		/// </summary>
		/// <returns>ConstIterator that points to the node past the last node of the list</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Inserts a new node containing the specified data after the node pointed at by the passed in Iterator
		/// </summary>
		/// <param name="data">Data to be contained within the new node</param>
		/// <param name="it">Iterator to have the new node inserted after</param>
		/// <returns>Iterator pointing to the new node created</returns>
		Iterator InsertAfter(const T& data, const Iterator& it);
		/// <summary>
		/// Finds the first node within the list that contains the specified data and returns an Iterator pointing to it
		/// </summary>
		/// <param name="value">The value of the node you are looking for</param>
		/// <param name="EqualityCompare">An equality compare functor that returns a boolean value that represents equality between two variables of type T</param>
		/// <returns>Iterator pointing to the node that contains the specified data. Returns end() if the list did not contain the data at all</returns>
		Iterator Find(const T& value, std::function<bool(const T& lhs, const T& rhs)> EqualityCompare = DefaultEquality<T>{});
		/// <summary>
		/// Finds the first node within the list that contains the specified data and returns a ConstIterator pointing to it
		/// </summary>
		/// <param name="value">The value of the node you are looking for</param>
		/// <param name="EqualityCompare">An equality compare functor that returns a boolean value that represents equality between two variables of type T</param>
		/// <returns>ConstIterator pointing to the node that contains the specified data. Returns cend() if the list did not contain the data at all</returns>
		ConstIterator Find(const T& value, std::function<bool(const T& lhs, const T& rhs)> EqualityCompare = DefaultEquality<T>{}) const;
		/// <summary>
		/// Removes the first node in the list that contains the value passed in 
		/// </summary>
		/// <param name="value">The data whose corresponding node needs to be removed from the list</param>
		/// <returns>True if a node was removed, false otherwise</returns>
		bool Remove(const T& value);
		/// <summary>
		/// Removes the node contained by the current iterator from the SList
		/// </summary>
		/// <param name="data">Iterator whose node should be removed from the list</param>
		/// <returns>True if the data was removed, false otherwise</returns>
		bool Remove(const Iterator& data);

	private:
		/// <summary>
		/// Number of nodes that contain data within the list
		/// </summary>
		size_t mSize = 0;
		/// <summary>
		/// Pointer to the front node within the list
		/// </summary>
		Node* mFront = nullptr;
		/// <summary>
		/// Pointer to the back node within the list
		/// </summary>
		Node* mBack = nullptr;
	};
}

#include "SList.inl"

