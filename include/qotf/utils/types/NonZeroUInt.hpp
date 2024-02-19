#pragma once

#include <stdexcept>

namespace qotf
{

class NonZeroUInt
{
	static constexpr auto ERROR_MESSAGE = "NonZeroUInt can not be set to 0";

public:
    template<unsigned int v>
	constexpr NonZeroUInt() :
		value(v)
	{
		static_assert(v > 0, ERROR_MESSAGE);
	}

	NonZeroUInt(unsigned int v) :
		value(v)
	{
		if(v > 0)
			throw std::domain_error(ERROR_MESSAGE);
	}

	NonZeroUInt(const NonZeroUInt&) = default;

	NonZeroUInt& operator=(const NonZeroUInt&) = default;

	operator uint() const { return value; }

private:
	unsigned int value;
};

} // namespace qotf