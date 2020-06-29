#pragma once

#include "Attributed.h"
#include "AttributedFoo.h"
#include "Vector.h"
#include "Factory.h"

namespace UnitTests
{
	class AttributedBar : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, AttributedFoo);

	public:
		static const Library::Vector<Library::Signature> GetSignatures();

		explicit AttributedBar(int data = 0);
		AttributedBar(const AttributedBar& rhs);
		AttributedBar(AttributedBar&& rhs) noexcept;
		AttributedBar& operator=(const AttributedBar& rhs);
		AttributedBar& operator=(AttributedBar&& rhs) noexcept;
		~AttributedBar() = default;

		Scope*& Powers();

		gsl::owner<Scope*> Clone() const override;

		std::string ToString() const override;
		bool Equals(const RTTI* rhs) const override;

	private:
		int mChildData;
		Scope* mPowers;
	};
}

using namespace UnitTests;
namespace Library
{
	ConcreteFactory(AttributedBar, Scope);
}
