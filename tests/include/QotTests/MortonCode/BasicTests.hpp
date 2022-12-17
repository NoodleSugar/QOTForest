#pragma once

#include <catch2/catch.hpp>

#include <qotf/morton/BasicMortonCode.hpp>

namespace qotf
{

TEST_CASE("Basic Morton Code 2", "[BasicMortonCode<2>]")
{
	constexpr uint	   kLevelCount = 64;
	constexpr uint64_t kMaxCoord   = 0xFFFF'FFFF'FFFF'FFFF;

	BasicMortonCode<2> c1({0, 0});
	BasicMortonCode<2> c2({1, 1});
	BasicMortonCode<2> c3({2, 2});
	BasicMortonCode<2> c4({3, 3});
	BasicMortonCode<2> c5({63, 2});
	BasicMortonCode<2> c6({0, kMaxCoord});

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

	for(uint i = 0; i < kLevelCount; ++i)
		CHECK(c6.decode(i) == 1);
}

TEST_CASE("Basic Morton Code 3", "[BasicMortonCode<3>]")
{
	constexpr uint	   kLevelCount = 64;
	constexpr uint64_t kMaxCoord   = 0xFFFF'FFFF'FFFF'FFFF;

	BasicMortonCode<3> c1({0, 0, 0});
	BasicMortonCode<3> c2({1, 1, 1});
	BasicMortonCode<3> c3({2, 2, 2});
	BasicMortonCode<3> c4({3, 3, 3});
	BasicMortonCode<3> c5({63, 0, 2});
	BasicMortonCode<3> c6({0, 0, kMaxCoord});

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

	for(uint i = 0; i < kLevelCount; ++i)
		CHECK(c6.decode(i) == 1);
}

} // namespace qotf