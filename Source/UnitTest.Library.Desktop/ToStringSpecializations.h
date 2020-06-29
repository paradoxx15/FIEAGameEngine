#pragma once

#include "CppUnitTest.h"
#include "SList.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "Scope.h"
#include "Attributed.h"
#include "AttributedBar.h"
#include "JsonParseMaster.h"
#include "Sector.h"
#include "World.h"

#pragma warning( push )
#pragma warning( disable : 4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning( pop )

namespace Microsoft::VisualStudio::CppUnitTestFramework
{

	using namespace UnitTests;
	using namespace Library;
	using namespace std::string_literals;

	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Foo>(const Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<const Foo, Foo>>(const std::pair<const Foo, Foo>& t)
	{
		try
		{
			return ToString(t.first);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(const HashMap<Foo, Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(const HashMap<Foo, Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>>(const HashMap<Foo, Foo>& t)
	{
		try
		{
			return ToString(t.Size());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

#pragma region Datum

	template<>
	inline std::wstring ToString<Datum&>(Datum& t)
	{
		RETURN_WIDE_STRING(t.Capacity());
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		RETURN_WIDE_STRING(t.Capacity());
	}

	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		std::string str = glm::to_string(t);
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		std::string str = glm::to_string(t);
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

#pragma endregion

#pragma region Scope

	template<>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Scope&>(Scope& t)
	{
		std::string str = t.ToString();
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope& t)
	{
		std::string str = t.ToString();
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

#pragma endregion

#pragma region Attributed

	template<>
	inline std::wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed>(const Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed&>(Attributed& t)
	{
		std::string str = t.ToString();
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	template<>
	inline std::wstring ToString<Attributed>(const Attributed& t)
	{
		std::string str = t.ToString();
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	template<>
	inline std::wstring ToString<AttributedBar&>(AttributedBar& t)
	{
		std::string str = t.ToString();
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	template<>
	inline std::wstring ToString<AttributedBar>(const AttributedBar& t)
	{
		std::string str = t.ToString();
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

#pragma endregion

#pragma region JsonParser

	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(const JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

#pragma endregion

#pragma region Sector
	template<>
	inline std::wstring ToString<Sector>(Sector* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Sector>(const Sector* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Sector

#pragma region World
	template<>
	inline std::wstring ToString<World>(World* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<World>(const World* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion World
}