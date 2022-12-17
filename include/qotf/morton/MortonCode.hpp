#pragma once

#include <qotf/utils/Type.hpp>

namespace qotf
{

template<uint D>
class MortonCode
{
	static_assert(D > 1);

public:
	virtual ~MortonCode() {}

	/**
	 * Decodes the Morton Code at the specific level,
	 * and returns the index of the target child
	 * The level is the opposite of the depth in trees
	 * 	- level = 0 :
	 * 		the target node is the deepest one
	 * 	- level = max_level :
	 * 		the target node is the root
	 */
	virtual uint decode(uint level) const = 0;
};

} // namespace qotf