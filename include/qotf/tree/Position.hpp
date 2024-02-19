#pragma once

#include "qotf/morton/MortonCode.hpp"

namespace qotf
{

template<uint Dimension>
struct Position final
{
	MortonCode<Dimension> code;
	uint			  depth;
};

} // namespace qotf