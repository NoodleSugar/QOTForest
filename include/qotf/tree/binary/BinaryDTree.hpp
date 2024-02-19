#pragma once

#include <qotf/DTree.hpp>
#include <qotf/internal/BitVector.hpp>

namespace qotf
{

/**
 * A structure for compact trees with no label
 * It has 3 types of nodes :
 *  - Composite  (NodeState::CompositeEmpty)
 *  - Leaf empty (NodeState::LeafEmpty)
 *  - Leaf full  (NodeState::LeafFilled)
 */
template<uint D>
class BinDTree final : public DTree<D>
{
	static_assert(D < 8);

	static constexpr size_t kDefaultLabelCount = 1;
	static constexpr ushort kNodeSize		  = 2;
	static constexpr ushort kByteSize		  = 8;
	static constexpr byte	kNodeMask		  = byte{0b11};

	class NodeIndex
	{
		static constexpr byte kByteMask = 0b111;

		// There are four nodes per bytes
		// The first node of a byte is on the two leftmost bits
		// In order to read it, we need to shift the byte to the right by six bits
		static constexpr uint kFirstNodeShift  = 6u;
		static constexpr uint kFourthNodeShift = 0u;

	public:
		size_t byteIndex;
		uint   bitShift;

		NodeIndex();
		explicit NodeIndex(size_t bitIndex);
		NodeIndex(const NodeIndex&) = default;

		NodeIndex& operator=(const NodeIndex&) = default;

		size_t toBitIndex() const;

		NodeIndex& operator++();
		NodeIndex  operator++(int);

		NodeIndex& operator--();
		NodeIndex  operator--(int);
	};

public:
	BinDTree(uint maxDepth, uint initNodeCount = 0);

	uint getDepth() const override { return m_depth; }
	uint getNodeCount() const override { return m_nodeCount; }

	/*
	 * Note : if the target node is a subnode of a leaf, then it returns the state
	 * of this leaf
	 */
	NodeState getNodeState(const MortonCode<D>&, uint nodeDepth) const override;

	void setNode(const MortonCode<D>&, uint nodeDepth);

	void removeNode(const MortonCode<D>&, uint nodeDepth);

private:
	internal::BitVector m_bitArray;

	uint m_depth;
	uint m_nodeCount;

	NodeState getNodeState(NodeIndex index) const;
	void	  setNodeState(NodeIndex index, NodeState node);
	void	  cleanNode(NodeIndex index);

	/**
	 * Get the index of the child at [childPos], of the node at [index]
	 * Requires :
	 *   - the node at [index] must be Composed
	 */
	NodeIndex getChildIndex(NodeIndex index, uint childPos) const;

	/**
	 * Get the index of the end of the parent node at [index]
	 * Requires :
	 *   - the node at [index] must be Composed
	 */
	NodeIndex getParentEndIndex(NodeIndex index) const;

	/**
	 * Add [filled] children to the parent node at [index]
	 */
	void addChildren(NodeIndex index, NodeState child);

	/**
	 * Remove the children of the parent node at [index]
	 * Requires :
	 *   - the node at [index] must be Composed
	 */
	void removeChildren(NodeIndex index);

