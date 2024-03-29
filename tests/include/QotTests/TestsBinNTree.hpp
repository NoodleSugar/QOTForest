#pragma once

#include <catch2/catch.hpp>

#include <qotf/morton/CompactMortonCode.hpp>
#include <qotf/binary/BinNTree.hpp>

/**
 *
 * Representations of a quadtree of depth 3 (with coordinates)
 *
 * Depth = 1 (root node)
 * _________________________
 * | 0,3   1,3   2,3   3,3 |
 * |                       |
 * | 0,2   1,2   2,2   3,2 |
 * |                       |
 * | 0,1   1,1   2,1   3,1 |
 * |                       |
 * | 0,0   1,0   2,0   3,0 |
 * |_______________________|
 *
 * Depth = 2
 * _________________________
 * | 0,3   1,3 | 2,3   3,3 |
 * |           |           |
 * | 0,2   1,2 | 2,2   3,2 |
 * |___________|___________|
 * | 0,1   1,1 | 2,1   3,1 |
 * |           |           |
 * | 0,0   1,0 | 2,0   3,0 |
 * |___________|___________|
 *
 * Depth = 3
 * _________________________
 * | 0,3 | 1,3 | 2,3 | 3,3 |
 * |_____|_____|_____|_____|
 * | 0,2 | 1,2 | 2,2 | 3,2 |
 * |_____|_____|_____|_____|
 * | 0,1 | 1,1 | 2,1 | 3,1 |
 * |_____|_____|_____|_____|
 * | 0,0 | 1,0 | 2,0 | 3,0 |
 * |_____|_____|_____|_____|
 *
 */

namespace qotf
{

using BinQuadtree = BinNTree<2>;

TEST_CASE("BinNTree init node", "[BinNTree]")
{
	BinQuadtree			 quadtree(3);
	CompactMortonCode<2> c({0, 0});

	const uint treeDiv = 4;

	REQUIRE(quadtree.getDepth() == 3);
	REQUIRE(quadtree.getNodeCount() == 1);
	REQUIRE(quadtree.getNodeState(c, 1) == NodeState::LeafEmpty);

	SECTION("Empty tree")
	{
		for(uint x = 0; x < treeDiv; ++x)
			for(uint y = 0; y < treeDiv; ++y)
				CHECK(quadtree.getNodeState(CompactMortonCode<2>({x, y}), 3) == NodeState::LeafEmpty);
	}
}

TEST_CASE("BinNTree set node", "[BinNTree]")
{
	BinQuadtree			 quadtree(3);
	CompactMortonCode<2> c({0, 0});

	SECTION("Deepest node")
	{
		quadtree.setNode(c, 3);
		CHECK(quadtree.getNodeCount() == 9);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::CompositeEmpty);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::CompositeEmpty);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({0, 1});
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({1, 0});
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({1, 1});
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafEmpty);
	}

	SECTION("Root node")
	{
		quadtree.setNode(c, 1);
		CHECK(quadtree.getNodeCount() == 1);

		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({0, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({2, 0});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({2, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
	}

	SECTION("Intermediary node")
	{
		c = CompactMortonCode<2>({2, 0});
		quadtree.setNode(c, 2);
		CHECK(quadtree.getNodeCount() == 5);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::CompositeEmpty);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({0, 0});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({0, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({2, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
	}
}

TEST_CASE("BinNTree remove node", "[BinNTree]")
{
	BinQuadtree			 quadtree(3);
	CompactMortonCode<2> c({0, 0});

	quadtree.setNode(c, 1);
	CHECK(quadtree.getNodeCount() == 1);

	SECTION("Deepest node")
	{
		quadtree.removeNode(c, 3);
		CHECK(quadtree.getNodeCount() == 9);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::CompositeEmpty);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::CompositeEmpty);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({0, 1});
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({1, 0});
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({1, 1});
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LeafFilled);
	}

	SECTION("Root node")
	{
		quadtree.removeNode(c, 1);
		CHECK(quadtree.getNodeCount() == 1);

		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({0, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({2, 0});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({2, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
	}

	SECTION("Intermediary node")
	{
		c = CompactMortonCode<2>({2, 0});
		quadtree.removeNode(c, 2);
		CHECK(quadtree.getNodeCount() == 5);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::CompositeEmpty);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafEmpty);
		c = CompactMortonCode<2>({0, 0});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({0, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
		c = CompactMortonCode<2>({2, 2});
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LeafFilled);
	}
}

TEST_CASE("BinNTree node set optimization", "[BinNTree]")
{
	BinQuadtree
	 quadtree(3);

	CompactMortonCode<2> c({0, 2});
	quadtree.setNode(c, 2);
	c = CompactMortonCode<2>({2, 0});
	quadtree.setNode(c, 2);
	c = CompactMortonCode<2>({2, 2});
	quadtree.setNode(c, 2);

	CHECK(quadtree.getNodeCount() == 5);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::CompositeEmpty);

	c = CompactMortonCode<2>({0, 0});
	quadtree.setNode(c, 3);
	c = CompactMortonCode<2>({0, 1});
	quadtree.setNode(c, 3);

	CHECK(quadtree.getNodeCount() == 9);
	CHECK(quadtree.getNodeState(c, 2) == NodeState::CompositeEmpty);

	c = CompactMortonCode<2>({1, 0});
	quadtree.setNode(c, 3);
	c = CompactMortonCode<2>({1, 1});
	quadtree.setNode(c, 3);

	CHECK(quadtree.getNodeCount() == 1);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::LeafFilled);
}

TEST_CASE("BinNTree node remove optimization", "[BinNTree]")
{
	BinQuadtree quadtree(3);

	CompactMortonCode<2> c({0, 0});
	quadtree.setNode(c, 1);

	c = CompactMortonCode<2>({0, 2});
	quadtree.removeNode(c, 2);
	c = CompactMortonCode<2>({2, 0});
	quadtree.removeNode(c, 2);
	c = CompactMortonCode<2>({2, 2});
	quadtree.removeNode(c, 2);

	CHECK(quadtree.getNodeCount() == 5);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::CompositeEmpty);

	c = CompactMortonCode<2>({0, 0});
	quadtree.removeNode(c, 3);
	c = CompactMortonCode<2>({0, 1});
	quadtree.removeNode(c, 3);

	CHECK(quadtree.getNodeCount() == 9);
	CHECK(quadtree.getNodeState(c, 2) == NodeState::CompositeEmpty);

	c = CompactMortonCode<2>({1, 0});
	quadtree.removeNode(c, 3);
	c = CompactMortonCode<2>({1, 1});
	quadtree.removeNode(c, 3);

	CHECK(quadtree.getNodeCount() == 1);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::LeafEmpty);
}

} // namespace qotf