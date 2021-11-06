#pragma once
#include <QOTF/morton/basic_morton_code.h>
#include <QOTF/morton/compact_morton_code.h>
#include <catch2/catch.hpp>

namespace QOTF
{
TEST_CASE("Basic Morton Code 2", "[BasicMortonCode<2>]")
{
	const uint MAX_DEPTH = sizeof(uint);

	BasicMortonCode<2> c1({0, 0});
	BasicMortonCode<2> c2({1, 1});
	BasicMortonCode<2> c3({2, 2});
	BasicMortonCode<2> c4({3, 3});
	BasicMortonCode<2> c5({63, 2});
	BasicMortonCode<2> c6({0, (1 << MAX_DEPTH) - 1});
	BasicMortonCode<2> c7({0, 1 << MAX_DEPTH});

	CHECK(c1.decode(0) == 0);
	CHECK(c2.decode(0) == 3);
	CHECK(c3.decode(0) == 0);
	CHECK(c3.decode(1) == 3);
	CHECK(c4.decode(0) == 3);
	CHECK(c4.decode(1) == 3);
	CHECK(c5.decode(0) == 2);
	CHECK(c5.decode(1) == 3);
	CHECK(c5.decode(2) == 2);
	CHECK(c5.decode(3) == 2);
	CHECK(c5.decode(4) == 2);
	CHECK(c5.decode(5) == 2);

	for(int i = 0; i < MAX_DEPTH; i++)
	{
		CHECK(c6.decode(i) == 1);
		CHECK(c7.decode(i) == 0);
	}
}

TEST_CASE("Basic Morton Code 3", "[BasicMortonCode<3>]")
{
	const uint MAX_DEPTH = sizeof(uint);

	BasicMortonCode<3> c1({0, 0, 0});
	BasicMortonCode<3> c2({1, 1, 1});
	BasicMortonCode<3> c3({2, 2, 2});
	BasicMortonCode<3> c4({3, 3, 3});
	BasicMortonCode<3> c5({63, 0, 2});
	BasicMortonCode<3> c6({0, 0, (1 << MAX_DEPTH) - 1});
	BasicMortonCode<3> c7({0, 0, 1 << MAX_DEPTH});

	CHECK(c1.decode(0) == 0);
	CHECK(c2.decode(0) == 7);
	CHECK(c3.decode(0) == 0);
	CHECK(c3.decode(1) == 7);
	CHECK(c4.decode(0) == 7);
	CHECK(c4.decode(1) == 7);
	CHECK(c5.decode(0) == 4);
	CHECK(c5.decode(1) == 5);
	CHECK(c5.decode(2) == 4);
	CHECK(c5.decode(3) == 4);
	CHECK(c5.decode(4) == 4);
	CHECK(c5.decode(5) == 4);

	for(int i = 0; i < MAX_DEPTH; i++)
	{
		CHECK(c6.decode(i) == 1);
		CHECK(c7.decode(i) == 0);
	}
}

TEST_CASE("Compact Morton Code 2", "[CompactMortonCode<2>]")
{
	const uint MAX_DEPTH = 64 / 2;

	CompactMortonCode<2> c1({0, 0});
	CompactMortonCode<2> c2({1, 1});
	CompactMortonCode<2> c3({2, 2});
	CompactMortonCode<2> c4({3, 3});
	CompactMortonCode<2> c5({63, 2});
	CompactMortonCode<2> c6({0, (1 << MAX_DEPTH) - 1});
	CompactMortonCode<2> c7({0, 1 << MAX_DEPTH});

	CHECK(c1.decode(0) == 0);
	CHECK(c2.decode(0) == 3);
	CHECK(c3.decode(0) == 0);
	CHECK(c3.decode(1) == 3);
	CHECK(c4.decode(0) == 3);
	CHECK(c4.decode(1) == 3);
	CHECK(c5.decode(0) == 2);
	CHECK(c5.decode(1) == 3);
	CHECK(c5.decode(2) == 2);
	CHECK(c5.decode(3) == 2);
	CHECK(c5.decode(4) == 2);
	CHECK(c5.decode(5) == 2);

	for(int i = 0; i < MAX_DEPTH; i++)
	{
		CHECK(c6.decode(i) == 1);
		CHECK(c7.decode(i) == 0);
	}
}

TEST_CASE("Compact Morton Code 3", "[CompactMortonCode<3>]")
{
	const uint MAX_DEPTH = 64 / 3;

	CompactMortonCode<3> c1({0, 0, 0});
	CompactMortonCode<3> c2({1, 1, 1});
	CompactMortonCode<3> c3({2, 2, 2});
	CompactMortonCode<3> c4({3, 3, 3});
	CompactMortonCode<3> c5({63, 0, 2});
	CompactMortonCode<3> c6({0, 0, (1 << MAX_DEPTH) - 1});
	CompactMortonCode<3> c7({0, 0, 1 << MAX_DEPTH});

	CHECK(c1.decode(0) == 0);
	CHECK(c2.decode(0) == 7);
	CHECK(c3.decode(0) == 0);
	CHECK(c3.decode(1) == 7);
	CHECK(c4.decode(0) == 7);
	CHECK(c4.decode(1) == 7);
	CHECK(c5.decode(0) == 4);
	CHECK(c5.decode(1) == 5);
	CHECK(c5.decode(2) == 4);
	CHECK(c5.decode(3) == 4);
	CHECK(c5.decode(4) == 4);
	CHECK(c5.decode(5) == 4);

	for(int i = 0; i < MAX_DEPTH; i++)
	{
		CHECK(c6.decode(i) == 1);
		CHECK(c7.decode(i) == 0);
	}
}

} // namespace QOTF