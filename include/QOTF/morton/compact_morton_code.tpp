#pragma once

#include "compact_morton_code.h"
#include <QOTF/utility.h>

namespace QOTF
{
namespace _MortonMask
{
	template<uint N, uint D, uint64_t M>
	constexpr uint64_t _get(uint n)
	{
		if(n == 0)
			return M;
		else
		{
			const uint	   shift = powerOfTwo(4 + n - N) * D;
			const uint64_t mask	 = shift > 63 ? M : _get<N, D, M>(n - 1);
			return (shift > 63 ? mask : (mask | (mask << shift)));
		}
	}

	template<uint N, uint D, uint64_t M>
	constexpr uint64_t get()
	{
		return _get<N, D, M>(N);
	}
} // namespace _MortonMask

#define MASK1 0x0000FFFFULL
#define MASK2 0x000000FFULL
#define MASK3 0x0000000FULL
#define MASK4 0x00000003ULL
#define MASK5 0x00000001ULL

template<uint D>
uint64_t CompactMortonCode<D>::split(uint n)
{
	static constexpr uint	  dFactor	 = D - 2;
	static constexpr uint64_t numberMask = ((uint64_t)1 << (64 / D)) - 1;

	static constexpr uint shift1 = 16 << dFactor;
	static constexpr uint shift2 = 8 << dFactor;
	static constexpr uint shift3 = 4 << dFactor;
	static constexpr uint shift4 = 2 << dFactor;
	static constexpr uint shift5 = 1 << dFactor;

	static constexpr uint64_t mask1 = _MortonMask::get<1, D, MASK1>();
	static constexpr uint64_t mask2 = _MortonMask::get<2, D, MASK2>();
	static constexpr uint64_t mask3 = _MortonMask::get<3, D, MASK3>();
	static constexpr uint64_t mask4 = _MortonMask::get<4, D, MASK4>();
	static constexpr uint64_t mask5 = _MortonMask::get<5, D, MASK5>();

	// Keep the first (64 / D) bits
	uint64_t x = n & numberMask;

	x = (x | x << shift1) & mask1;
	x = (x | x << shift2) & mask2;
	x = (x | x << shift3) & mask3;
	x = (x | x << shift4) & mask4;
	x = (x | x << shift5) & mask5;

	return x;
}

#undef MASK1
#undef MASK2
#undef MASK3
#undef MASK4
#undef MASK5

template<uint D>
uint64_t CompactMortonCode<D>::encode(const Point& coords)
{
	uint64_t code = 0;

	for(uint i = 1; i <= D; ++i)
		code |= split(coords[i - 1]) << (D - i);

	return code;
}

template<uint D>
uint CompactMortonCode<D>::decode(uint level) const
{
	static constexpr uint mask = (1 << D) - 1;
	return (m_code >> (D * level)) & mask;
}

} // namespace QOTF
