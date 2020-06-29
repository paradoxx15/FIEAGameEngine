#pragma once

#include <functional>
#include <algorithm>
#include <utility>
#include <string>
#include <initializer_list>
#include <gsl/gsl>
#include <glm/glm.hpp>
#include <cstdint>
#include <RTTI.h>

#pragma warning( push )
#pragma warning( disable : 4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning( pop )

namespace Library
{
	class Scope;
	class Attributed;

	class Datum final
	{
		friend Scope;
		friend Attributed;

	public:
		using ReserveStrategy = std::function<size_t(const size_t size, const size_t capacity)>;
		using RTTIPointer = RTTI*;
		using ScopePointer = Scope*;

		enum class DatumTypes
		{
			Unknown,
			Integer,
			Float,
			Vector,
			Matrix,
			String,
			Pointer,
			Table,

			Size = Table,
			Begin = Unknown,
			End = Table + 1,
		};

#pragma region PublicMethods

		/// <summary>
		/// Returns the DatumTypes enum value that represents type T
		/// </summary>
		template<typename T>
		static constexpr DatumTypes TypeOf();

#pragma region ConstructorsDestructor

		/// <summary>
		/// Constructs a default Datum. Has the option to set the type in construction
		/// </summary>
		/// <param name="type">The DatumType the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(DatumTypes type = DatumTypes::Unknown, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Constructor for specifically only specifying the reserve strategy
		/// </summary>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(ReserveStrategy reserveStategy);

		/// <summary>
		/// Scalar constructor that sets the datum to be of type Integer with its first element being the passed in int
		/// </summary>
		/// <param name="data">The data the first element of the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(int data, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Scalar constructor that sets the datum to be of type Float with its first element being the passed in float
		/// </summary>
		/// <param name="data">The data the first element of the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(float data, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Scalar constructor that sets the datum to be of type Vector with its first element being the passed in glm::vec4
		/// </summary>
		/// <param name="data">The data the first element of the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(const glm::vec4& data, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Scalar constructor that sets the datum to be of type Matrix with its first element being the passed in glm::mat4
		/// </summary>
		/// <param name="data">The data the first element of the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(const glm::mat4& data, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Scalar constructor that sets the datum to be of type String with its first element being the passed in std::string
		/// </summary>
		/// <param name="data">The data the first element of the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(const std::string& data, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());
		/// <summary>
		/// Scalar constructor that sets the datum to be of type Pointer with its first element being the passed in RTTI*
		/// </summary>
		/// <param name="data">The data the first element of the Datum should be set to</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(RTTIPointer data, ReserveStrategy reserveStrategy = DefaultRerserveStrategy());

		/// <summary>
		/// Initializer constructor that sets the datum to be of type Integer and sets its data equal to the passed in list of ints
		/// </summary>
		/// <param name="list">The initializer_list of data that the datum should contain</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(std::initializer_list<int> list, ReserveStrategy reserveStategy = DefaultRerserveStrategy());
		/// <summary>
		/// Initializer constructor that sets the datum to be of type Float and sets its data equal to the passed in list of floats
		/// </summary>
		/// <param name="list">The initializer_list of data that the datum should contain</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(std::initializer_list<float> list, ReserveStrategy reserveStategy = DefaultRerserveStrategy());
		/// <summary>
		/// Initializer constructor that sets the datum to be of type Vector and sets its data equal to the passed in list of glm::vec4
		/// </summary>
		/// <param name="list">The initializer_list of data that the datum should contain</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(std::initializer_list<glm::vec4> list, ReserveStrategy reserveStategy = DefaultRerserveStrategy());
		/// <summary>
		/// Initializer constructor that sets the datum to be of type Matrix and sets its data equal to the passed in list of glm::mat4
		/// </summary>
		/// <param name="list">The initializer_list of data that the datum should contain</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(std::initializer_list<glm::mat4> list, ReserveStrategy reserveStategy = DefaultRerserveStrategy());
		/// <summary>
		/// Initializer constructor that sets the datum to be of type String and sets its data equal to the passed in list of std::strings
		/// </summary>
		/// <param name="list">The initializer_list of data that the datum should contain</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(std::initializer_list<std::string> list, ReserveStrategy reserveStategy = DefaultRerserveStrategy());
		/// <summary>
		/// Initializer constructor that sets the datum to be of type Pointer and sets its data equal to the passed in list of RTTI*
		/// </summary>
		/// <param name="list">The initializer_list of data that the datum should contain</param>
		/// <param name="reserveStrategy">The strategy used for reserving capacity</param>
		Datum(std::initializer_list<RTTIPointer> list, ReserveStrategy reserveStategy = DefaultRerserveStrategy());

