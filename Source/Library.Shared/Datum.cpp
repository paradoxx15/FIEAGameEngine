#include "pch.h"
#include "Datum.h"

namespace Library
{
	const Datum::ToStringFunctors Datum::ToStringMethods[] = {
			&Datum::ToStringHelper<int>,
			&Datum::ToStringHelper<float>,
			&Datum::ToStringHelper<glm::vec4>,
			&Datum::ToStringHelper<glm::mat4>,
			&Datum::ToStringHelper<std::string>,
			&Datum::ToStringHelper<RTTIPointer>,
			&Datum::ToStringHelper<RTTIPointer>
	};

#pragma region ConstuctorsDestructor

	Datum::Datum(DatumTypes type, ReserveStrategy reserveStrategy) :
		mType(type), mReserveStrategy(reserveStrategy) {}

	Datum::Datum(ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy) {}

#pragma region DataConstructors

	Datum::Datum(int data, ReserveStrategy reserveStrategy) :
		mReserveStrategy(reserveStrategy)
	{
		ConstructorHelper({ data });
	}

	Datum::Datum(float data, ReserveStrategy reserveStrategy) :
		mReserveStrategy(reserveStrategy)
	{
		ConstructorHelper({ data });
	}

	Datum::Datum(const glm::vec4& data, ReserveStrategy reserveStrategy) :
		mReserveStrategy(reserveStrategy)
	{
		ConstructorHelper({ data });
	}

	Datum::Datum(const glm::mat4& data, ReserveStrategy reserveStrategy) :
		mReserveStrategy(reserveStrategy)
	{
		ConstructorHelper({ data });
	}

	Datum::Datum(const std::string& data, ReserveStrategy reserveStrategy) :
		mReserveStrategy(reserveStrategy)
	{
		ConstructorHelper({ data });
	}

	Datum::Datum(RTTIPointer data, ReserveStrategy reserveStrategy) :
		mReserveStrategy(reserveStrategy)
	{
		ConstructorHelper({ data });
	}

	Datum::Datum(std::initializer_list<int> list, ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy)
	{
		ConstructorHelper(list);
	}

	Datum::Datum(std::initializer_list<float> list, ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy)
	{
		ConstructorHelper(list);
	}

	Datum::Datum(std::initializer_list<glm::vec4> list, ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy)
	{
		ConstructorHelper(list);
	}

	Datum::Datum(std::initializer_list<glm::mat4> list, ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy)
	{
		ConstructorHelper(list);
	}

	Datum::Datum(std::initializer_list<std::string> list, ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy)
	{
		ConstructorHelper(list);
	}

	Datum::Datum(std::initializer_list<RTTIPointer> list, ReserveStrategy reserveStategy) :
		mReserveStrategy(reserveStategy)
	{
		ConstructorHelper(list);
	}

	Datum::Datum(const Datum& rhs) : 
		mType(rhs.mType), mIsExternal(rhs.mIsExternal), mReserveStrategy(rhs.mReserveStrategy)
	{
		CopyHelper(rhs);
	}

	Datum::Datum(Datum&& rhs) noexcept :
		mSize(rhs.mSize), mCapacity(rhs.mCapacity), mData(rhs.mData), mType(rhs.mType),
		mIsExternal(rhs.mIsExternal), mReserveStrategy(rhs.mReserveStrategy)
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mData.vo = nullptr;
	}

	Datum::~Datum()
	{
		Clear();

		if (!mIsExternal)
		{
			mCapacity = 0;
			free(mData.vo);
		}
	}


	void Datum::CopyHelper(const Datum& rhs)
	{
		if (!mIsExternal && rhs.mCapacity > 0)
		{
			Reserve(rhs.mCapacity);

			if (mType == DatumTypes::String)
			{
				for (size_t i = 0; i < rhs.mSize; ++i)
				{
					PushBack(rhs.mData.s[i]);
				}
			}
			else
			{
				mSize = rhs.mSize;
				std::memcpy(mData.b, rhs.mData.b, mSize * DataTypeSizes[static_cast<size_t>(mType)]);
			}
		}
		else
		{
			SetType(rhs.mType);
			mData.vo = rhs.mData.vo;
			mSize = rhs.mSize;
			mCapacity = rhs.mSize;
		}
	}
#pragma endregion 

