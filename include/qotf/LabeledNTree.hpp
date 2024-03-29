#pragma once

#include <qotf/NTree.h>

namespace qotf
{
/**
 * L : type of the labels
 */
template<unsigned int D, typename L>
class LabeledNTree : public NTree<D>
{
public:
	virtual ~LabeledNTree() {}

	/**
	 * Get the label of the node at specific coordinates and depth
	 * The level represents the "size" of the node (in contrary to the depth) :
	 *  - depth = 1 :
	 * 		the root node is selected
	 *  - depth = max_depth :
	 * 		the deepest node is selected
	 */
	virtual const L getNodeLabel(const mortonCode<D>&, uint nodeDepth) const = 0;

	/**
	 * Set the label of the node at specific coordinates and depth
	 */
	virtual void setNodeLabel(const MortonCode<D>&, uint nodeDepth, const L&) = 0;
};
} // namespace qotf