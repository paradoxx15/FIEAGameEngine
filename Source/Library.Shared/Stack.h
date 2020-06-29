#pragma once

#include "Vector.h"

namespace Library
{
	template<typename T>
	class Stack
	{
	public:

		/// <summary>
		/// Constructor. Initializes the internal vector to a default capacity of 7 or a user defined capacity
		/// </summary>
		/// <param name="capacity">The capacity of the internal vector</param>
		Stack(size_t capacity = 7);
		/// <summary>
		/// Copy constructor. Copies the internal vector of the stack.
		/// </summary>
		/// <param name="rhs">The stack that will be copied</param>
		Stack(const Stack& rhs);
		/// <summary>
		/// Move constructor. Moves the internal vector of the stack.
		/// </summary>
		/// <param name="rhs">The stack being moved into this one</param>
		Stack(Stack&& rhs);
		/// <summary>
		/// Copy assignment operator. Copies the internal vector of the right hand side stack into this one.
		/// </summary>
		/// <param name="rhs">The stack that is being copied</param>
		/// <returns>This stack after being mutated</returns>
		Stack& operator=(const Stack& rhs);
		/// <summary>
		/// Move assignment operator. Moves the internal vector of the right hand side stack into this one.
		/// </summary>
		/// <param name="rhs">The stack that is being moved</param>
		/// <returns>This stack after being mutated</returns>
		Stack& operator=(const Stack&& rhs);
		/// <summary>
		/// Virtual destructor. Will destruct the internal vector
		/// </summary>
		virtual ~Stack() = default;
		
		/// <summary>
		/// Return the number of elements within the stack
		/// </summary>
		/// <returns>The number of elements within the stack</returns>
		size_t Size() const;
		/// <summary>
		/// Checks whether or not the stack is empty
		/// </summary>
		/// <returns>True if the stack is empty, false otherwise</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Pushes data onto the top of the stack.
		/// </summary>
		/// <param name="data">The data being pushed onto the stack</param>
		void Push(const T& data);

		/// <summary>
		/// Pushes r-value data onto the top of the stack.
		/// </summary>
		/// <param name="data">The data being pushed onto the stack</param>
		void Push(T&& data);

		/// <summary>
		/// Pops off the data on the top of the stack, reducing the size of the stack by 1
		/// </summary>
		/// <returns>The data removed from the top of the stack</returns>
		T& Pop();

		/// <summary>
		/// Gets a reference to the data at the top of the stack
		/// </summary>
		/// <returns>A reference to the data at the top of the stack</returns>
		T& Peek();

		/// <summary>
		/// Gets a reference to the data at the top of the stack
		/// </summary>
		/// <returns>A const reference to the data at the top of the stack</returns>
		const T& Peek() const;

	private:

		/// <summary>
		/// The internal vector used to store all the data of the stack
		/// </summary>
		Vector<T> mVector;
	};
}

#include "Stack.inl"