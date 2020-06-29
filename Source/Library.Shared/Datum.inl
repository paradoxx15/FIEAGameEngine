#include "Datum.h"

namespace Library
{
#pragma region TemplatedMethods

	template<typename T>
	inline bool Datum::operator==(const T& data) const
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown
			          || TypeOf<T>() != DatumTypes::Size
					  || TypeOf<T>() != DatumTypes::Begin
					  || TypeOf<T>() != DatumTypes::End, 
					  "Cannot preform equality operation on Unknown type");

		return mSize == 1 && mType == TypeOf<T>() && DataTypeEquality[static_cast<size_t>(mType) - 1](&GetHelper<T>(0), &data);
	}

	template<typename T>
	inline bool Datum::operator!=(const T& data) const
	{
		return !operator==<T>(data);
	}

	template<typename T>
	inline T* Datum::Find(const T& value)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown
			|| TypeOf<T>() != DatumTypes::Size
			|| TypeOf<T>() != DatumTypes::Begin
			|| TypeOf<T>() != DatumTypes::End,
			"Cannot find data of an unsupported Datum type");

		for (size_t i = 0; i < mSize; ++i)
		{
			T& data = reinterpret_cast<T*>(mData.vo)[i];
			if (DataTypeEquality[static_cast<size_t>(mType) - 1](&data, &(value)))
			{
				return &data;
			}
		}

		return nullptr;
	}

	template<typename T>
	inline const T* Datum::Find(const T& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	template<typename T>
	inline void Datum::SetStorage(T* data, size_t size)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown
			|| TypeOf<T>() != DatumTypes::Size
			|| TypeOf<T>() != DatumTypes::Begin
			|| TypeOf<T>() != DatumTypes::End,
			"Cannot set storage with unsupported Datum type");

		SetType(TypeOf<T>());
		mIsExternal = true;
		mData.vo = data;
		mSize = size;
		mCapacity = size;
	}

	template<typename T>
	inline void Datum::Set(const T& data, size_t index)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown
			|| TypeOf<T>() != DatumTypes::Size
			|| TypeOf<T>() != DatumTypes::Begin
			|| TypeOf<T>() != DatumTypes::End
			|| TypeOf<T>() != DatumTypes::Table,
			"Cannot preform Set on Unknown type");

		if (TypeOf<T>() != mType || index >= mSize)
		{
			throw std::runtime_error("Can't set data of a different type than the Datum. Or index is out of range");
		}

		T& dataToSet = GetHelper<T>(index);
		dataToSet = data;
	}

	template<typename T>
	void Datum::PushBack(const T& value)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown
			|| TypeOf<T>() != DatumTypes::Size
			|| TypeOf<T>() != DatumTypes::Begin
			|| TypeOf<T>() != DatumTypes::End
			|| TypeOf<T>() != DatumTypes::Table,
			"Cannot preform PushBack on unsupported DatumTypes");

		if (mIsExternal)
		{
			throw std::runtime_error("You can't modify external storage");
		}

		if (mType == DatumTypes::Unknown)
		{
			SetType(TypeOf<T>());
		}

		if (TypeOf<T>() != mType)
		{
			throw std::runtime_error("You can't push data that is not of the same type as this datum");
		}

		if (mSize >= mCapacity)
		{
			Reserve(std::max(mCapacity + 1, mReserveStrategy(mSize, mCapacity)));
		}

		new(reinterpret_cast<T*>(mData.vo) + mSize++) T(value);
	}

	template<typename T>
	inline bool Datum::Remove(const T& data)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Unsupported datum type");

		if (mIsExternal)
		{
			throw std::runtime_error("You can't remove external data");
		}

		if (mSize == 0)
		{
			return false;
		}

		T* itemToDelete = Find(data);
		if (itemToDelete == nullptr)
		{
			return false;
		}

		if (mType == DatumTypes::String)
		{
			itemToDelete->~T();
		}
		std::memmove(itemToDelete, itemToDelete + 1, DataTypeSizes[static_cast<size_t>(mType)] * ((reinterpret_cast<T*>(mData.vo) + --mSize) - itemToDelete));
		return true;
	}

#pragma endregion

#pragma region PrivateSpecializations

	template<>
	inline void Datum::PushBack(const Scope& value)
	{
		if (mIsExternal)
		{
			throw std::runtime_error("You can't modify external storage");
		}

		if (mType == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Table);
		}

		if (mSize >= mCapacity)
		{
			Reserve(std::max(mCapacity + 1, mReserveStrategy(mSize, mCapacity)));
		}

		new(mData.t + mSize++) Scope*(&const_cast<Scope&>(value));
	}

	template<>
	inline void Datum::Set(const Scope& data, size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index is out of range");
		}

		mData.t[index] = &const_cast<Scope&>(data);
	}

	template<>
	inline void Datum::SetStorage(void* data, size_t size)
	{
		mIsExternal = true;
		mData.vo = data;
		mSize = size;
		mCapacity = size;
	}

