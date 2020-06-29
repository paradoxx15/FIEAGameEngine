#include "pch.h"
#include "AttributedBar.h"

using namespace Library;
using namespace std::string_literals;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedBar);

	const Library::Vector<Library::Signature> AttributedBar::GetSignatures()
	{
		return Library::Vector<Library::Signature>
		{
			{"ChildData"s, Datum::DatumTypes::Integer, true, 1, offsetof(AttributedBar, mChildData)},
			{"Powers"s, Datum::DatumTypes::Table, false, 1, offsetof(AttributedBar, mPowers)}
		};
	}

	AttributedBar::AttributedBar(int data) :
		AttributedFoo(this->TypeIdClass()), mChildData(data)
	{
		mExternalInt = data;
		mPowers = new Scope;
		mPowers->Append("Flying"s);
		Adopt(*mPowers, "Powers");
	}

	AttributedBar::AttributedBar(const AttributedBar& rhs) :
		AttributedFoo(rhs), mChildData(rhs.mChildData), mPowers(Find("Powers")->GetScope(0))
	{
	}

	AttributedBar::AttributedBar(AttributedBar&& rhs) noexcept :
		AttributedFoo(std::move(rhs)), mChildData(rhs.mChildData), mPowers(Find("Powers")->GetScope(0))
	{
	}

	AttributedBar& AttributedBar::operator=(const AttributedBar& rhs)
	{
		AttributedFoo::operator=(rhs);

		if (this != &rhs)
		{
			mChildData = rhs.mChildData;
			mPowers = Find("Powers")->GetScope(0);
		}

		return *this;
	}

	AttributedBar& AttributedBar::operator=(AttributedBar&& rhs) noexcept
	{
		AttributedFoo::operator=(std::move(rhs));

		if (this != &rhs)
		{
			mChildData = rhs.mChildData;
			mPowers = Find("Powers")->GetScope(0);
		}

		return *this;
	}

	Scope*& AttributedBar::Powers()
	{
		return mPowers;
	}

	gsl::owner<Scope*> AttributedBar::Clone() const
	{
		return new AttributedBar(*this);
	}

	std::string AttributedBar::ToString() const
	{
		return "AttributedBar";
	}

	bool AttributedBar::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}

		return mExternalInt == static_cast<const AttributedBar*>(rhs)->Data() && mPowers == static_cast<const AttributedBar*>(rhs)->mPowers;
	}
}