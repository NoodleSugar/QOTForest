#ifndef COMPACT_MORTON_CODE
#define COMPACT_MORTON_CODE

#include "morton_code.h"

namespace QOTF
{
template<uint D>
class CompactMortonCode : public MortonCode<D>
{
public:
	using Point = std::array<uint, D>;

	CompactMortonCode() = delete;

	explicit CompactMortonCode(const Point& p) :
		m_code(encode(p)) {}

	uint decode(uint level) const override;

private:
	uint64_t m_code;

	static uint64_t encode(const Point&);
	static uint64_t split(uint);
};

} // namespace QOTF

#include "compact_morton_code.tpp"

#endif