		/// <summary>
		/// Copy constructor for Datum. Shallow copies unless of type String in which case it deep copies
		/// </summary>
		/// <param name="rhs">The Datum whose data should be copied into this Datum</param>
		Datum(const Datum& rhs);
		/// <summary>
		/// Move constructor for Datum
		/// </summary>
		/// <param name="rhs">The rvalue Datum whose data will be moved to this Datum</param>
		/// <returns></returns>
		Datum(Datum&& rhs) noexcept;
		/// <summary>
		/// Destructs the Datum freeing any allocated memory
		/// </summary>
		~Datum();

#pragma endregion

#pragma region AssigmentOperators

		/// <summary>
		/// Copy assignment operator. Shallow copies unless of type String in which case it deep copies
		/// </summary>
		/// <param name="rhs">The Datum whose data should be copied into this Datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(const Datum& rhs);
		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">The rvalue Datum whose data will be moved to this Datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(Datum&& rhs) noexcept;

		/// <summary>
		/// Scalar assignment operator for int. Sets the first element of datum to the passed in data
		/// </summary>
		/// <param name="data">The value the first element of this Datum should be set to</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(int data);
		/// <summary>
		/// Scalar assignment operator for float. Sets the first element of datum to the passed in data
		/// </summary>
		/// <param name="data">The value the first element of this Datum should be set to</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(float data);
		/// <summary>
		/// Scalar assignment operator for glm::vec4. Sets the first element of datum to the passed in data
		/// </summary>
		/// <param name="data">The value the first element of this Datum should be set to</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(const glm::vec4& data);
		/// <summary>
		/// Scalar assignment operator for glm::mat4. Sets the first element of datum to the passed in data
		/// </summary>
		/// <param name="data">The value the first element of this Datum should be set to</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(const glm::mat4& data);
		/// <summary>
		/// Scalar assignment operator for std::string. Sets the first element of datum to the passed in data
		/// </summary>
		/// <param name="data">The value the first element of this Datum should be set to</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(const std::string& data);
		/// <summary>
		/// Scalar assignment operator for RTTI*. Sets the first element of datum to the passed in data
		/// </summary>
		/// <param name="data">The value the first element of this Datum should be set to</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(RTTIPointer data);

		/// <summary>
		/// Scalar assignment operator for an int initializer list. Sets the first n elements of datum to the passed in lists data.
		/// n being the size of the list. Will reserve space if the datum has not already allocated it.
		/// </summary>
		/// <param name="list">The intializer list of data used to set the elements of the datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(std::initializer_list<int> list);
		/// <summary>
		/// Scalar assignment operator for a float initializer list. Sets the first n elements of datum to the passed in lists data.
		/// n being the size of the list. Will reserve space if the datum has not already allocated it.
		/// </summary>
		/// <param name="list">The intializer list of data used to set the elements of the datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(std::initializer_list<float> list);
		/// <summary>
		/// Scalar assignment operator for a glm::vec4 initializer list. Sets the first n elements of datum to the passed in lists data.
		/// n being the size of the list. Will reserve space if the datum has not already allocated it.
		/// </summary>
		/// <param name="list">The intializer list of data used to set the elements of the datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(std::initializer_list<glm::vec4> list);
		/// <summary>
		/// Scalar assignment operator for a glm::mat4 initializer list. Sets the first n elements of datum to the passed in lists data.
		/// n being the size of the list. Will reserve space if the datum has not already allocated it.
		/// </summary>
		/// <param name="list">The intializer list of data used to set the elements of the datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(std::initializer_list<glm::mat4> list);
		/// <summary>
		/// Scalar assignment operator for a std::string initializer list. Sets the first n elements of datum to the passed in lists data.
		/// n being the size of the list. Will reserve space if the datum has not already allocated it.
		/// </summary>
		/// <param name="list">The intializer list of data used to set the elements of the datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(std::initializer_list<std::string> list);
		/// <summary>
		/// Scalar assignment operator for a RTTI* initializer list. Sets the first n elements of datum to the passed in lists data.
		/// n being the size of the list. Will reserve space if the datum has not already allocated it.
		/// </summary>
		/// <param name="list">The intializer list of data used to set the elements of the datum</param>
		/// <returns>A reference to this Datum after its mutation</returns>
		Datum& operator=(std::initializer_list<RTTIPointer> list);

#pragma endregion

#pragma region EqualityOperators

