#ifndef MORTON_CODE_H
#define MORTON_CODE_H

#include <cstdarg>
#include <cstdint>

namespace QOTF
{
typedef unsigned int uint;

template<uint D>
class MortonCode
{
	static_assert(D >= 2);

public:
	using Point = uint[D];

	static uint64_t split(uint n);
	static uint64_t encode(const Point&);
	static uint		decode(uint64_t c, uint level);
};

} // namespace QOTF

#include "morton_code.tpp"

#endif
