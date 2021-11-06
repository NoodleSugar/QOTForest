#ifndef BASIC_MORTON_CODE
#define BASIC_MORTON_CODE

#include "morton_code.h"
#include <array>

namespace QOTF
{
template<uint D>
class BasicMortonCode : public MortonCode<D>
{
public:
	using Point = std::array<uint, D>;

	BasicMortonCode() = delete;

	explicit BasicMortonCode(const Point& p) :
		m_coords(p) {}

	uint decode(uint level) const override;

private:
	Point m_coords;
};

} // namespace QOTF

#include "basic_morton_code.tpp"

#endif