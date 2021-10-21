#ifndef LABELED_NTREE_H
#define LABELED_NTREE_H

#include "ntree.h"

namespace QOTF
{
template<uint D, typename L>
class LabeledTree : public NTree<D>
{
public:
	virtual ~LabeledTree() {}

	/**
	 * Get the label of the node at specific coordinates and level
	 * The level represents the "size" of the node (in contrary to the depth) :
	 *  - level = 0 : the deepest node is selected
	 *  - level = max_depth : the root node is selected
	 */
	virtual L getNodeLabel(uint64_t mortonCode, uint nodeLevel) const = 0;

	/**
	 * Set the label of the node at specific coordinates and level
	 */
	virtual void setNodeLabel(uint64_t mortonCode, uint nodeLevel, const L& label) = 0;
};
} // namespace QOTF

#endif
