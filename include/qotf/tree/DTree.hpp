#pragma once

#include <memory>

#include "Position.hpp"
#include "qotf/tree/DefaultLabel.hpp"
#include "qotf/utils/Math.hpp"
#include "qotf/utils/Types.hpp"

namespace qotf
{

template<uint D, class L = DefaultLabel>
class DTree
{
protected:
	static constexpr auto CHILDREN_COUNT = powerOfTwo(D);

	using NodeType	   = Node<D, L>;
	using PositionType = Position<D>;

public:
	virtual NodeType getRoot() const				 = 0;
	virtual NodeType getNode(const Coords<D>&) const = 0;

	virtual uint getDepth() const = 0;

	virtual ~DTree() {}
};

} // namespace qotf