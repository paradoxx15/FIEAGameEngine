#include "pch.h"
#include "AttributedFoo.h"

using namespace Library;
using namespace std::string_literals;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	const Library::Vector<Library::Signature> AttributedFoo::GetSignatures()
	{
		return Library::Vector<Library::Signature> 
			   {
					{"Data"s, Datum::DatumTypes::Integer, true, 1, offsetof(AttributedFoo, mExternalInt)},
					{"ExternalFloat"s, Datum::DatumTypes::Float, true, 1, offsetof(AttributedFoo, mExternalFloat)},
					{"ExternalVec"s, Datum::DatumTypes::Vector, true, 1, offsetof(AttributedFoo, mExternalVec)},
					{"ExternalMat"s, Datum::DatumTypes::Matrix, true, 1, offsetof(AttributedFoo, mExternalMat)},
					{"ExternalString"s, Datum::DatumTypes::String, true, 1, offsetof(AttributedFoo, mExternalString)},
					{"ExternalRTTI"s, Datum::DatumTypes::Pointer, true, 1, offsetof(AttributedFoo, mExternalRTTI)},
					{"Array"s, Datum::DatumTypes::Float, true, 6, offsetof(AttributedFoo, mArray)}
		       };
	}

	AttributedFoo::AttributedFoo(int data) :
		Attributed(this->TypeIdClass()), mExternalInt(data)
	{
	}

	AttributedFoo::AttributedFoo(size_t typeId) :
		Attributed(typeId), mExternalInt(0)
	{
	}

	int& AttributedFoo::Data()
	{
		return mExternalInt;
	}

	int AttributedFoo::Data() const
	{
		return mExternalInt;
	}

	gsl::owner<Scope*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo";
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}

		return *this == *static_cast<const AttributedFoo*>(rhs);
	}
}