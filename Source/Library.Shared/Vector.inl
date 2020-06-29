#include "Vector.h"

namespace Library
{
#pragma region Iterator
	template<typename T>
	inline Vector<T>::Iterator::Iterator(size_t index, Vector& owner) : mIndex(index), mOwner(&owner) {}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr || mOwner->Size() == 0)
		{
			throw std::runtime_error("This iterator does not belong to a vector");
		}

		return (*mOwner)[mIndex];
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& it) const
	{
		return !(*this != it);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& it) const
	{
		return (mOwner != it.mOwner || mIndex != it.mIndex);
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr || mIndex >= mOwner->Size())
		{
			throw std::runtime_error("You cannot increment the end iterator");
		}

		++mIndex;
		return *this;
	}

	template<typename T> 
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (mOwner == nullptr || mIndex == 0)  
		{
			throw std::runtime_error("You cannot decrement the begin iterator");
		}

		--mIndex;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator it = *this;
		operator--();
		return it;
	}
#pragma endregion

#pragma region ConstIterator
	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(size_t index, const Vector& owner) : mIndex(index), mOwner(&owner) {}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& it) : mIndex(it.mIndex), mOwner(it.mOwner) {}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr || mOwner->mSize == 0)
		{
			throw std::runtime_error("This iterator does not belong to a vector");
		}

		return (*mOwner)[mIndex];
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& it) const
	{
		return !(*this != it);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& it) const
	{
		return (mOwner != it.mOwner || mIndex != it.mIndex);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr || mIndex >= mOwner->Size())
		{
			throw std::runtime_error("You cannot increment the end iterator");
		}

		++mIndex;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		operator++();
		return it;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (mOwner == nullptr || mIndex == 0)
		{
			throw std::runtime_error("You cannot increment the begin iterator");
		}

		--mIndex;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator it = *this;
		operator--();
		return it;
	}
#pragma endregion

#pragma region Vector

#pragma region MemberMethods
	template<typename T>
	inline Vector<T>::Vector(size_t capacity, ReserveStrategy reserveStrategy) : mData(nullptr), mSize(0), mReserveStrategy(reserveStrategy)
	{
		Reserve(capacity);
	}

	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<T> list)
	{
		Reserve(list.size());

		for (const T& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& rhs)
	{
		Reserve(rhs.mCapacity);

		for (size_t i = 0; i < rhs.mSize; ++i)
		{
			PushBack(rhs[i]);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& rhs) noexcept : mSize(rhs.mSize), mCapacity(rhs.mCapacity), mData(rhs.mData)
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mData = nullptr;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		if (mData != nullptr)
		{
			Clear();
			free(mData);
		}
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Reserve(rhs.mCapacity);
			for (size_t i = 0; i < rhs.mSize; ++i)
			{
				PushBack(rhs[i]);
			}
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			free(mData);
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mData = rhs.mData;

			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mData = nullptr;
		}

		return *this;
	}
#pragma endregion

#pragma region ElementAccess
	template<typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		if (mData == nullptr || index >= mSize)
		{
			throw std::runtime_error("Index is outside range of vector");
		}

		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		if (mData == nullptr || index >= mSize)
		{
			throw std::runtime_error("Index is outside range of vector");
		}

		return mData[index];
	}

	template<typename T>
	inline T& Vector<T>::At(size_t index)
	{
		if (mData == nullptr || index >= mSize)
		{
			throw std::runtime_error("Index is outside range of vector");
		}

		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		if (mData == nullptr || index >= mSize)
		{
			throw std::runtime_error("Index is outside range of vector");
		}

		return mData[index];
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty");
		}

		return mData[0];
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty");
		}

		return mData[0];
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty");
		}

		return mData[mSize - 1];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("Vector is empty");
		}

		return mData[mSize - 1];
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value, std::function<bool(const T & lhs, const T & rhs)> EqualityCompare)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (EqualityCompare(mData[i], value))
			{
				return Iterator(i, *this);
			}
		}

		return end();
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value, std::function<bool(const T & lhs, const T & rhs)> EqualityCompare) const
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (EqualityCompare(mData[i], value))
			{
				return ConstIterator(i, *this);
			}
		}

		return cend();
	}