#pragma endregion


#pragma region HelperMethods

#pragma region GetHelper
	template<>
	inline int& Datum::GetHelper<int>(size_t index) { return GetInt(index); }

	template<>
	inline float& Datum::GetHelper<float>(size_t index) { return GetFloat(index); }

	template<>
	inline glm::vec4& Datum::GetHelper<glm::vec4>(size_t index) { return GetVector(index); }

	template<>
	inline glm::mat4& Datum::GetHelper<glm::mat4>(size_t index) { return GetMatrix(index); }

	template<>
	inline std::string& Datum::GetHelper<std::string>(size_t index) { return GetString(index); }

	template<>
	inline Datum::RTTIPointer& Datum::GetHelper<Datum::RTTIPointer>(size_t index) { return GetPointer(index); }

	template<>
	inline Datum::ScopePointer& Datum::GetHelper<Datum::ScopePointer>(size_t index) { return GetScope(index); }

	template<>
	inline const int& Datum::GetHelper<int>(size_t index) const { return GetInt(index); }

	template<>
	inline const float& Datum::GetHelper<float>(size_t index) const { return GetFloat(index); }

	template<>
	inline const glm::vec4& Datum::GetHelper<glm::vec4>(size_t index) const { return GetVector(index); }

	template<>
	inline const glm::mat4& Datum::GetHelper<glm::mat4>(size_t index) const { return GetMatrix(index); }

	template<>
	inline const std::string& Datum::GetHelper<std::string>(size_t index) const { return GetString(index); }

	template<>
	inline const Datum::RTTIPointer& Datum::GetHelper<Datum::RTTIPointer>(size_t index) const { return GetPointer(index); }

	template<>
	inline const Datum::ScopePointer& Datum::GetHelper<Datum::ScopePointer>(size_t index) const { return GetScope(index); }

#pragma endregion

	template<typename T>
	inline void Datum::ConstructorHelper(std::initializer_list<T> list)
	{
		mType = TypeOf<T>();
		Reserve(list.size());

		for (const T& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline Datum& Datum::AssignmentHelper(std::initializer_list<T> list)
	{
		if (list.size() > 1 || *this != *list.begin())
		{
			SetType(TypeOf<T>());

			if (mIsExternal && list.size() > mCapacity)
			{
				throw std::runtime_error("You can't set the elements of external storage past its capacity");
			}
			else if (!mIsExternal)
			{
				Resize(list.size());
			}

			size_t count = 0;
			for (const T& value : list)
			{
				Set(value, count++);
			}
		}

		return *this;
	}

#pragma region ToStringHelper

	template<typename T> inline std::string Datum::ToStringHelper(size_t index) const { return std::string(); }

	template<> inline std::string Datum::ToStringHelper<int>(size_t index) const { return std::to_string(GetInt(index)); }

	template<> inline std::string Datum::ToStringHelper<float>(size_t index) const { return std::to_string(GetFloat(index)); }

	template<> inline std::string Datum::ToStringHelper<glm::vec4>(size_t index) const { return glm::to_string(GetVector(index)); }

	template<> inline std::string Datum::ToStringHelper<glm::mat4>(size_t index) const { return glm::to_string(GetMatrix(index)); }

	template<> inline std::string Datum::ToStringHelper<std::string>(size_t index) const { return GetString(index); }

	template<> inline std::string Datum::ToStringHelper<Datum::RTTIPointer>(size_t index) const { return GetPointer(index)->ToString(); }

#pragma endregion

	template<typename T> inline static constexpr Datum::DatumTypes Datum::TypeOf() { return DatumTypes::Unknown; }

	template<> inline static constexpr Datum::DatumTypes Datum::TypeOf<int>() { return DatumTypes::Integer; }

	template<> inline static constexpr Datum::DatumTypes Datum::TypeOf<float>() { return DatumTypes::Float;  }

	template<> inline static constexpr Datum::DatumTypes Datum::TypeOf<glm::vec4>() { return DatumTypes::Vector; }

	template<> inline static constexpr  Datum::DatumTypes Datum::TypeOf<glm::mat4>() { return DatumTypes::Matrix; }

	template<> inline static constexpr  Datum::DatumTypes Datum::TypeOf<std::string>() { return DatumTypes::String; }

	template<> inline static constexpr  Datum::DatumTypes Datum::TypeOf<RTTI*>() { return DatumTypes::Pointer; }

	template<> inline static constexpr  Datum::DatumTypes Datum::TypeOf<Scope*>() { return DatumTypes::Table; }

#pragma endregion
}