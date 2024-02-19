#pragma once

#include <memory>

#include "DTree.hpp"

namespace qotf
{

template<uint D, class L = DefaultLabel>
class MutDTree : public DTree<D, L>
{
public:
	virtual void setRoot(const NodeType&)						  = 0;
	virtual void setNode(const PositionType, const NodeType&)	  = 0;
	virtual void setSubTree(const PositionType, const TreeType&) = 0;
};

} // namespace qotf