#pragma endregion
	
#pragma region Capacity
	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity > mCapacity)
		{
			mCapacity = capacity;
			T* newData = reinterpret_cast<T*>(realloc(mData, capacity * sizeof(T)));

			if (newData == nullptr)
			{
				throw std::runtime_error("Reserve memory allocation failed");
			}

			mData = newData;
		}
	}

	template<typename T>
	void Vector<T>::Resize(size_t newSize)
	{
		if (mSize > newSize)
		{
			for (size_t i = newSize; i < mSize; ++i)
			{
				mData[i].~T();
			}
		}

		if (newSize > mCapacity)
		{
			mCapacity = newSize;
			T* newData = reinterpret_cast<T*>(realloc(mData, newSize * sizeof(T)));
			if (newData == nullptr)
			{
				throw std::runtime_error("Reserve memory allocation failed");
			}
			mData = newData;
		}

		if (mSize < newSize)
		{
			for (size_t i = mSize; i < newSize; i++)
			{
				new(mData + i) T();
			}
		}

		mSize = newSize;
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (mCapacity > mSize)
		{
			if (mSize == 0)
			{
				free(mData);
				mData = nullptr;
			}
			else
			{
				Resize(mSize);
			}

			mCapacity = mSize;
		}
	}

#pragma endregion

#pragma region Modifiers
	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			mData[--mSize].~T();
		}
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		if (mSize >= mCapacity)
		{
			Reserve(std::max(mCapacity + 1, mReserveStrategy(mSize, mCapacity)));
		}

		new(mData + mSize) T(value);

		return Iterator(mSize++, *this);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::PushBack(T&& value)
	{
		if (mSize >= mCapacity)
		{
			Reserve(std::max(mCapacity + 1, mReserveStrategy(mSize, mCapacity)));
		}

		new(mData + mSize) T(std::move(value));

		return Iterator(mSize++, *this);
	}

	template<typename T>
	inline bool Vector<T>::Remove(const T& value)
	{
		return Remove(Find(value));
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& value)
	{
		if (value.mOwner != this)
		{
			throw std::runtime_error("This list does not own the passed iterator");
		}

		if (value.mIndex >= mSize)
		{
			return false;
		}

		mData[value.mIndex].~T();
		std::memmove(mData + value.mIndex, mData + value.mIndex + 1, sizeof(T) * (--mSize - value.mIndex));
		return true;
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& startIt, const Iterator& endIt)
	{
		if (startIt.mOwner != this || endIt.mOwner != this)
		{
			throw std::runtime_error("This list does not own the passed iterator");
		}

		if (startIt.mIndex >= mSize)
		{
			return false;
		}

		size_t numElements = mSize;

		for (size_t i = startIt.mIndex; i < endIt.mIndex; ++i)
		{
			mData[i].~T();
			--mSize;
		}

		if (endIt.mIndex < numElements)
		{
			std::memmove(mData + startIt.mIndex, mData + endIt.mIndex, sizeof(T) * (numElements - endIt.mIndex));
		}
		
		return true;
	}

	template<typename T>
	inline void Vector<T>::Clear()
	{
		for (size_t i = 0; i < mSize; i++)
		{
			mData[i].~T();
		}
		mSize = 0;
	}
#pragma endregion
	
#pragma region IteratorMethods
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(0, *this);
	}

	template<typename T>
	inline typename Vector<T>:: ConstIterator Vector<T>::begin() const
	{
		return cbegin();
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(0, *this);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(mSize, *this);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return cend();
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(mSize, *this);
	}
#pragma endregion
	
#pragma endregion
}