		/// <summary>
		/// Compares this datum to another datum for logical equivalence. Two datums are the equal if they contain data of logical equivalence
		/// </summary>
		/// <param name="rhs">The datum this datum is being compared to</param>
		/// <returns>True if the datums are equivalent, false otherwise</returns>
		bool operator==(const Datum& rhs) const;
		/// <summary>
		/// Compares this datum to another datum for inequality. Two datums are not equal if their contained data is different
		/// </summary>
		/// <param name="rhs">The datum this datum is being compared to</param>
		/// <returns>True if the datums are not equivalent, false otherwise</returns>
		bool operator!=(const Datum& rhs) const;

		/// <summary>
		/// Scalar equality operator. Compares this datum to a scalar value. 
		/// Will not compile if you try to call this method with an unsupported data type.
		/// <param name="data">The data the first element of this datum is compared to</param>
		/// <returns>If the datum is of size 1 and its data is equivalent to the data passed in then this returns true</returns>
		template<typename T>
		bool operator==(const T& data) const;

		/// <summary>
		/// Scalar inequality operator. Compares this datum to a scalar value. 
		/// Will not compile if you try to call this method with an unsupported data type.
		/// <param name="data">The data the first element of this datum is compared to</param>
		/// <returns>If the datum is not of size 1 or its data is not equivalent to the data passed in then this returns true</returns>
		template<typename T>
		bool operator!=(const T& data) const;

#pragma endregion

#pragma region Type

		/// <summary>
		/// Returns the DatumTypes enum value that represents the type of data this datum contains
		/// </summary>
		/// <returns>DatumTypes value that this datum is set to</returns>
		DatumTypes Type() const;
		/// <summary>
		/// Sets the type of data this datum can hold
		/// </summary>
		/// <param name="type">The enum value of DatumTypes for this datums type to be set to</param>
		/// <exception cref="std::runtime_error">Throws an exception if the datums type has already been set</exception>
		void SetType(DatumTypes type);

#pragma endregion

#pragma region Capacity

		/// <summary>
		/// Returns a count of the number of elements contained within this datum
		/// </summary>
		/// <returns>The size of this datum</returns>
		size_t Size() const;
		/// <summary>
		/// Returns the number of elements this datum can contain
		/// </summary>
		/// <returns>The capacity of this datum</returns>
		size_t Capacity() const;
		/// <summary>
		/// Tells the user whether this datum contains internal storage or external storage
		/// </summary>
		/// <returns>True if the datum has external storage, false otherwise</returns>
		bool IsExternalStorage() const;

		/// <summary>
		/// Tells whether the datum contains any data or not
		/// </summary>
		/// <returns>True if the datum has no elements, false otherwise</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Resizes the datum to have a size and capacity of the passed in new size. Will add or remove elements if necessary.
		/// Added elements will be base constructed variables of the current type the datum is.
		/// </summary>
		/// <param name="newSize">The new size and capacity the datum should be</param>
		/// <exception cref="std::runtime_error">Throws an exception if the datum does not have a type or has external storage</exception>
		void Resize(size_t newSize);
		/// <summary>
		/// Reserves memory to hold the number of elements of the capacity passed in. 
		/// Does not modify the capacity if the passed in argument is smaller than the current capacity.
		/// </summary>
		/// <param name="capacity">The new capacity of elements the datum can contain</param>
		/// <exception cref="std::runtime_error">Throws an exception if the datum does not have a type or has external storage</exception>
		void Reserve(size_t capacity);

#pragma endregion

#pragma region AccessData
		/// <summary>
		/// Attempts to find the passed in data within the datum and return a pointer to it.
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="value">The value of the data to be searched for</param>
		/// <returns>A pointer to the found data. Will return nullptr is no such data is found</returns>
		template<typename T>
		T* Find(const T& value);

