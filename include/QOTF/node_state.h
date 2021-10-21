#ifndef NODE_STATE_H
#define NODE_STATE_H

namespace QOTF
{
/**
 * Right bit :
 *  - 0 = Empty
 *  - 1 = Filled (contains data)
 * Left bit :
 *  - 0 = Leaf      (non parent/has no children)
 *  - 1 = Composite (parent/has children)
 */
enum class NodeState
{
	LEAF_EMPTY		 = 0b00,
	LEAF_FILLED		 = 0b01,
	COMPOSITE_EMPTY	 = 0b10,
	COMPOSITE_FILLED = 0b11
};

} // namespace QOTF

#endif
