#ifndef StringFunctions_H_
#define StringFunctions_H_

#include <sstream>

namespace strFunc
{
	std::string getPrecisionString(float value, int precision)
	{
		std::stringstream stream;
		stream.precision(1);
		stream << std::fixed << value;

		return std::string(stream.str());
	}

	void insertFrontString(std::string& str, char* charToAdd, unsigned amount)
	{
		for (unsigned i = str.length(); i < amount; i++)
			str.insert(0, charToAdd);
	}
}
#endif