#pragma endregion

#pragma region AssignmentOperators

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			if (!mIsExternal)
			{
				free(mData.vo);
			}

			SetType(rhs.mType);
			mIsExternal = rhs.mIsExternal;
			mReserveStrategy = rhs.mReserveStrategy;
			CopyHelper(rhs);
		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mData.vo = rhs.mData.vo;
			mIsExternal = rhs.mIsExternal;
			mReserveStrategy = rhs.mReserveStrategy;
			mType = rhs.mType;

			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mData.vo = nullptr;
		}

		return *this;
	}

	Datum& Datum::operator=(int data)
	{
		return AssignmentHelper({ data });
	}

	Datum& Datum::operator=(float data)
	{
		return AssignmentHelper({ data });
	}

	Datum& Datum::operator=(const glm::vec4& data)
	{
		return AssignmentHelper({ data });
	}

	Datum& Datum::operator=(const glm::mat4& data)
	{
		return AssignmentHelper({ data });
	}

	Datum& Datum::operator=(const std::string& data)
	{
		return AssignmentHelper({ data });
	}

	Datum& Datum::operator=(RTTIPointer data)
	{
		return AssignmentHelper({ data });
	}

	Datum& Datum::operator=(std::initializer_list<int> list)
	{
		return AssignmentHelper(list);
	}

	Datum& Datum::operator=(std::initializer_list<float> list)
	{
		return AssignmentHelper(list);
	}

	Datum& Datum::operator=(std::initializer_list<glm::vec4> list)
	{
		return AssignmentHelper(list);
	}

	Datum& Datum::operator=(std::initializer_list<glm::mat4> list)
	{
		return AssignmentHelper(list);
	}

	Datum& Datum::operator=(std::initializer_list<std::string> list)
	{
		return AssignmentHelper(list);
	}

	Datum& Datum::operator=(std::initializer_list<RTTIPointer> list)
	{
		return AssignmentHelper(list);
	}

#pragma endregion

#pragma region EqualityOperators

	bool Datum::operator==(const Datum& rhs) const
	{
		if (mSize != rhs.mSize || mType != rhs.Type())
		{
			return false;
		}

		if (mType == DatumTypes::Pointer || mType == DatumTypes::String || mType == DatumTypes::Table)
		{
			bool ret = true;
			for (size_t i = 0; i < mSize; ++i)
			{
				size_t size = i * DataTypeSizes[static_cast<size_t>(mType)];
				ret &= DataTypeEquality[static_cast<size_t>(mType) - 1](mData.b + size, rhs.mData.b + size);
			}
			return ret;
		}
		else
		{
			return std::memcmp(mData.b, rhs.mData.b, mSize * DataTypeSizes[static_cast<size_t>(mType)]) == 0;
		}

	}

	bool Datum::operator!=(const Datum& rhs) const
	{
		return !operator==(rhs);
	}

#pragma endregion

#pragma region Type

	Datum::DatumTypes Library::Datum::Type() const
	{
		return mType;
	}

	void Library::Datum::SetType(DatumTypes type)
	{
		if (mType == DatumTypes::Unknown || mType == type)
		{
			mType = type;
		}
		else
		{
			throw std::runtime_error("You cannot change type of datum after it has already been set");
		}
	}

#pragma endregion

#pragma region Capacity

	size_t Datum::Size() const
	{
		return mSize;
	}

	size_t Datum::Capacity() const
	{
		return mCapacity;
	}

	bool Datum::IsExternalStorage() const
	{
		return mIsExternal;
	}

	bool Datum::IsEmpty() const
	{
		return mSize == 0;
	}

	void Datum::Resize(size_t newSize)
	{
		if (mType == DatumTypes::Unknown || mIsExternal)
		{
			throw std::runtime_error("Invalid operation");
		}

		if (mSize > newSize && mType == DatumTypes::String)
		{
			for (size_t i = newSize; i < mSize; ++i)
			{
				mData.s[i].~basic_string();
			}
		}

		if (newSize > mCapacity)
		{
			Reserve(newSize);
		}

		if (mSize < newSize)
		{
			for (size_t i = mSize; i < newSize; i++)
			{
				DataTypeConstructors[static_cast<size_t>(mType) - 1](mData.b + (i * DataTypeSizes[static_cast<size_t>(mType)]));
			}
		}

		mSize = newSize;
	}

	void Datum::Reserve(size_t capacity)
	{
		if (mType == DatumTypes::Unknown || mIsExternal)
		{
			throw std::runtime_error("Invalid operation");
		}

		if (capacity > mCapacity)
		{
			void* newData = realloc(mData.vo, capacity * DataTypeSizes[static_cast<size_t>(mType)]);
			assert(newData != nullptr);
			mData.vo = newData;
			mCapacity = capacity;
		}
	}