		/// <summary>
		/// Attempts to find the passed in data within the datum and return a pointer to a constant reference of the data.
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="value">The value of the data to be searched for</param>
		/// <returns>A pointer to a const version of the found data. Will return nullptr is no such data is found</returns>
		template<typename T>
		const T* Find(const T& value) const;

		/// <summary>
		/// Gets a reference to the integer contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the integer contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Integer</exception>
		int& GetInt(size_t index = 0);
		/// <summary>
		/// Gets a reference to the float contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the float contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Float</exception>
		float& GetFloat(size_t index = 0);
		/// <summary>
		/// Gets a reference to the glm::vec4 contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the glm::vec4 contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Vector</exception>
		glm::vec4& GetVector(size_t index = 0);
		/// <summary>
		/// Gets a reference to the glm::mat4 contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the glm::mat4 contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Matrix</exception>
		glm::mat4& GetMatrix(size_t index = 0);
		/// <summary>
		/// Gets a reference to the std::string contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the std::string contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type String</exception>
		std::string& GetString(size_t index = 0);
		/// <summary>
		/// Gets a reference to the RTTI* contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the RTTI* contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Pointer</exception>
		RTTIPointer& GetPointer(size_t index = 0);
		/// <summary>
		/// Gets a reference to the Scope* contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A reference to the Scope* contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Pointer</exception>
		ScopePointer& GetScope(size_t index = 0);

		/// <summary>
		/// Gets a const reference to the integer contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the integer contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Integer</exception>
		const int& GetInt(size_t index = 0) const;
		/// <summary>
		/// Gets a const reference to the float contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the float contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Float</exception>
		const float& GetFloat(size_t index = 0) const;
		/// <summary>
		/// Gets a const reference to the glm::vec4 contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the glm::vec4 contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Vector</exception>
		const glm::vec4& GetVector(size_t index = 0) const;
		/// <summary>
		/// Gets a const reference to the glm::mat4 contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the glm::mat4 contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Matrix</exception>
		const glm::mat4& GetMatrix(size_t index = 0) const;
		/// <summary>
		/// Gets a const reference to the std::string contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the std::string contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type String</exception>
		const std::string& GetString(size_t index = 0) const;
		/// <summary>
		/// Gets a const reference to the RTTI* contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the RTTI* contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Pointer</exception>
		const RTTIPointer& GetPointer(size_t index = 0) const;
		/// <summary>
		/// Gets a const reference to the Scope* contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to return a reference to</param>
		/// <returns>A const reference to the Scope* contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Pointer</exception>
		const ScopePointer& GetScope(size_t index = 0) const;

		/// <summary>
		/// Returns a string representation of the data contained at the passed in index
		/// </summary>
		/// <param name="index">Index of the data to be converted into a string representation</param>
		/// <returns>A string representation of the data at the passed in index</returns>
		std::string ToString(size_t index = 0);

#pragma endregion

#pragma region ModifyData
		/// <summary>
		/// Sets this datum to be an external storage datum that contains pointers to the passed in data.
		/// Will set the datum type if it is currently unknown.
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="data">The array of data this datum should hold</param>
		/// <param name="size">The size of the data being passed in</param>
		/// <exception cref="std::runtime_error">Throws an exception of the passed in data is a different type than what this datum can hold</exception>
		template<typename T>
		void SetStorage(T* data, size_t size);

