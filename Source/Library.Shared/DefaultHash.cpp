#include "pch.h"
#include "DefaultHash.h"

namespace Library
{
	size_t AdditiveHash(const uint8_t* data, size_t size)
	{
		size_t hashValue = 0;
		for (size_t i = 0; i < size; i++)
		{
			hashValue += HashPrime * data[i];
		}

		return hashValue;
	}
}