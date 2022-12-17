#pragma once

#include <qotf/morton/MortonCode.hpp>
#include <qotf/utils/Math.hpp>
#include <qotf/utils/NodeState.hpp>
#include <qotf/utils/Type.hpp>

namespace qotf
{
/**
 * D : spatial dimension of the tree
 */
template<uint D>
class NTree
{
	static_assert(D > 1);

protected:
	static constexpr uint kChildrenCount = powerOfTwo(D);

public:
	virtual ~NTree() {}

	virtual uint getDepth() const = 0;

	virtual uint getNodeCount() const = 0;

	/**
	 * Get the state of the node at specific coordinates and depth
	 *  - depth = 1 :
	 *  	the root node is selected
	 *  - depth = max_depth :
	 *  	the deepest node is selected
	 */
	virtual NodeState getNodeState(const MortonCode<D>&, uint nodeDepth) const = 0;
};

} // namespace qotf