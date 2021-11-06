#ifndef NTREE_H
#define NTREE_H

#include "morton/morton_code.h"
#include "node_state.h"
#include "utility.h"
#include <cstdint>

namespace QOTF
{
/**
 * D : spatial dimension of the tree
 */
template<uint D>
class NTree
{
	static_assert(D >= 2);

public:
	virtual ~NTree() {}

	static constexpr uint CHILDREN_NUMBER = powerOfTwo(D);

	virtual uint getDepth() const = 0;

	virtual uint getNodeNumber() const = 0;

	/**
	 * Get the label of the node at specific coordinates and depth
	 *  - depth = 1 :
	 *  	the root node is selected
	 *  - depth = max_depth :
	 *  	the deepest node is selected
	 */
	virtual NodeState getNodeState(const MortonCode<D>&, uint nodeDepth) const = 0;
};

} // namespace QOTF

#endif