		/// <summary>
		/// Sets the data and the passed in index to the new data being passed in
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="data">The new data to be set at the specified index</param>
		/// <param name="index">The index of where the new data is being set to</param>
		/// <exception cref="std::runtime_error">Throws an exception of the passed in data is a different type than what this datum can hold</exception>
		template<typename T>
		void Set(const T& data, size_t index);

		/// <summary>
		/// Pushes the user passed data into the datum at the back of the datums data. Will increase the capacity of the datum if it is already filled.
		/// Uses a default or user defined reserve strategy for increasing capacity
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="value">The value to be added to the datum</param>
		/// <exception cref="std::runtime_error">Throws an exception of the passed in data is a different type than what this datum can hold</exception>
		template<typename T>
		void PushBack(const T& value);

		/// <summary>
		/// Converts the string passed in into data of the current datums type and sets the data at the passed in index to that converted string data.
		/// </summary>
		/// <param name="str">The std::string representing the data to be created</param>
		/// <param name="index">The index of where the converted string data should be placed within the datum</param>
		/// <exception cref="std::runtime_error">Throws an exception if the passed in data is a different type than what this datum can hold, or if this datum is of type Pointer</exception>
		void SetFromString(const std::string& str, size_t index = 0);

#pragma endregion

#pragma region RemoveData
		/// <summary>
		/// Destructs the last element within the datum. Does nothing if the datum is empty
		/// </summary>
		/// <exception cref="std::runtime_error">Throws an exception of the datum has external storage</exception>
		void PopBack();

		/// <summary>
		/// Removes any element from the datum. Does not harm the datum if the passed in value is not contained within the datum
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="value">The value of the element to be removed from the datum</param>
		/// <returns>True if the element was removed, false otherwise</returns>
		/// <exception cref="std::runtime_error">Throws an exception of the datum has external storage</exception>
		template<typename T>
		bool Remove(const T& data);

		/// <summary>
		/// Removes an element from the datum at the specified index. Does not harm the datum if the index is greater than its size. Will just return false.
		/// Will not compile if you try to call this method with an unsupported data type.
		/// </summary>
		/// <param name="index">The index of the element to be removed from the datum</param>
		/// <returns>True if the element was removed, false otherwise</returns>
		/// <exception cref="std::runtime_error">Throws an exception of the datum has external storage</exception>
		bool RemoveAt(size_t index);

		/// <summary>
		/// If the datum is external storage, clear will nullify the data pointer and set capacity and size to 0 and set the datum to not be external anymore.
		/// If the datum is internal storage, clear will destruct any allocated memory and set size to 0. Will not affect capacity
		/// </summary>
		void Clear();

#pragma endregion

#pragma endregion
		
	private:

#pragma region PrivateSpecializations
		/// <summary>
		/// Private PushBack for adding new Scopes to the end of the Datum.
		/// </summary>
		/// <param name="value">The Scope reference being added to the Datum</param>
		template<> void PushBack(const Scope& value);

		/// <summary>
		/// Private Set for replacing the Scope pointer at a specified index within the Datum
		/// </summary>
		/// <param name="data">The Scope reference whose address is replacing the Scope pointer at the given index</param>
		/// <param name="index">The index where the passed in Scope reference is being set to</param>
		template<> void Set(const Scope& data, size_t index);

		/// <summary>
		/// Sets this datum to be an external storage datum that contains pointers to the passed in data.
		/// </summary>
		/// <param name="data">The array of data this datum should hold</param>
		/// <param name="size">The size of the data being passed in</param>
		template<> void SetStorage(void* data, size_t size);

#pragma endregion

#pragma region HelperMethods
		/// <summary>
		/// Helper method for templated methods to be able to call the correct get method
		/// </summary>
		/// <param name="index">The index of the data to be returned</param>
		/// <returns>A reference to the data contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Integer</exception>
		template<typename T>
		T& GetHelper(size_t index);

		/// <summary>
		/// Helper method for const templated methods to be able to call the correct get method
		/// </summary>
		/// <param name="index">The index of the data to be returned</param>
		/// <returns>A const reference to the data contained at the given index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the datums range, or if the datum is not of type Integer</exception>
		template<typename T>
		const T& GetHelper(size_t index) const;