	/**
	 * Return whether or not the node at [index] has been optimized
	 * If the node is Composite and its children are all Full (resp. Empty)
	 * then this node become Full (resp. Empty) and its children are removed
	 */
	bool optimizeNode(NodeIndex index);
};

/***************************
 * BinDTree implementation *
 ***************************/

template<uint D>
inline NodeState BinDTree<D>::getNodeState(NodeIndex index) const
{
	const byte& bytes = m_bitArray.getData()[index.byteIndex];
	return static_cast<NodeState>((bytes >> index.bitShift) & kNodeMask);
}

template<uint D>
inline void BinDTree<D>::cleanNode(NodeIndex index)
{
	byte& bytes = m_bitArray.getData()[index.byteIndex];
	bytes &= ~(kNodeMask << index.bitShift);
}

template<uint D>
inline void BinDTree<D>::setNodeState(NodeIndex index, NodeState node)
{
	m_bitArray.setBit();
	byte& bytes = m_bitArray.getData()[index.byteIndex];
	cleanNode(index);
	bytes |= static_cast<byte>(node) << index.bitShift;
}

template<uint D>
inline BinDTree<D>::BinDTree(uint maxDepth, uint initNodeNumber) :
	m_bitArray(initNodeNumber ?
				initNodeNumber * kNodeSizeInBits :
				kDefaultLabelCount * kNodeSizeInBits),
	m_depth(maxDepth),
	m_nodeCount(initNodeNumber ? initNodeNumber : kDefaultLabelCount)
{
}

template<uint D>
inline typename BinDTree<D>::NodeIndex BinDTree<D>::getChildIndex(NodeIndex index, uint childPos) const
{
	NodeIndex& childIndex = ++index;

	while(childPos)
	{
		if(getNodeState(childIndex) == NodeState::CompositeEmpty)
			// Prepare to skip the children of the composite node
			childPos += BinDTree<D>::CHILDREN_COUNT;

		// Skip the current node
		--childPos;
		++childIndex;
	}
	return childIndex;
}

template<uint D>
inline typename BinDTree<D>::NodeIndex BinDTree<D>::getParentEndIndex(NodeIndex index) const
{
	NodeIndex endIndex = ++index;

	uint remainingNode = BinDTree<D>::CHILDREN_COUNT;
	while(remainingNode)
	{
		if(getNodeState(endIndex) == NodeState::CompositeEmpty)
			// Prepare to skip the children of the composite node
			remainingNode += BinDTree<D>::CHILDREN_COUNT;

		// Skip the current node
		--remainingNode;
		++endIndex;
	}
	return endIndex;
}

template<uint D>
inline void BinDTree<D>::addChildren(NodeIndex index, NodeState child)
{
	m_bitArray.insert(index.toBitIndex(), BinDTree<D>::CHILDREN_COUNT * kNodeSizeInBits);

	for(uint i = 0; i < D; i++)
		setNodeState(index++, child);

	m_nodeCount += BinDTree<D>::CHILDREN_COUNT;
}

template<uint D>
inline void BinDTree<D>::removeChildren(NodeIndex index)
{
	const NodeIndex nodeEndIndex = getParentEndIndex(index);

	const size_t numberBitsToRemove = nodeEndIndex.toBitIndex() - index.toBitIndex() - 2;

	m_bitArray.remove(index.toBitIndex(), numberBitsToRemove);
	m_nodeCount -= numberBitsToRemove / kNodeSizeInBits;
}

template<uint D>
inline bool BinDTree<D>::optimizeNode(NodeIndex parentIndex)
{
	++parentIndex;
	NodeIndex&		childIndex		= parentIndex;
	const NodeIndex firstChildIndex = parentIndex;

	const NodeState firstChild = getNodeState(childIndex);
	if(firstChild == NodeState::CompositeEmpty)
		return false;

	for(uint i = 1; i < BinDTree<D>::CHILDREN_COUNT; ++i)
	{
		++childIndex;
		if(firstChild != getNodeState(childIndex))
			return false;
	}

	removeChildren(firstChildIndex);
	setNodeState(parentIndex, firstChild);

	return true;
}

template<uint D>
NodeState BinDTree<D>::getNodeState(const MortonCode<D>& mortonCode, uint nodeDepth) const
{
	uint	  level		= m_depth - 1;
	uint	  nodeLevel = m_depth - nodeDepth;
	NodeIndex index;

	// Road to the node
	while(level > nodeLevel)
	{
		// Check the current node state
		switch(getNodeState(index))
		{
		case NodeState::LeafEmpty:
			return NodeState::LeafEmpty;
		case NodeState::LeafFilled:
			return NodeState::LeafFilled;
		case NodeState::CompositeEmpty:
			index = getChildIndex(index, mortonCode.decode(--level));
			break;
		case NodeState::CompositeFilled:
			// TODO throw custom exception
			throw std::logic_error("BitOctree::getNodeState : Error while reading nodes");
		}
	}

	switch(getNodeState(index))
	{
	case NodeState::LeafEmpty:
		return NodeState::LeafEmpty;
	case NodeState::LeafFilled:
		return NodeState::LeafFilled;
	case NodeState::CompositeEmpty:
		return NodeState::CompositeEmpty;
	case NodeState::CompositeFilled:
		// TODO throw custom exception
		throw std::logic_error("BitOctree::getNodeState : Error while reading nodes");
	};
}

template<uint D>
void BinDTree<D>::setNode(const MortonCode<D>& mortonCode, uint nodeDepth)
{
	uint	  level		= m_depth - 1;
	uint	  nodeLevel = m_depth - nodeDepth;
	NodeIndex index;

	std::vector<NodeIndex> nodeIndexStack;
	nodeIndexStack.reserve(m_depth);
	nodeIndexStack.push_back(index);

	// Road to the node
	while(level > nodeLevel)
	{
		// Check the current node state
		switch(getNodeState(index))
		{
		case NodeState::LeafEmpty:
			goto leaf_empty;
		case NodeState::LeafFilled:
			return;
		case NodeState::CompositeEmpty:
			index = getChildIndex(index, mortonCode.decode(--level));
			break;
		case NodeState::CompositeFilled:
			throw std::logic_error("BitOctree::getNodeState : Error while reading nodes");
		}
		nodeIndexStack.push_back(index);
	}

	// Check target node state
	switch(getNodeState(index))
	{
	case NodeState::LeafEmpty:
		setNodeState(index, NodeState::LeafFilled);
		nodeIndexStack.pop_back();
		break;
	case NodeState::LeafFilled:
		return;
	case NodeState::CompositeEmpty:
		removeChildren(index);
	case NodeState::CompositeFilled:
		// TODO throw custom exception
		throw std::logic_error("BitOctree::getNodeState : Error while reading nodes");
	}

	// While optimization is possible
	while(!nodeIndexStack.empty())
	{
		if(!optimizeNode(nodeIndexStack.back()))
			return;
		nodeIndexStack.pop_back();
	}
	return;

	while(level > nodeLevel)
	{
leaf_empty:
		NodeIndex childrenIndex = index;
		++childrenIndex;

		// Set node to composite
		setNodeState(index, NodeState::CompositeEmpty);

		// Add empty children to this node
		addChildren(childrenIndex, NodeState::LeafEmpty);

		// Go to the target child of this node
		index = getChildIndex(index, mortonCode.decode(--level));
	}
	setNodeState(index, NodeState::LeafFilled);
}

template<uint D>
void BinDTree<D>::removeNode(const MortonCode<D>& mortonCode, uint nodeDepth)
{
	uint	  level		= m_depth - 1;
	uint	  nodeLevel = m_depth - nodeDepth;
	NodeIndex index;

	std::vector<NodeIndex> nodeIndexStack;
	nodeIndexStack.reserve(m_depth);
	nodeIndexStack.push_back(index);

	// Road to the node
	// TODO faire des boucles for à la place des while ?
	while(level > nodeLevel)
	{
		// Check the current node state
		uint childBitPos;
		switch(getNodeState(index))
		{
		case NodeState::LeafEmpty:
			return;
		case NodeState::LeafFilled:
			goto leaf_filled;
		case NodeState::CompositeEmpty:
			index = getChildIndex(index, mortonCode.decode(--level));
			break;
		case NodeState::CompositeFilled:
			// TODO throw custom exception
			throw std::logic_error("BitOctree::getNodeState : Error while reading nodes");
		}
		nodeIndexStack.push_back(index);
	}

	// Check target node state
	switch(getNodeState(index))
	{
	case NodeState::LeafEmpty:
		return;
	case NodeState::LeafFilled:
		cleanNode(index);
		nodeIndexStack.pop_back();
		break;
	case NodeState::CompositeEmpty:
		removeChildren(index);
	case NodeState::CompositeFilled:
		// TODO throw custom exception
		throw std::logic_error("BitOctree::getNodeState : Error while reading nodes");
	}

	// While optimization possible
	while(!nodeIndexStack.empty())
	{
		if(!optimizeNode(nodeIndexStack.back()))
			break;
		nodeIndexStack.pop_back();
	}
	return;

	while(level > nodeLevel)
	{
leaf_filled:
		// Set the node to Composite
		setNodeState(index, NodeState::CompositeEmpty);
		++index;

		// Add filled children to this node
		addChildren(index, NodeState::LeafFilled);
		--index;

		// Go to the target child of this node
		index = getChildIndex(index, mortonCode.decode(--level));
	}
	cleanNode(index);
}

/*****************************
 * Node Index implementation *
 *****************************/

template<uint D>
inline BinDTree<D>::NodeIndex::NodeIndex() :
	byteIndex(0),
	bitShift(kFirstNodeShift)
{
}

template<uint D>
inline BinDTree<D>::NodeIndex::NodeIndex(size_t bitIndex) :
	byteIndex(bitIndex >> kNodeSizeInBits),
	bitShift(kFirstNodeShift - (bitIndex & kByteMask))
{
}

template<uint D>
inline size_t BinDTree<D>::NodeIndex::toBitIndex() const
{
	return (byteIndex << kNodeSizeInBits) + kFirstNodeShift - bitShift;
}

template<uint D>
inline typename BinDTree<D>::NodeIndex& BinDTree<D>::NodeIndex::operator++()
{
	if(bitShift == kFourthNodeShift)
	{
		byteIndex++;
		bitShift = kFirstNodeShift;
	}
	else
		bitShift -= kNodeSizeInBits;
	return *this;
}

template<uint D>
inline typename BinDTree<D>::NodeIndex BinDTree<D>::NodeIndex::operator++(int)
{
	const NodeIndex result = *this;
	++(*this);
	return result;
}

template<uint D>
inline typename BinDTree<D>::NodeIndex& BinDTree<D>::NodeIndex::operator--()
{
	if(bitShift == kFirstNodeShift)
	{
		byteIndex--;
		bitShift = kFourthNodeShift;
	}
	else
		bitShift += kNodeSizeInBits;
	return *this;
}

template<uint D>
inline typename BinDTree<D>::NodeIndex BinDTree<D>::NodeIndex::operator--(int)
{
	const NodeIndex result = *this;
	--(*this);
	return result;
}

} // namespace qotf