#ifndef MORTON_CODE2_H
#define MORTON_CODE2_H

#include <cstdint>

namespace QOTF
{
typedef unsigned int uint;

// Morton code for quadtrees
namespace MortonCode2
{
	static inline uint64_t splitBy2(uint a)
	{
		// Keep the first 32 bits
		uint64_t x = a & 0xffffff;
		// 0000 0000 0000 0000 1111 1111 1111 1111 0000 0000 0000 0000 1111 1111 1111 1111
		x = (x | x << 16) & 0x0000ffff0000ffff;
		// 0000 0000 1111 1111 0000 0000 1111 1111 0000 0000 1111 1111 0000 0000 1111 1111
		x = (x | x << 8) & 0x00ff00ff00ff00ff;
		// 0000 1111 0000 1111 0000 1111 0000 1111 0000 1111 0000 1111 0000 1111 0000 1111
		x = (x | x << 4) & 0x0f0f0f0f0f0f0f0f;
		// 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011 0011
		x = (x | x << 2) & 0x3333333333333333;
		// 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101 0101
		x = (x | x << 1) & 0x5555555555555555;
		return x;
	}

	static inline uint64_t encode(uint x, uint y)
	{
		uint64_t code = 0;
		code |= splitBy2(x) << 1 | splitBy2(y);
		return code;
	}

	static inline uint decode(uint64_t morton_code, uint level)
	{
		return (morton_code >> (2 * level)) & 0b11;
	}
} // namespace MortonCode2

} // namespace QOTF

#endif