		/// <summary>
		/// Helper function for all the different construction calls. 
		/// Sets the type of the datum, reserves necessary space for passed in data, and pushes the data into the datum
		/// </summary>
		/// <param name="list">The initializer list of data used to construct the initial data of the datum</param>
		template<typename T>
		void ConstructorHelper(std::initializer_list<T> list);

		/// <summary>
		/// Helper function for the different scalar and initializer_list assignment operators.
		/// Sets the datums type if not already set, resizes the datum if needed and it is not external, 
		/// and sets the first n elements of the datum equal to the data contained within the passed in list. n being the size of the list.
		/// </summary>
		/// <param name="list">The list of data used to accomplish the above described functionality</param>
		/// <returns>A reference to the datum after its been modified</returns>
		/// <exception cref="std::runtime_exception">
		/// Throws a runtime exception of the passed in list is a different type than what the datum is set to.
		/// Or if the datum is external storage and the passed in list is of greater size than the storages capacity
		/// </exception>
		template<typename T>
		Datum& AssignmentHelper(std::initializer_list<T> list);

		/// <summary>
		/// Helper function for copy semantics. Does a deep copy of the passed in datum into this datum.
		/// </summary>
		/// <param name="rhs">The datum to be copied</param>
		void CopyHelper(const Datum& rhs);

		/// <summary>
		/// Templated ToStringHelper that is called by the ToString method which returns a string representation of the data contained at the passed in index.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<typename T>
		std::string ToStringHelper(size_t index) const;

		/// <summary>
		/// ToStringHelper specialization for converting integers.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<> std::string ToStringHelper<int>(size_t index) const;
		/// <summary>
		/// ToStringHelper specialization for converting floats.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<> std::string ToStringHelper<float>(size_t index) const;
		/// <summary>
		/// ToStringHelper specialization for converting glm::vec4's.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<> std::string ToStringHelper<glm::vec4>(size_t index) const;
		/// <summary>
		/// ToStringHelper specialization for converting glm::mat4's.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<> std::string ToStringHelper<glm::mat4>(size_t index) const;
		/// <summary>
		/// ToStringHelper specialization for converting std::string.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<> std::string ToStringHelper<std::string>(size_t index) const;
		/// <summary>
		/// ToStringHelper specialization for converting RTTI*'s.
		/// </summary>
		/// <param name="index">The index of the data to convert to a string</param>
		/// <returns>std::string representation of the data contained at the passed in index</returns>
		template<> std::string ToStringHelper<RTTIPointer>(size_t index) const;
#pragma endregion

#pragma region LookUpTables
		/// <summary>
		/// A look up table for data sizes based upon what type a datum is set to
		/// </summary>
		inline static const size_t DataTypeSizes[static_cast<size_t>(DatumTypes::End)]{ 0, sizeof(int), sizeof(float), sizeof(glm::vec4), sizeof(glm::mat4), sizeof(std::string), sizeof(RTTIPointer), sizeof(Scope*) };

		/// <summary>
		/// A look up table for placement new construction calls depending on what type a datum is set to
		/// </summary>
		inline static const std::function<void(void* location)> DataTypeConstructors[static_cast<size_t>(DatumTypes::Size)]
		{
			[](void* location) { new(location) int(0); },
			[](void* location) { new(location) float(0.0f); },
			[](void* location) { new(location) glm::vec4(0.0f); },
			[](void* location) { new(location) glm::mat4(0.0f); },
			[](void* location) { new(location) std::string(); },
			[](void* location) { new(location) RTTIPointer(nullptr); },
			nullptr,
		};

