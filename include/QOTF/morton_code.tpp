#pragma onde

#include "morton_code.h"
#include <QOTF/utility.h>

namespace QOTF
{
namespace _Mask
{
	template<uint N, uint NN, uint D, uint64_t M>
	struct _Rec
	{
		enum
		{
			shift = (PowerOfTwo<4 + N - NN>::value * D),
			mask  = (shift > 63 ? M : _Rec<N - 1, NN, D, M>::value),
			value = (shift > 63 ? mask : (mask | (mask << shift)))
		};
	};

	template<uint NN, uint D, uint64_t M>
	struct _Rec<0, NN, D, M>
	{
		enum
		{
			shift = 0,
			mask  = M,
			value = M
		};
	};

	template<uint N, uint D, uint64_t M>
	using Rec = _Rec<N, N, D, M>;

} // namespace _Mask

#define MASK1 0x0000FFFFULL
#define MASK2 0x000000FFULL
#define MASK3 0x0000000FULL
#define MASK4 0x00000003ULL
#define MASK5 0x00000001ULL

template<uint D>
uint64_t MortonCode<D>::split(uint n)
{
	static constexpr uint	  dFactor	 = D - 2;
	static constexpr uint64_t numberMask = ((uint64_t)1 << (64 / D)) - 1;

	static constexpr uint shift1 = 16 << dFactor;
	static constexpr uint shift2 = 8 << dFactor;
	static constexpr uint shift3 = 4 << dFactor;
	static constexpr uint shift4 = 2 << dFactor;
	static constexpr uint shift5 = 1 << dFactor;

	static constexpr uint64_t mask1 = _Mask::Rec<1, D, MASK1>::value;
	static constexpr uint64_t mask2 = _Mask::Rec<2, D, MASK2>::value;
	static constexpr uint64_t mask3 = _Mask::Rec<3, D, MASK3>::value;
	static constexpr uint64_t mask4 = _Mask::Rec<4, D, MASK4>::value;
	static constexpr uint64_t mask5 = _Mask::Rec<5, D, MASK5>::value;

	// Keep the first (64 / D) bits
	uint64_t x = n & numberMask;

	x = (x | x << shift1) & mask1;
	x = (x | x << shift2) & mask2;
	x = (x | x << shift3) & mask3;
	x = (x | x << shift4) & mask4;
	x = (x | x << shift5) & mask5;

	return x;
}

template<uint D>
uint64_t MortonCode<D>::encode(const Point& coords)
{
	uint64_t code = 0;

	for(int i = 1; i <= D; i++)
		code |= split(coords[i - 1]) << (D - i);

	return code;
}

template<uint D>
uint MortonCode<D>::decode(uint64_t c, uint level)
{
	const static uint mask = (1 << D) - 1;
	return (c >> (D * level)) & mask;
}

} // namespace QOTF
