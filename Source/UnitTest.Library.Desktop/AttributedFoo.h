#pragma once

#include "Attributed.h"
#include "Vector.h"
#include "Foo.h"
#include "Factory.h"

namespace UnitTests
{
	class AttributedFoo : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Library::Attributed);

	public:
		static const Library::Vector<Library::Signature> GetSignatures();

		explicit AttributedFoo(int data = 0);
		AttributedFoo(const AttributedFoo& rhs) = default;
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;
		~AttributedFoo() = default;

		int& Data();
		int Data() const;

		gsl::owner<Scope*> Clone() const override;

		std::string ToString() const override;
		bool Equals(const RTTI* rhs) const override;

		int mExternalInt = 0;
		float mExternalFloat = 1.1f;
		glm::vec4 mExternalVec = glm::vec4(1);
		glm::mat4 mExternalMat = glm::mat4(2);
		std::string mExternalString = "Hello";
		RTTI* mExternalRTTI = &foo;
		float mArray[6] = { 1, 2, 3, 4, 5, 6 };

	protected:
		explicit AttributedFoo(size_t typeId);

		Foo foo;
	};
}

using namespace UnitTests;
namespace Library
{
	ConcreteFactory(AttributedFoo, Scope);
}