		/// <summary>
		/// A look up table of equality functions based upon what type a datum is set to
		/// </summary>
		inline static const std::function<bool(const void* lhs, const void* rhs)> DataTypeEquality[static_cast<size_t>(DatumTypes::Size)]
		{
			[](const void* lhs, const void* rhs) { return std::memcmp(lhs, rhs, sizeof(int)) == 0; },
			[](const void* lhs, const void* rhs) { return std::memcmp(lhs, rhs, sizeof(float)) == 0; },
			[](const void* lhs, const void* rhs) { return std::memcmp(lhs, rhs, sizeof(glm::vec4)) == 0; },
			[](const void* lhs, const void* rhs) { return std::memcmp(lhs, rhs, sizeof(glm::mat4)) == 0; },
			[](const void* lhs, const void* rhs) { return *reinterpret_cast<const std::string*>(lhs) == *reinterpret_cast<const std::string*>(rhs); },
			[](const void* lhs, const void* rhs) { return (*reinterpret_cast<RTTI* const*>(lhs))->Equals(*reinterpret_cast<RTTI* const*>(rhs)); },
			[](const void* lhs, const void* rhs) { return (*reinterpret_cast<RTTI* const*>(lhs))->Equals(*reinterpret_cast<RTTI* const*>(rhs)); },
		};

		/// <summary>
		/// A look up table of string to data conversion functions based upon what type a datum is set to
		/// </summary>
		inline static const std::function<void(std::string str, void* data, size_t index)> SetFromStringFunctions[static_cast<size_t>(DatumTypes::Size)]
		{
			[](std::string str, void* data, size_t index) { reinterpret_cast<int*>(data)[index] = stoi(str); },
			[](std::string str, void* data, size_t index) { reinterpret_cast<float*>(data)[index] = stof(str); },
			[](std::string str, void* data, size_t index) { glm::vec4 newData; sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &newData[0], &newData[1], &newData[2], &newData[3]);
															reinterpret_cast<glm::vec4*>(data)[index] = newData; },
			[](std::string str, void* data, size_t index) { glm::mat4 newData;
															sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
																	 &newData[0][0], &newData[0][1], &newData[0][2], &newData[0][3], &newData[1][0], &newData[1][1],
																	 &newData[1][2], &newData[1][3], &newData[2][0], &newData[2][1], &newData[2][2], &newData[2][3],
																	 &newData[3][0], &newData[3][1], &newData[3][2], &newData[3][3]);
															reinterpret_cast<glm::mat4*>(data)[index] = newData; },
			[](std::string str, void* data, size_t index) { reinterpret_cast<std::string*>(data)[index] = str; },
			nullptr,
			nullptr
		};

		/// <summary>
		/// A look up table of ToString member methods based upon what type a datum is set to
		/// </summary>
		using ToStringFunctors = std::string(Datum::*)(size_t index) const;
		static const ToStringFunctors ToStringMethods[static_cast<size_t>(DatumTypes::Size)];

#pragma endregion

		/// <summary>
		/// The default strategy for how PushBack will reserve new memory if necessary
		/// </summary>
		struct DefaultRerserveStrategy
		{
			/// <summary>
			/// Returns a new capacity that is increased by a factor of 1.5
			/// </summary>
			/// <param>Empty param not used for the default reserve strategy</param>
			/// <param name="capacity">The current capacity of the vector</param>
			/// <returns>A size_t of the capacity multiplied by a factor of 1.5</returns>
			const size_t operator()(const size_t, const size_t capacity)
			{
				return static_cast<size_t>(capacity * 1.5);
			}
		};

		/// <summary>
		/// The union of pointers used to access data within the datum.
		/// </summary>
		union DatumValues
		{
			int* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			RTTIPointer* p;
			Scope** t;
			uint8_t* b;
			void* vo = nullptr;
		};

		/// <summary>
		/// The reserve strategy to be used when a method call needs to increase reserved memory
		/// </summary>
		ReserveStrategy mReserveStrategy{ DefaultRerserveStrategy() };
		/// <summary>
		/// The enum value of what type this datum currently is
		/// </summary>
		DatumTypes mType{ DatumTypes::Unknown };
		/// <summary>
		/// The union array of data contained within this datum
		/// </summary>
		DatumValues mData{ nullptr };
		/// <summary>
		/// A bool representing if this datum has external storage or internal storage
		/// </summary>
		bool mIsExternal{ false };
		/// <summary>
		/// The number of elements contained within this datum
		/// </summary>
		size_t mSize{ 0 };
		/// <summary>
		/// The number of elements able to be contained within this datum
		/// </summary>
		size_t mCapacity{ 0 };
	};
}

#include "Datum.inl"