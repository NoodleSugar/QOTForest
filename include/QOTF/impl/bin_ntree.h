#ifndef BTREE_H
#define BTREE_H

#include "QOTF/bit_array.h"
#include "QOTF/ntree.h"
#include "QOTF/utility.h"
#include "bin_ntree_node_index.h"

namespace QOTF
{
/**
 * A structure for compact trees with no label
 * It has 3 types of nodes :
 *  - Composite  (NodeState::COMPOSITE_EMPTY)
 *  - Leaf empty (NodeState::LEAF_EMPTY)
 *  - Leaf full  (NodeState::LEAF_FILLED)
 */
template<uint D>
class BinNTree : public NTree<D>
{
public:
	BinNTree(uint maxDepth, uint initNodeNumber = 0);

	uint getDepth() const override { return m_depth; }
	uint getNodeNumber() const override { return m_nodeNumber; }

	/*
	 * Note : if the target node is a subnode of a leaf, then it returns the state of this leaf
	 */
	NodeState getNodeState(const MortonCode<D>&, uint nodeDepth) const override;

	void setNode(const MortonCode<D>&, uint nodeDepth);

	void removeNode(const MortonCode<D>&, uint nodeDepth);

private:
	BitArray m_bitArray;
	uint	 m_depth;
	uint	 m_nodeNumber;

	/**
	 * Get the index of the child at [childPos], of the node at [nodeIndex]
	 * Requires :
	 *   - the node at [nodeIndex] must be Composed
	 */
	_BinNTree::NodeIndex getChildIndex(const _BinNTree::NodeIndex& nodeIndex, uint childPos) const;

	/**
	 * Get the index of the end of the parent node at [nodeIndex]
	 *  Requires :
	 *   - the node at [nodeIndex] must be Composed
	 */
	_BinNTree::NodeIndex getParentEndIndex(const _BinNTree::NodeIndex& nodeIndex) const;

	/**
	 * Add [filled] children to the parent node at [nodeIndex]
	 */
	void addChildren(const _BinNTree::NodeIndex& nodeIndex, bool filled = false);

	/**
	 * Remove the children of the parent node at [nodeIndex]
	 *  Requires :
	 *   - the node at [nodeIndex] must be Composed
	 */
	void removeChildren(const _BinNTree::NodeIndex& nodeIndex);

	/**
	 * Return whether or not the node at [nodeIndex] has been optimized
	 * If the node is Composite and its children are all Full (resp. Empty)
	 * then this node become Full (resp. Empty) and its children are removed
	 */
	bool optimizeNode(const _BinNTree::NodeIndex& nodeIndex);
};

} // namespace QOTF

#include "bin_ntree.tpp"

#endif
