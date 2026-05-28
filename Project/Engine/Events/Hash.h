#ifndef HASH_H
#define HASH_H

#include <string>

namespace dae
{
	/*
	Type used to store string hashes
	*/
	using hash_t = uint32_t;

	/*
	Creates a hash from a c_string using the sdbm hash
	*/
	constexpr hash_t make_hash(const char* string, size_t length)
	{
		hash_t result{ 0 };

		for (size_t charIndex{ 0 }; charIndex < length; ++charIndex)
		{
			result = string[charIndex] + (result << 6) + (result << 16) - result;
		}

		return result;
	}

	/*
	Creates a hash from a string using the sdbm hash
	*/
	constexpr hash_t make_hash(const std::string& string)
	{
		return make_hash(string.c_str(), string.size());
	}

	namespace hash_literals
	{
		consteval hash_t operator""_h(const char* string, size_t length)
		{
			return make_hash(string, length);
		}
	}
}

#endif