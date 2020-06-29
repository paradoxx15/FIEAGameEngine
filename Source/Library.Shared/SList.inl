#include <stdexcept>
#include "SList.h"

namespace Library
{
#pragma region Node

	template<typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		Data(data), Next(next)
	{
	}

#pragma endregion

#pragma region Iterator
	template<typename T>
	inline SList<T>::Iterator::Iterator(Node* node, const SList* owner) : mNode(node), mOwner(owner) {}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("This iterators node is the end node");
		}

		return mNode->Data;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& it) const
	{
		return !(*this != it);
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& it) const
	{
		return (mOwner != it.mOwner || mNode != it.mNode);
	}

	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("This iterators node is the end node");
		}

		mNode = mNode->Next;
		return *this;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}
#pragma endregion

#pragma region ConstIterator
	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Node* node, const SList* owner) : mNode(node), mOwner(owner) {}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& it) : mNode(it.mNode), mOwner(it.mOwner) {}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("This ConstIterators node is the end node");
		}

		return mNode->Data;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& it) const
	{
		return !(*this != it);
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& it) const
	{
		if (mOwner != it.mOwner || mNode != it.mNode)
		{
			return true;
		}

		return false;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("This ConstIterators node is the end node");
		}

		mNode = mNode->Next;
		return *this;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		operator++();
		return it;
	}
#pragma endregion

#pragma region SList

	template<typename T>
	SList<T>::SList(SList&& rhs) noexcept :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.mSize = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	SList<T>::SList(const SList& rhs)
	{
		Node* listNode = rhs.mFront;
		while (listNode != nullptr)
		{
			PushBack(listNode->Data);
			listNode = listNode->Next;
		}
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Node* listNode = rhs.mFront;
			while (listNode != nullptr)
			{
				PushBack(listNode->Data);
				listNode = listNode->Next;
			}
		}

		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();

			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;

			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty");
		}

		return mFront->Data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty");
		}

		return mFront->Data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty");
		}

		return mBack->Data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty");
		}

		return mBack->Data;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		mFront = new Node(data, mFront);
		if (IsEmpty())
		{
			mBack = mFront;
		}

		mSize++;

		return Iterator(mFront, this);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		Node* newBack = new Node(data, nullptr);

		if (IsEmpty())
		{
			mFront = mBack = newBack;
		}
		else
		{
			mBack->Next = newBack;
			mBack = mBack->Next;
		}

		mSize++;

		return Iterator(mBack, this);
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			Node* node = mFront->Next;
			delete mFront;
			mFront = node;
			mSize--;

			if (mSize <= 1)
			{
				mBack = mFront;
			}
		}
	}

	template<typename T>
	void SList<T>::PopBack()
	{
		if (!IsEmpty())
		{
			if (mSize == 1)
			{
				delete mFront;
				mFront = mBack = nullptr;
			}
			else
			{
				Node* newBack = mFront;
				for (Node* curNode = mFront; curNode->Next != nullptr; curNode = curNode->Next)
				{
					newBack = curNode;
				}
				mBack = newBack;
				delete newBack->Next;
				mBack->Next = nullptr;
			}

			mSize--;
		}
	}

	template<typename T>
	void SList<T>::Clear()
	{
		if (!IsEmpty())
		{
			Node* prevNode;
			Node* curNode = mFront;
			while(curNode != nullptr)
			{
				prevNode = curNode;
				curNode = curNode->Next;
				delete prevNode;
			}
			mFront = mBack = nullptr;
			mSize = 0;
		}
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(mFront, this);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const 
	{
		return cbegin();
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(mFront, this);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end() 
	{
		return Iterator(nullptr, this);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const 
	{
		return cend();
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(nullptr, this);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const T& data, const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("This list does not own the passed iterator");
		}

		if (it.mNode == nullptr || it.mNode == mBack)
		{
			PushBack(data);
			return Iterator(mBack, this);
		}

		Node* newNode = new Node(data, it.mNode->Next);
		it.mNode->Next = newNode;
		mSize++;

		return Iterator(newNode, this);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value, std::function<bool(const T& lhs, const T& rhs)> EqualityCompare)
	{

		for (Iterator it = begin(); it != end(); ++it)
		{
			if (EqualityCompare(it.mNode->Data, value))
			{
				return it;
			}
		}

		return end();
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::Find(const T& value, std::function<bool(const T& lhs, const T& rhs)> EqualityCompare) const
	{
		for (ConstIterator it = begin(); it != end(); ++it)
		{
			if (EqualityCompare(it.mNode->Data, value))
			{
				return it;
			}
		}

		return cend();
	}

	template<typename T>
	inline bool SList<T>::Remove(const T& value)
	{
		return Remove(Find(value));
	}

	template<typename T>
	bool SList<T>::Remove(const SList<T>::Iterator& data)
	{
		if (data.mOwner != this)
		{
			throw std::runtime_error("This list does not own the passed iterator");
		}

		if (mSize == 0 || data == end())
		{
			return false;
		}

		if (data == begin())
		{
			PopFront();
			return true;
		}

		if (data.mNode == mBack)
		{
			PopBack();
		}
		else
		{
			Node* next = data.mNode->Next;
			data.mNode->Data.~T();
			new(&data.mNode->Data)T(std::move(next->Data));
			data.mNode->Next = next->Next;
			delete next;

			if (data.mNode->Next == nullptr)
			{
				mBack = data.mNode;
			}
			--mSize;

		}

		return true;
	}

#pragma endregion
}