#pragma endregion

#pragma region AccessData

#pragma region Get

	int& Datum::GetInt(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::Integer)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.i[index];
	}

	float& Datum::GetFloat(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::Float)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.f[index];
	}

	glm::vec4& Datum::GetVector(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::Vector)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.v[index];
	}

	glm::mat4& Datum::GetMatrix(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::Matrix)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.m[index];
	}

	std::string& Datum::GetString(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::String)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.s[index];
	}

	Datum::RTTIPointer& Datum::GetPointer(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::Pointer)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.p[index];
	}

	Datum::ScopePointer& Datum::GetScope(size_t index)
	{
		if (index >= mSize || mType != DatumTypes::Table)
		{
			throw std::runtime_error("Can't get data past the size of the datum");
		}

		return mData.t[index];
	}

	const int& Datum::GetInt(size_t index) const
	{
		return const_cast<Datum&>(*this).GetInt(index);
	}

	const float& Datum::GetFloat(size_t index) const
	{
		return const_cast<Datum&>(*this).GetFloat(index);
	}

	const glm::vec4& Datum::GetVector(size_t index) const
	{
		return const_cast<Datum&>(*this).GetVector(index);
	}

	const glm::mat4& Datum::GetMatrix(size_t index) const
	{
		return const_cast<Datum&>(*this).GetMatrix(index);
	}

	const std::string& Datum::GetString(size_t index) const
	{
		return const_cast<Datum&>(*this).GetString(index);
	}

	const Datum::RTTIPointer& Datum::GetPointer(size_t index) const
	{
		return const_cast<Datum&>(*this).GetPointer(index);
	}

	const Datum::ScopePointer& Datum::GetScope(size_t index) const
	{
		return const_cast<Datum&>(*this).GetScope(index);
	}

#pragma endregion

	std::string Datum::ToString(size_t index)
	{
		if (mType == DatumTypes::Unknown || index >= mSize)
		{
			throw std::runtime_error("Unable to create a string for this Datum");
		}

		return (this->*ToStringMethods[static_cast<size_t>(mType) - 1])(index);
	}

#pragma endregion

#pragma region RemoveData

	void Datum::SetFromString(const std::string& str, size_t index)
	{
		if (index >= mSize || mType == DatumTypes::Unknown || mType == DatumTypes::Pointer || mType == DatumTypes::Table)
		{
			throw std::runtime_error("Unable to SetFromString");
		}

		SetFromStringFunctions[static_cast<size_t>(mType) - 1](str, mData.vo, index);
	}

	void Datum::PopBack()
	{
		if (mIsExternal)
		{
			throw std::runtime_error("You can't remove external data");
		}

		if (!IsEmpty())
		{
			if (mType == DatumTypes::String)
			{
				mData.s[mSize - 1].~basic_string();
			}
			--mSize;
		}
	}

	bool Datum::RemoveAt(size_t index)
	{
		if (mIsExternal)
		{
			throw std::runtime_error("You can't remove external data");
		}

		if (mSize == 0 || index >= mSize)
		{
			return false;
		}

		if (mType == DatumTypes::String)
		{
			mData.s[index].~basic_string();
		}

		size_t size = DataTypeSizes[static_cast<size_t>(mType)];
		uint8_t* itemToDelete = mData.b + (index * size);
		std::memmove(itemToDelete, itemToDelete + size, size * (--mSize - index));
		return true;
	}

	void Datum::Clear()
	{
		if (mIsExternal)
		{
			mCapacity = 0;
			mData.vo = nullptr;
			mIsExternal = false;
		}
		else if (mType == DatumTypes::String)
		{
			for (size_t i = 0; i < mSize; ++i)
			{
				mData.s[i].~basic_string();
			}
		}

		mSize = 0;
	}

#pragma endregion
}



