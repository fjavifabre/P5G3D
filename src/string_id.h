#pragma once

#include <string>

/*
	string hashing to cheaper compartion
*/

#define SID(string) simple_hash(string)

inline unsigned int simple_hash(const std::string &str)
{
	unsigned int result = 0;

	for (auto& it : str)
		result = 37 * result + 17 * static_cast<char>(it);

	return result;
}

inline unsigned int simple_hash(const char* cStr)
{
	return simple_hash(std::string(cStr));
}
