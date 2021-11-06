#pragma once

#include "bin_ntree.h"
#include <stdexcept>
#include <string>

namespace QOTF
{
#define DEFAULT_NODE_NUMBER 1

#define NODE_SIZE_IN_BITS 2
#define BYTE_SIZE_IN_BITS 8

#define NODE_MASK 0b11

#define NODE_LEAF_EMPTY_BITS	  0b00
#define NODE_LEAF_FILLED_BITS	  0b11
#define NODE_COMPOSITE_EMPTY_BITS 0b01

#define NODE_PATTERN(node) (node << 6)

#define NODE_TO_STRING(node)                    \
	std::string(((node)&0b01 ?                  \
					((node)&0b10 ? "11" : "01") : \
					((node)&0b10 ? "10" : "00")))

#define BYTES  ((byte*)m_bitArray.getDirectAccess())
#define CBYTES ((const byte*)m_bitArray.getDirectAccess())

#define GET_NODE(nodeIndex) \
	((CBYTES[(nodeIndex).byteIndex] >> (nodeIndex).bitShift) & NODE_MASK)
#define CLEAN_NODE(nodeIndex) \
	(BYTES[(nodeIndex).byteIndex] &= ~(NODE_MASK << (nodeIndex).bitShift))
#define SET_NODE(nodeIndex, node) \
	CLEAN_NODE(nodeIndex);        \
	(BYTES[(nodeIndex).byteIndex] |= (node) << (nodeIndex).bitShift)

template<uint D>
inline BinNTree<D>::BinNTree(uint maxDepth, uint initNodeNumber) :
	m_bitArray(initNodeNumber ?
				  initNodeNumber * NODE_SIZE_IN_BITS :
				  DEFAULT_NODE_NUMBER * NODE_SIZE_IN_BITS),
	m_depth(maxDepth),
	m_nodeNumber(initNodeNumber ? initNodeNumber : DEFAULT_NODE_NUMBER)
{
}

template<uint D>
inline _BinNTree::NodeIndex BinNTree<D>::getChildIndex(const _BinNTree::NodeIndex& nodeIndex, uint childPos) const
{
	_BinNTree::NodeIndex childIndex(nodeIndex);
	childIndex.next();

	while(childPos)
	{
		if(GET_NODE(childIndex) == NODE_COMPOSITE_EMPTY_BITS)
			// Prepare to skip the children of the composite node
			childPos += BinNTree<D>::CHILDREN_NUMBER;

		// Skip the current node
		childPos--;
		childIndex.next();
	}
	return childIndex;
}

template<uint D>
inline _BinNTree::NodeIndex BinNTree<D>::getParentEndIndex(const _BinNTree::NodeIndex& nodeIndex) const
{
	uint remainingNode = BinNTree<D>::CHILDREN_NUMBER;

	_BinNTree::NodeIndex endIndex(nodeIndex);
	endIndex.next();
	while(remainingNode)
	{
		if(GET_NODE(endIndex) == NODE_COMPOSITE_EMPTY_BITS)
			// Prepare to skip the children of the composite node
			remainingNode += BinNTree<D>::CHILDREN_NUMBER;

		// Skip the current node
		remainingNode--;
		endIndex.next();
	}
	return endIndex;
}

template<uint D>
inline void BinNTree<D>::addChildren(const _BinNTree::NodeIndex& nodeIndex, bool filled)
{
	m_bitArray.insert(nodeIndex.toBitPos(), BinNTree<D>::CHILDREN_NUMBER * NODE_SIZE_IN_BITS, filled);
	m_nodeNumber += BinNTree<D>::CHILDREN_NUMBER;
}

template<uint D>
inline void BinNTree<D>::removeChildren(const _BinNTree::NodeIndex& nodeIndex)
{
	const _BinNTree::NodeIndex nodeEndIndex = getParentEndIndex(nodeIndex);

	size_t numberBitsToRemove = nodeEndIndex.toBitPos() - nodeIndex.toBitPos() - 2;

	m_bitArray.remove(nodeIndex.toBitPos(), numberBitsToRemove);
	m_nodeNumber -= numberBitsToRemove / NODE_SIZE_IN_BITS;
}

template<uint D>
inline bool BinNTree<D>::optimizeNode(const _BinNTree::NodeIndex& parentIndex)
{
	_BinNTree::NodeIndex childIndex = parentIndex;
	childIndex.next();

	const byte firstChild = GET_NODE(childIndex);
	if(firstChild == NODE_COMPOSITE_EMPTY_BITS)
		return false;

	for(int i = 1; i < BinNTree<D>::CHILDREN_NUMBER; i++)
	{
		childIndex.next();
		if(firstChild != GET_NODE(childIndex))
			return false;
	}

	_BinNTree::NodeIndex firstChildIndex = parentIndex;
	firstChildIndex.next();

	removeChildren(firstChildIndex);
	SET_NODE(parentIndex, firstChild);

	return true;
}

template<uint D>
NodeState BinNTree<D>::getNodeState(const MortonCode<D>& mortonCode, uint nodeDepth) const
{
	uint				 level	   = m_depth - 1;
	uint				 nodeLevel = m_depth - nodeDepth;
	_BinNTree::NodeIndex nodeIndex;
	byte				 node;

	// Road to the node
	while(level > nodeLevel)
	{
		// Check the current node state
		switch(node = GET_NODE(nodeIndex))
		{
		case NODE_LEAF_EMPTY_BITS:
			return NodeState::LEAF_EMPTY;
		case NODE_LEAF_FILLED_BITS:
			return NodeState::LEAF_FILLED;
		case NODE_COMPOSITE_EMPTY_BITS:
			nodeIndex = getChildIndex(nodeIndex, mortonCode.decode(--level));
			break;
		default:
			const std::string errMsg("BitOctree::getNodeState : Error while reading nodes\nUNKNOWN NODE : {}");
			throw std::logic_error(errMsg + NODE_TO_STRING(node));
		}
	}

	switch(node = GET_NODE(nodeIndex))
	{
	case NODE_LEAF_EMPTY_BITS:
		return NodeState::LEAF_EMPTY;
	case NODE_LEAF_FILLED_BITS:
		return NodeState::LEAF_FILLED;
	case NODE_COMPOSITE_EMPTY_BITS:
		return NodeState::COMPOSITE_EMPTY;
	default:
		const std::string errMsg("BitOctree::getNodeState : Error while reading nodes\nUNKNOWN NODE : {}");
		throw std::logic_error(errMsg + NODE_TO_STRING(node));
	};
}

template<uint D>
void BinNTree<D>::setNode(const MortonCode<D>& mortonCode, uint nodeDepth)
{
	uint				 level	   = m_depth - 1;
	uint				 nodeLevel = m_depth - nodeDepth;
	_BinNTree::NodeIndex nodeIndex;
	byte				 node;

	std::vector<_BinNTree::NodeIndex> nodeIndexStack;
	nodeIndexStack.reserve(m_depth);
	nodeIndexStack.push_back(nodeIndex);

	// Road to the node
	while(level > nodeLevel)
	{
		// Check the current node state
		switch(node = GET_NODE(nodeIndex))
		{
		case NODE_LEAF_EMPTY_BITS:
			goto leaf_empty;
		case NODE_LEAF_FILLED_BITS:
			return;
		case NODE_COMPOSITE_EMPTY_BITS:
			nodeIndex = getChildIndex(nodeIndex, mortonCode.decode(--level));
			break;
		default:
			const std::string errMsg("BitOctree::getNodeState : Error while reading nodes\nUNKNOWN NODE : {}");
			throw std::logic_error(errMsg + NODE_TO_STRING(node));
		}
		nodeIndexStack.push_back(nodeIndex);
	}

	// Check target node state
	switch(node = GET_NODE(nodeIndex))
	{
	case NODE_COMPOSITE_EMPTY_BITS:
		removeChildren(nodeIndex);
	case NODE_LEAF_EMPTY_BITS:
		SET_NODE(nodeIndex, NODE_LEAF_FILLED_BITS);
		nodeIndexStack.pop_back();
		break;
	case NODE_LEAF_FILLED_BITS:
		return;
	default:
		const std::string errMsg("BitOctree::getNodeState : Error while reading nodes\nUNKNOWN NODE : {}");
		throw std::logic_error(errMsg + NODE_TO_STRING(node));
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
		// Set node to composite
		SET_NODE(nodeIndex, NODE_COMPOSITE_EMPTY_BITS);
		nodeIndex.next();

		// Add empty children to this node
		addChildren(nodeIndex, false);
		nodeIndex.prev();

		// Go to the target child of this node
		nodeIndex = getChildIndex(nodeIndex, mortonCode.decode(--level));
	}
	SET_NODE(nodeIndex, NODE_LEAF_FILLED_BITS);
}

template<uint D>
void BinNTree<D>::removeNode(const MortonCode<D>& mortonCode, uint nodeDepth)
{
	uint				 level	   = m_depth - 1;
	uint				 nodeLevel = m_depth - nodeDepth;
	_BinNTree::NodeIndex nodeIndex;
	byte				 node;

	std::vector<_BinNTree::NodeIndex> nodeIndexStack;
	nodeIndexStack.reserve(m_depth);
	nodeIndexStack.push_back(nodeIndex);

	// Road to the node
	while(level > nodeLevel)
	{
		// Check the current node state
		uint childBitPos;
		node = GET_NODE(nodeIndex);
		switch(node)
		{
		case NODE_LEAF_EMPTY_BITS:
			return;
		case NODE_LEAF_FILLED_BITS:
			goto leaf_filled;
		case NODE_COMPOSITE_EMPTY_BITS:
			nodeIndex = getChildIndex(nodeIndex, mortonCode.decode(--level));
			break;
		default:
			const std::string errMsg("BitOctree::getNodeState : Error while reading nodes\nUNKNOWN NODE : {}");
			throw std::logic_error(errMsg + NODE_TO_STRING(node));
		}
		nodeIndexStack.push_back(nodeIndex);
	}

	// Check target node state
	switch(node = GET_NODE(nodeIndex))
	{
	case NODE_COMPOSITE_EMPTY_BITS:
		removeChildren(nodeIndex);
	case NODE_LEAF_FILLED_BITS:
		CLEAN_NODE(nodeIndex);
		nodeIndexStack.pop_back();
		break;
	case NODE_LEAF_EMPTY_BITS:
		return;
	default:
		const std::string errMsg("BitOctree::getNodeState : Error while reading nodes\nUNKNOWN NODE : {}");
		throw std::logic_error(errMsg + NODE_TO_STRING(node));
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
		SET_NODE(nodeIndex, NODE_COMPOSITE_EMPTY_BITS);
		nodeIndex.next();

		// Add filled children to this node
		addChildren(nodeIndex, true);
		nodeIndex.prev();

		// Go to the target child of this node
		nodeIndex = getChildIndex(nodeIndex, mortonCode.decode(--level));
	}
	CLEAN_NODE(nodeIndex);
}

} // namespace QOTF