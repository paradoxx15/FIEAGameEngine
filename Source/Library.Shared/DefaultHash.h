#pragma once

#include <string>
#include <cstdint>

namespace Library
{
	const size_t HashPrime = 31;

	size_t AdditiveHash(const uint8_t* data, size_t size);

	template<typename T>
	struct DefaultHash final
	{
		size_t operator()(const T& key) const;
	};

	template<>
	struct DefaultHash<std::string> final
	{
		size_t operator()(const std::string& key) const;
	};

	template<>
	struct DefaultHash<const std::string> final
	{
		size_t operator()(const std::string& key) const;
	};

	template<>
	struct DefaultHash<std::wstring> final
	{
		size_t operator()(const std::wstring& key) const;
	};

	template<>
	struct DefaultHash<const std::wstring> final
	{
		size_t operator()(const std::wstring& key) const;
	};

	template<>
	struct DefaultHash<char*> final
	{
		size_t operator()(const char* key) const;
	};

	template<>
	struct DefaultHash<char* const> final
	{
		size_t operator()(const char* const key) const;
	};

	template<>
	struct DefaultHash<const char*> final
	{
		size_t operator()(const char* key) const;
	};

	template<>
	struct DefaultHash<const char* const> final
	{
		size_t operator()(const char* const key) const;
	};

}

#include "DefaultHash.inl"