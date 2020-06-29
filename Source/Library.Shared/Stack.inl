#pragma once

#include "Stack.h"

namespace Library
{
	template<typename T>
	inline Stack<T>::Stack(size_t capacity) : mVector(capacity) {}

	template<typename T>
	inline Stack<T>::Stack(const Stack& rhs) : mVector(rhs.mVector) {}

	template<typename T>
	inline Stack<T>::Stack(Stack&& rhs) : mVector(std::move(rhs.mVector)) {}

	template<typename T>
	inline Stack<T>& Stack<T>::operator=(const Stack& rhs)
	{
		mVector(rhs.mVector);
	}

	template<typename T>
	inline Stack<T>& Stack<T>::operator=(const Stack&& rhs)
	{
		mVector(std::move(mVector));
	}

	template<typename T>
	inline size_t Stack<T>::Size() const
	{
		return mVector.Size();
	}

	template<typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return mVector.IsEmpty();
	}

	template<typename T>
	inline void Stack<T>::Push(const T& data)
	{
		mVector.PushBack(data);
	}

	template<typename T>
	inline void Stack<T>::Push(T&& data)
	{
		mVector.PushBack(std::move(data));
	}

	template<typename T>
	inline T& Stack<T>::Pop()
	{
		T& ret = mVector.Back();
		mVector.PopBack();
		return ret;
	}

	template<typename T>
	inline T& Stack<T>::Peek()
	{
		return mVector.Back();
	}

	template<typename T>
	inline const T& Stack<T>::Peek() const
	{
		return mVector.Back();
	}


}
