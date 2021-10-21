#ifndef NTREE_H
#define NTREE_H

#include "node_state.h"
#include "utility.h"
#include <cstdint>

namespace QOTF
{
template<uint D>
class NTree
{
	static_assert(D >= 2);

public:
	virtual ~NTree() {}

	static const short MAX_MORTON_DEPTH = 64 / D;
	static const short CHILDREN_NUMBER	= PowerOfTwo<D>::value;

	virtual uint getDepth() const = 0;

	virtual uint getNodeNumber() const = 0;

	/**
	 * Get the label of the node at specific coordinates and depth
	 *  - depth = 1 :
	 *  	the root node is selected
	 *  - depth = max_depth :
	 *  	the deepest node is selected
	 */
	virtual NodeState getNodeState(uint64_t mortonCode, uint nodeDepth) const = 0;
};

} // namespace QOTF

#endif