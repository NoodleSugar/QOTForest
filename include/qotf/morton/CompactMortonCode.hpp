#pragma once

#include <qotf/morton/MortonCode.hpp>
#include <qotf/utils/Math.hpp>

#include <array>

namespace qotf
{

template<uint D>
class CompactMortonCode final : public MortonCode<D>
{
public:
	using Point = std::array<uint32_t, D>;

	CompactMortonCode()							= delete;
	CompactMortonCode(const CompactMortonCode&) = default;

	explicit constexpr CompactMortonCode(const Point& p) :
		m_code(encode(p)) {}

	CompactMortonCode& operator=(const CompactMortonCode&) = default;

	constexpr uint decode(uint level) const override;

private:
	uint64_t m_code;

	template<uint N, uint64_t M, uint n = N>
	constexpr static uint64_t getMask();

	constexpr static uint64_t encode(const Point&);
	constexpr static uint64_t split(uint);
};

template<uint D>
template<uint N, uint64_t M, uint n>
constexpr uint64_t CompactMortonCode<D>::getMask()
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

template<uint D>
constexpr uint64_t CompactMortonCode<D>::split(uint n)
{
	constexpr uint	   dFactor	= D - 2;
	constexpr uint64_t bitsMask = (1ULL << (64 / D)) - 1;

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

	// Keep the first (64 / D) bits
	uint64_t x = n & bitsMask;

	x = (x | x << shift1) & mask1;
	x = (x | x << shift2) & mask2;
	x = (x | x << shift3) & mask3;
	x = (x | x << shift4) & mask4;
	x = (x | x << shift5) & mask5;

	return x;
}

template<uint D>
constexpr uint64_t CompactMortonCode<D>::encode(const Point& coords)
{
	uint64_t code = 0;

	for(uint i = 1; i <= D; ++i)
		code |= split(coords[i - 1]) << (D - i);

	return code;
}

template<uint D>
constexpr uint CompactMortonCode<D>::decode(uint level) const
{
	constexpr uint mask = (1 << D) - 1;
	return (m_code >> (D * level)) & mask;
}

} // namespace qotf