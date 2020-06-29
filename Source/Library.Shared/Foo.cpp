#include "pch.h"
#include "Foo.h"

namespace Library
{
	Foo::Foo(int data) :
		mData(data)
	{
	}

	bool Foo::operator==(const Foo& foo)
	{
		return mData == foo.mData;
	}

	int& Foo::Data()
	{
		return mData;
	}

	int Foo::Data() const
	{
		return mData;
	}
}