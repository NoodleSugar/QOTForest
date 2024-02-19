#pragma once

#include <qotf/MutDTree.hpp>

namespace qotf
{

template<class Dimension>
class CompactDefaultTree final : MutDTree<Dimension>
{
	virtual Node	 getRoot() const					  = 0;
	virtual Node	 getNode(const Position<D>&) const	  = 0;
	virtual DTreeRef getSubTree(const Position<D>&) const = 0;
	virtual DTreeRef getChildTree(uint index) const		  = 0;

	virtual uint getDepth() const = 0;
};

}