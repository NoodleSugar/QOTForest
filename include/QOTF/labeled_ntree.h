#ifndef LABELED_NTREE_H
#define LABELED_NTREE_H

#include "ntree.h"

namespace QOTF
{
template<uint D, typename L>
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
	virtual const L getNodeLabel(uint64_t mortonCode, uint nodeDepth) const = 0;

	/**
	 * Set the label of the node at specific coordinates and depth
	 */
	virtual void setNodeLabel(uint64_t mortonCode, uint nodeDepth, const L& label) = 0;
};
} // namespace QOTF

#endif
