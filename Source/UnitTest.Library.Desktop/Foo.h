#pragma once

#include "RTTI.h"

namespace UnitTests
{
	class Foo final : public Library::RTTI
	{
		RTTI_DECLARATIONS(Foo, Library::RTTI);

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		~Foo();

		bool operator==(const Foo& rhs) const noexcept;
		bool operator!=(const Foo& rhs) const noexcept;

		int& Data();
		int Data() const;
		void SetData(int data);

		std::string ToString() const override;
		bool Equals(const RTTI* rhs) const override;

	private:
		int* mData;
	};
}