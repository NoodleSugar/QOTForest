#ifndef MORTON_CODE_H
#define MORTON_CODE_H

#include <cstdint>

namespace QOTF
{
typedef unsigned int uint;

template<uint D>
class MortonCode
{
	static_assert(D >= 2);

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

} // namespace QOTF

#endif
