#pragma once

#include <qotf/morton/MortonCode.hpp>

#include <array>

namespace qotf
{
template<uint D>
class BasicMortonCode final : public MortonCode<D>
{
public:
	using Point = std::array<uint64_t, D>;

	BasicMortonCode()						= delete;
	BasicMortonCode(const BasicMortonCode&) = default;

	explicit constexpr BasicMortonCode(const Point& p) :
		m_coords(p) {}

	BasicMortonCode& operator=(const BasicMortonCode&) = default;

	constexpr uint decode(uint level) const override;

private:
	const Point m_coords;
};

template<uint D>
constexpr uint BasicMortonCode<D>::decode(uint level) const
{
	uint index = 0;

	for(uint i = 1; i <= D; ++i)
		index |= ((m_coords[i - 1] >> level) & 1) << D - i;

	return index;
}

} // namespace qotf