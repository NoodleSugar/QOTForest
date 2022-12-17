#pragma once

namespace qotf
{

/**
 * Right bit :
 *  - 0 = Empty
 *  - 1 = Filled (contains data)
 * Left bit :
 *  - 0 = Leaf      (non parent/has no children)
 *  - 1 = Composite (parent/has children)
 */
enum class NodeState : byte
{
	LeafEmpty		= 0b00,
	LeafFilled		= 0b01,
	CompositeEmpty	= 0b10,
	CompositeFilled = 0b11
};

} // namespace qotf