#pragma once
#include <QOTF/morton_code2.h>
#include <QOTF/octree/bin_octree.h>
#include <QOTF/quadtree/bin_quadtree.h>
#include <catch2/catch.hpp>

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
namespace QOTF
{
TEST_CASE("BinNTree init node", "[BinNTree]")
{
	BinQuadtree quadtree(3);
	uint64_t	c = MortonCode2::encode(0, 0);

	uint64_t treeDeepestNodeNumber = 15;

	REQUIRE(quadtree.getDepth() == 3);
	REQUIRE(quadtree.getNodeNumber() == 1);
	REQUIRE(quadtree.getNodeState(c, 1) == NodeState::LEAF_EMPTY);

	SECTION("Empty tree")
	{
		for(c = 0; c < treeDeepestNodeNumber; c++)
			CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_EMPTY);
	}
}

TEST_CASE("BinNTree set node", "[BinNTree]")
{
	BinQuadtree quadtree(3);
	uint64_t	c = MortonCode2::encode(0, 0);

	SECTION("Deepest node")
	{
		quadtree.setNode(c, 3);
		CHECK(quadtree.getNodeNumber() == 9);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::COMPOSITE_EMPTY);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::COMPOSITE_EMPTY);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(0, 1);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(1, 0);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(1, 1);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_EMPTY);
	}

	SECTION("Root node")
	{
		quadtree.setNode(c, 1);
		CHECK(quadtree.getNodeNumber() == 1);

		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(0, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(2, 0);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(2, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
	}

	SECTION("Intermediary node")
	{
		c = MortonCode2::encode(2, 0);
		quadtree.setNode(c, 2);
		CHECK(quadtree.getNodeNumber() == 5);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::COMPOSITE_EMPTY);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(0, 0);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(0, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(2, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
	}
}

TEST_CASE("BinNTree remove node", "[BinNTree]")
{
	BinQuadtree quadtree(3);
	uint64_t	c = MortonCode2::encode(0, 0);

	quadtree.setNode(c, 1);
	CHECK(quadtree.getNodeNumber() == 1);

	SECTION("Deepest node")
	{
		quadtree.removeNode(c, 3);
		CHECK(quadtree.getNodeNumber() == 9);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::COMPOSITE_EMPTY);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::COMPOSITE_EMPTY);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(0, 1);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(1, 0);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(1, 1);
		CHECK(quadtree.getNodeState(c, 3) == NodeState::LEAF_FILLED);
	}

	SECTION("Root node")
	{
		quadtree.removeNode(c, 1);
		CHECK(quadtree.getNodeNumber() == 1);

		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(0, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(2, 0);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(2, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
	}

	SECTION("Intermediary node")
	{
		c = MortonCode2::encode(2, 0);
		quadtree.removeNode(c, 2);
		CHECK(quadtree.getNodeNumber() == 5);

		CHECK(quadtree.getNodeState(c, 1) == NodeState::COMPOSITE_EMPTY);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_EMPTY);
		c = MortonCode2::encode(0, 0);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(0, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
		c = MortonCode2::encode(2, 2);
		CHECK(quadtree.getNodeState(c, 2) == NodeState::LEAF_FILLED);
	}
}

TEST_CASE("BinNTree node set optimization", "[BinNTree]")
{
	BinQuadtree quadtree(3);
	uint64_t	c;

	c = MortonCode2::encode(0, 2);
	quadtree.setNode(c, 2);
	c = MortonCode2::encode(2, 0);
	quadtree.setNode(c, 2);
	c = MortonCode2::encode(2, 2);
	quadtree.setNode(c, 2);

	CHECK(quadtree.getNodeNumber() == 5);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::COMPOSITE_EMPTY);

	c = MortonCode2::encode(0, 0);
	quadtree.setNode(c, 3);
	c = MortonCode2::encode(0, 1);
	quadtree.setNode(c, 3);

	CHECK(quadtree.getNodeNumber() == 9);
	CHECK(quadtree.getNodeState(c, 2) == NodeState::COMPOSITE_EMPTY);

	c = MortonCode2::encode(1, 0);
	quadtree.setNode(c, 3);
	c = MortonCode2::encode(1, 1);
	quadtree.setNode(c, 3);

	CHECK(quadtree.getNodeNumber() == 1);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::LEAF_FILLED);
}

TEST_CASE("BinNTree node remove optimization", "[BinNTree]")
{
	BinQuadtree quadtree(3);
	uint64_t	c;

	c = MortonCode2::encode(0, 0);
	quadtree.setNode(c, 1);

	c = MortonCode2::encode(0, 2);
	quadtree.removeNode(c, 2);
	c = MortonCode2::encode(2, 0);
	quadtree.removeNode(c, 2);
	c = MortonCode2::encode(2, 2);
	quadtree.removeNode(c, 2);

	CHECK(quadtree.getNodeNumber() == 5);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::COMPOSITE_EMPTY);

	c = MortonCode2::encode(0, 0);
	quadtree.removeNode(c, 3);
	c = MortonCode2::encode(0, 1);
	quadtree.removeNode(c, 3);

	CHECK(quadtree.getNodeNumber() == 9);
	CHECK(quadtree.getNodeState(c, 2) == NodeState::COMPOSITE_EMPTY);

	c = MortonCode2::encode(1, 0);
	quadtree.removeNode(c, 3);
	c = MortonCode2::encode(1, 1);
	quadtree.removeNode(c, 3);

	CHECK(quadtree.getNodeNumber() == 1);
	CHECK(quadtree.getNodeState(c, 1) == NodeState::LEAF_EMPTY);
}

} // namespace QOTF