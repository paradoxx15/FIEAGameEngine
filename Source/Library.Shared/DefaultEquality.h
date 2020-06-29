#pragma once

#include <string>

namespace Library
{
	template <typename T>
	struct DefaultEquality final
	{
		bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs == rhs;
		}
	};

	template<>
	struct DefaultEquality<char*> final
	{
		bool operator()(const char* lhs, const char* rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<char* const> final
	{
		bool operator()(const char* const lhs, const char* const rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char*> final
	{
		bool operator()(const char* lhs, const char* rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char* const> final
	{
		bool operator()(const char* const lhs, const char* const rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};
}