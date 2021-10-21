#ifndef MORTON_CODE3_H
#define MORTON_CODE3_H

#include <cstdint>

namespace QOTF
{
typedef unsigned int uint;

// Morton code for octrees
namespace MortonCode3
{
	static inline uint64_t splitBy3(uint a)
	{
		// keep the first 21 bits
		uint64_t x = a & 0x1fffff;
		// 1111 1111 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111
		x = (x | x << 32) & 0xffff00000000ffff;
		// 0000 0000 1111 1111 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 1111 1111
		x = (x | x << 16) & 0xff0000ff0000ff;
		// 1111 0000 0000 1111 0000 0000 1111 0000 0000 1111 0000 0000 1111 0000 0000 1111
		x = (x | x << 8) & 0xf00f00f00f00f00f;
		// 0011 0000 1100 0011 0000 1100 0011 0000 1100 0011 0000 1100 0011 0000 1100 0011
		x = (x | x << 4) & 0x30c30c30c30c30c3;
		// 1001 0010 0100 1001 0010 0100 1001 0010 0100 1001 0010 0100 1001 0010 0100 1001
		x = (x | x << 2) & 0x9249249249249249;
		return x;
	}

	static inline uint64_t encode(uint x, uint y, uint z)
	{
		uint64_t code = 0;
		code |= splitBy3(x) << 2 | splitBy3(y) << 1 | splitBy3(z);
		return code;
	}

	static inline uint decode(uint64_t morton_code, uint level)
	{
		return (morton_code >> (3 * level)) & 0b111;
	}
} // namespace MortonCode3

} // namespace QOTF

#endif
