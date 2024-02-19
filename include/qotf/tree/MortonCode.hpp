#pragma once

#include <qotf/utils/Math.hpp>
#include <qotf/utils/Types.hpp>

namespace qotf
{

template<uint D>
class MortonCode final
{
	static_assert(D > 1, "Morton code can only work for spatial dimension count > 1");

public:
	constexpr uint decode(uint level) const;

	explicit constexpr MortonCode(const Coords<D>& p) :
		m_code(encode(p)) {}

	MortonCode(const MortonCode&) = default;

	MortonCode& operator=(const MortonCode&) = default;

private:
	uint64_t m_code;

	template<uint N, uint64_t M, uint n = N>
	constexpr static uint64_t getMask();

	constexpr static uint64_t encode(const Coords&);
	constexpr static uint64_t split(uint);
};

template<uint D>
template<uint N, uint64_t M, uint n>
constexpr uint64_t MortonCode<D>::getMask()
{
	if constexpr(n == 0)
		return M;
	else
	{
		constexpr uint	   shift = powerOfTwo(4 + n - N) * D;
		constexpr uint64_t mask	 = shift > 63 ? M : getMask<N, M, n - 1>();
		return (shift > 63 ? mask : (mask | (mask << shift)));
	}
}

namespace Mask
{

template<class M>
inline constexpr M filterNRightBits(uint n)
{
	return (M{1} << n) - 1;
}

}

namespace Bits
{

template<class I>
inline constexpr I selectBits(I bits, uint shift, uint count)
{
	constexpr auto mask = Mask::filterNRightBits<uint>(count);
	
	return (bits >> shift) & mask;
}

}

template<uint D>
constexpr uint64_t MortonCode<D>::split(uint n)
{
	// Keep the first (64 / D) bits
	constexpr auto bitsMask = Mask::filterNRightBits<uint64_t>(64 / D));

	uint64_t x = n & bitsMask;

	// Black magic
	constexpr uint dFactor = D - 2;

	constexpr uint shift1 = 16 << dFactor;
	constexpr uint shift2 = 8 << dFactor;
	constexpr uint shift3 = 4 << dFactor;
	constexpr uint shift4 = 2 << dFactor;
	constexpr uint shift5 = 1 << dFactor;

	constexpr uint64_t mask1 = getMask<1, 0x0000FFFFULL>();
	constexpr uint64_t mask2 = getMask<2, 0x000000FFULL>();
	constexpr uint64_t mask3 = getMask<3, 0x0000000FULL>();
	constexpr uint64_t mask4 = getMask<4, 0x00000003ULL>();
	constexpr uint64_t mask5 = getMask<5, 0x00000001ULL>();

	x = (x | x << shift1) & mask1;
	x = (x | x << shift2) & mask2;
	x = (x | x << shift3) & mask3;
	x = (x | x << shift4) & mask4;
	x = (x | x << shift5) & mask5;

	return x;
}

template<uint D>
constexpr uint64_t MortonCode<D>::encode(const Coords<D>& coords)
{
	uint64_t code = 0;

	for(uint i = 1; i <= D; ++i)
		code |= split(coords[i - 1]) << (D - i);

	return code;
}

template<uint D>
constexpr uint MortonCode<D>::decode(uint level) const
{
	return Bits::selectBits(m_code, D * level, D);
}

} // namespace qotf