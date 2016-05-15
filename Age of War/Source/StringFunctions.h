#ifndef StringFunctions_H_
#define StringFunctions_H_

#include <sstream>

namespace strFunc
{
	inline std::string getPrecisionString(float value, unsigned short precision)
	{
		std::stringstream stream;
		stream.precision(precision);
		stream << std::fixed << value;

		return std::string(stream.str());
	}
}
#endif