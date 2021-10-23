#pragma once
#include <QOTF/morton_code.h>
#include <QOTF/morton_code2.h>
#include <QOTF/morton_code3.h>
#include <catch2/catch.hpp>

namespace QOTF
{
TEST_CASE("MortonCode split", "[MortonCode]")
{
	uint64_t split_empty = MortonCode<3>::split(0);
	uint64_t split_inter = MortonCode<3>::split(0b11);
	uint64_t split_full	 = MortonCode<3>::split(0b11111111);

	CHECK(split_empty == 0);
	CHECK(split_inter == 011);
	CHECK(split_full == 011111111);

	split_empty = MortonCode<2>::split(0);
	split_inter = MortonCode<2>::split(0b11);
	split_full	= MortonCode<2>::split(0b11111111);

	CHECK(split_empty == 0);
	CHECK(split_inter == 0b101);
	CHECK(split_full == 0b101010101010101);
}

TEST_CASE("MortonCode encode", "[MortonCode]")
{
	uint64_t c1 = MortonCode<3>::encode({0, 0, 0});
	uint64_t c2 = MortonCode<3>::encode({1, 1, 1});
	uint64_t c3 = MortonCode<3>::encode({2, 2, 2});
	uint64_t c4 = MortonCode<3>::encode({3, 3, 3});
	uint64_t c5 = MortonCode<3>::encode({63, 0, 2});

	CHECK(c1 == 0);
	CHECK(c2 == 07);
	CHECK(c3 == 070);
	CHECK(c4 == 077);
	CHECK(c5 == 0444454);

	c1 = MortonCode<2>::encode({0, 0});
	c2 = MortonCode<2>::encode({1, 1});
	c3 = MortonCode<2>::encode({2, 2});
	c4 = MortonCode<2>::encode({3, 3});
	c5 = MortonCode<2>::encode({63, 2});

	CHECK(c1 == 0);
	CHECK(c2 == 0b11);
	CHECK(c3 == 0b1100);
	CHECK(c4 == 0b1111);
	CHECK(c5 == 0b101010101110);
}

TEST_CASE("MortonCode decode", "[MortonCode]")
{
	/**
	 * x = 143 : 0b 0000 1000 1111
	 * y = 542 : 0b 0010 0001 1110
	 * z = 962 : 0b 0011 1100 0010
	 */
	int64_t c = MortonCode<3>::encode({143, 542, 962});

	uint d1	 = MortonCode<3>::decode(c, 0);
	uint d2	 = MortonCode<3>::decode(c, 1);
	uint d3	 = MortonCode<3>::decode(c, 2);
	uint d4	 = MortonCode<3>::decode(c, 3);
	uint d5	 = MortonCode<3>::decode(c, 4);
	uint d6	 = MortonCode<3>::decode(c, 5);
	uint d7	 = MortonCode<3>::decode(c, 6);
	uint d8	 = MortonCode<3>::decode(c, 7);
	uint d9	 = MortonCode<3>::decode(c, 8);
	uint d10 = MortonCode<3>::decode(c, 9);

	CHECK(d1 == 0b100);
	CHECK(d2 == 0b111);
	CHECK(d3 == 0b110);
	CHECK(d4 == 0b110);
	CHECK(d5 == 0b010);
	CHECK(d6 == 0b000);
	CHECK(d7 == 0b001);
	CHECK(d8 == 0b101);
	CHECK(d9 == 0b001);
	CHECK(d10 == 0b011);

	c = MortonCode<2>::encode({143, 542});

	d1	= MortonCode<2>::decode(c, 0);
	d2	= MortonCode<2>::decode(c, 1);
	d3	= MortonCode<2>::decode(c, 2);
	d4	= MortonCode<2>::decode(c, 3);
	d5	= MortonCode<2>::decode(c, 4);
	d6	= MortonCode<2>::decode(c, 5);
	d7	= MortonCode<2>::decode(c, 6);
	d8	= MortonCode<2>::decode(c, 7);
	d9	= MortonCode<2>::decode(c, 8);
	d10 = MortonCode<2>::decode(c, 9);

	CHECK(d1 == 0b10);
	CHECK(d2 == 0b11);
	CHECK(d3 == 0b11);
	CHECK(d4 == 0b11);
	CHECK(d5 == 0b01);
	CHECK(d6 == 0b00);
	CHECK(d7 == 0b00);
	CHECK(d8 == 0b10);
	CHECK(d9 == 0b00);
	CHECK(d10 == 0b01);
}

TEST_CASE("Morton Code 3 splitBy3", "[MortonCode3]")
{
	uint64_t split_empty = MortonCode3::splitBy3(0);
	uint64_t split_inter = MortonCode3::splitBy3(0b11);
	uint64_t split_full	 = MortonCode3::splitBy3(0b11111111);

	CHECK(split_empty == 0);
	CHECK(split_inter == 011);
	CHECK(split_full == 011111111);
}

TEST_CASE("Morton Code 3 encode", "[MortonCode3]")
{
	uint64_t c1 = MortonCode3::encode(0, 0, 0);
	uint64_t c2 = MortonCode3::encode(1, 1, 1);
	uint64_t c3 = MortonCode3::encode(2, 2, 2);
	uint64_t c4 = MortonCode3::encode(3, 3, 3);
	uint64_t c5 = MortonCode3::encode(63, 0, 2);

	CHECK(c1 == 0);
	CHECK(c2 == 07);
	CHECK(c3 == 070);
	CHECK(c4 == 077);
	CHECK(c5 == 0444454);
}

TEST_CASE("Morton Code 3 decode", "[MortonCode3]")
{
	/**
		 * x = 143 : 0b 0000 1000 1111
		 * y = 542 : 0b 0010 0001 1110
		 * z = 962 : 0b 0011 1100 0010
		 */
	int64_t c = MortonCode3::encode(143, 542, 962);

	uint d1	 = MortonCode3::decode(c, 0);
	uint d2	 = MortonCode3::decode(c, 1);
	uint d3	 = MortonCode3::decode(c, 2);
	uint d4	 = MortonCode3::decode(c, 3);
	uint d5	 = MortonCode3::decode(c, 4);
	uint d6	 = MortonCode3::decode(c, 5);
	uint d7	 = MortonCode3::decode(c, 6);
	uint d8	 = MortonCode3::decode(c, 7);
	uint d9	 = MortonCode3::decode(c, 8);
	uint d10 = MortonCode3::decode(c, 9);

	CHECK(d1 == 0b100);
	CHECK(d2 == 0b111);
	CHECK(d3 == 0b110);
	CHECK(d4 == 0b110);
	CHECK(d5 == 0b010);
	CHECK(d6 == 0b000);
	CHECK(d7 == 0b001);
	CHECK(d8 == 0b101);
	CHECK(d9 == 0b001);
	CHECK(d10 == 0b011);
}

TEST_CASE("Morton Code 2 splitBy2", "[MortonCode2]")
{
	uint64_t split_empty = MortonCode2::splitBy2(0);
	uint64_t split_inter = MortonCode2::splitBy2(0b11);
	uint64_t split_full	 = MortonCode2::splitBy2(0b11111111);

	CHECK(split_empty == 0);
	CHECK(split_inter == 0b101);
	CHECK(split_full == 0b101010101010101);
}

TEST_CASE("Morton Code 2 encode", "[MortonCode2]")
{
	uint64_t c1 = MortonCode2::encode(0, 0);
	uint64_t c2 = MortonCode2::encode(1, 1);
	uint64_t c3 = MortonCode2::encode(2, 2);
	uint64_t c4 = MortonCode2::encode(3, 3);
	uint64_t c5 = MortonCode2::encode(63, 2);

	CHECK(c1 == 0);
	CHECK(c2 == 0b11);
	CHECK(c3 == 0b1100);
	CHECK(c4 == 0b1111);
	CHECK(c5 == 0b101010101110);
}

TEST_CASE("Morton Code 2 decode", "[MortonCode2]")
{
	/**
	 * x = 143 : 0b 0000 1000 1111
	 * y = 542 : 0b 0010 0001 1110
	 */
	int64_t c = MortonCode2::encode(143, 542);

	uint d1	 = MortonCode2::decode(c, 0);
	uint d2	 = MortonCode2::decode(c, 1);
	uint d3	 = MortonCode2::decode(c, 2);
	uint d4	 = MortonCode2::decode(c, 3);
	uint d5	 = MortonCode2::decode(c, 4);
	uint d6	 = MortonCode2::decode(c, 5);
	uint d7	 = MortonCode2::decode(c, 6);
	uint d8	 = MortonCode2::decode(c, 7);
	uint d9	 = MortonCode2::decode(c, 8);
	uint d10 = MortonCode2::decode(c, 9);

	CHECK(d1 == 0b10);
	CHECK(d2 == 0b11);
	CHECK(d3 == 0b11);
	CHECK(d4 == 0b11);
	CHECK(d5 == 0b01);
	CHECK(d6 == 0b00);
	CHECK(d7 == 0b00);
	CHECK(d8 == 0b10);
	CHECK(d9 == 0b00);
	CHECK(d10 == 0b01);
}

} // namespace QOTF