#pragma once

#include "basic_morton_code.h"

namespace QOTF
{
template<uint D>
uint BasicMortonCode<D>::decode(uint level) const
{
	uint index = 0;

	for(int i = 1; i <= D; i++)
		index |= ((m_coords[i - 1] >> level) & 1) << D - i;

	return index;
}

} // namespace QOTF