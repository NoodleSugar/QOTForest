#pragma once
#include <QOTF/bit_array.h>
#include <catch2/catch.hpp>

namespace QOTF
{
TEST_CASE("BitArray initialization", "[BitArray]")
{
	BitArray array(24);
	byte*	 bytes = array.getByteAccess();

	REQUIRE(array.getSize() == 24);
	REQUIRE((int)bytes[0] == 0);
	REQUIRE((int)bytes[1] == 0);
	REQUIRE((int)bytes[2] == 0);
}

TEST_CASE("BitArray resize", "[BitArray]")
{
	BitArray array(15);
	REQUIRE(array.getSize() == 15);

	SECTION("Resize upper")
	{
		array.resize(19);
		CHECK(array.getSize() == 19);
	}

	SECTION("Resize lower")
	{
		array.resize(10);
		CHECK(array.getSize() == 10);
	}

	SECTION("Resize to zero")
	{
		array.resize(0);
		CHECK(array.getSize() == 0);
	}
}

TEST_CASE("BitArray getting bits", "[BitArray]")
{
	BitArray array(8);
	byte*	 bytes = array.getByteAccess();

	bytes[0] = 0b01101101;

	CHECK(array.get(0) == 0);
	CHECK(array.get(1) == 1);
	CHECK(array.get(2) == 1);
	CHECK(array.get(3) == 0);
	CHECK(array.get(4) == 1);
	CHECK(array.get(5) == 1);
	CHECK(array.get(6) == 0);
	CHECK(array.get(7) == 1);
}

TEST_CASE("BitArray getting bytes", "[BitArray]")
{
	BitArray array(24);
	byte*	 bytes = array.getByteAccess();

	bytes[0] = 0b00001111;
	bytes[1] = 0b01011010;
	bytes[2] = 0b11111111;

	CHECK(array.getByteAtBit(0) == 0b00001111);
	CHECK(array.getByteAtBit(1) == 0b00011110);
	CHECK(array.getByteAtBit(4) == 0b11110101);
	CHECK(array.getByteAtBit(7) == 0b10101101);
	CHECK(array.getByteAtBit(8) == 0b01011010);
}

TEST_CASE("BitArray set bits", "[BitArray]")
{
	BitArray array(24);
	byte*	 bytes = array.getByteAccess();

	SECTION("1.1")
	{
		array.set(4, 4);
		CHECK((int)bytes[0] == 0b00001111);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b00000000);
	}

	SECTION("1.1 2")
	{
		array.set(6, 10);
		CHECK((int)bytes[0] == 0b00000011);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b00000000);
	}

	SECTION("1.1 2 3")
	{
		array.set(6, 12);
		CHECK((int)bytes[0] == 0b00000011);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b11000000);
	}

	SECTION("1.1 3")
	{
		array.set(7, 3);
		CHECK((int)bytes[0] == 0b00000001);
		CHECK((int)bytes[1] == 0b11000000);
		CHECK((int)bytes[2] == 0b00000000);
	}

	SECTION("1.2")
	{
		array.set(3, 4);
		CHECK((int)bytes[0] == 0b00011110);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b00000000);
	}

	SECTION("2")
	{
		array.set(8, 8);
		CHECK((int)bytes[0] == 0b00000000);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b00000000);
	}

	SECTION("2 3")
	{
		array.set(8, 10);
		CHECK((int)bytes[0] == 0b00000000);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b11000000);
	}

	SECTION("3")
	{
		array.set(0, 7);
		CHECK((int)bytes[0] == 0b11111110);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b00000000);
	}
}

TEST_CASE("BitArray unset bits", "[BitArray]")
{
	BitArray array(24);
	byte*	 bytes = array.getByteAccess();

	bytes[0] = 0b11111111;
	bytes[1] = 0b11111111;
	bytes[2] = 0b11111111;

	SECTION("1.1")
	{
		array.unset(4, 4);
		CHECK((int)bytes[0] == 0b11110000);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b11111111);
	}

	SECTION("1.1 2")
	{
		array.unset(6, 10);
		CHECK((int)bytes[0] == 0b11111100);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b11111111);
	}

	SECTION("1.1 2 3")
	{
		array.unset(6, 12);
		CHECK((int)bytes[0] == 0b11111100);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b00111111);
	}

	SECTION("1.1 3")
	{
		array.unset(7, 3);
		CHECK((int)bytes[0] == 0b11111110);
		CHECK((int)bytes[1] == 0b00111111);
		CHECK((int)bytes[2] == 0b11111111);
	}

	SECTION("1.2")
	{
		array.unset(3, 4);
		CHECK((int)bytes[0] == 0b11100001);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b11111111);
	}

	SECTION("2")
	{
		array.unset(8, 8);
		CHECK((int)bytes[0] == 0b11111111);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b11111111);
	}

	SECTION("2 3")
	{
		array.unset(8, 10);
		CHECK((int)bytes[0] == 0b11111111);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b00111111);
	}

	SECTION("3")
	{
		array.unset(0, 7);
		CHECK((int)bytes[0] == 0b00000001);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b11111111);
	}
}

TEST_CASE("BitArray set bytes", "BitArray")
{
	BitArray array(24);
	byte*	 bytes = array.getByteAccess();

	SECTION("Byte aligned")
	{
		array.setByteAtBit(0, 0b11111111);
		CHECK((int)bytes[0] == 0b11111111);
		CHECK((int)bytes[1] == 0b00000000);
		CHECK((int)bytes[2] == 0b00000000);
	}

	SECTION("Byte non aligned")
	{
		array.setByteAtBit(13, 0b11111111);
		CHECK((int)bytes[0] == 0b00000000);
		CHECK((int)bytes[1] == 0b00000111);
		CHECK((int)bytes[2] == 0b11111000);
	}
}

TEST_CASE("BitArray append bits", "BitArray")
{
	BitArray array(8);
	byte*	 bytes;

	SECTION("A byte of non null bits")
	{
		array.append(8, true);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 16);
		CHECK((int)bytes[0] == 0b00000000);
		CHECK((int)bytes[1] == 0b11111111);
	}

	SECTION("Greater than a byte of non null bits")
	{
		array.append(10, true);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 18);
		CHECK((int)bytes[0] == 0b00000000);
		CHECK((int)bytes[1] == 0b11111111);
		CHECK((int)bytes[2] == 0b11000000);
	}

	SECTION("Lesser than a byte of non null bits")
	{
		array.append(3, true);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 11);
		CHECK((int)bytes[0] == 0b00000000);
		CHECK((int)bytes[1] == 0b11100000);
	}
}

TEST_CASE("BitArray insert bits", "BitArray")
{
	BitArray array(26);
	byte*	 bytes = array.getByteAccess();

	bytes[0] = 0b11010110;
	bytes[1] = 0b10110101;
	bytes[2] = 0b10101101;
	bytes[3] = 0b01000000;

	SECTION("ShiftRifht 1.1 2.1 3")
	{
		array.insert(2, 4, true);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 30);
		CHECK((int)bytes[0] == 0b11111101);
		CHECK((int)bytes[1] == 0b01101011);
		CHECK((int)bytes[2] == 0b01011010);
		CHECK((int)bytes[3] == 0b11010100);
	}

	SECTION("ShiftRifht 1.1 2.2 3")
	{
		array.insert(4, 8, true);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 34);
		CHECK((int)bytes[0] == 0b11011111);
		CHECK((int)bytes[1] == 0b11110110);
		CHECK((int)bytes[2] == 0b10110101);
		CHECK((int)bytes[3] == 0b10101101);
		CHECK((int)bytes[4] == 0b01000000);
	}

	SECTION("ShiftRifht 1.2")
	{
		array.insert(20, 5, true);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 31);
		CHECK((int)bytes[0] == 0b11010110);
		CHECK((int)bytes[1] == 0b10110101);
		CHECK((int)bytes[2] == 0b10101111);
		CHECK((int)bytes[3] == 0b11101010);
	}
}

TEST_CASE("BitArray remove bits", "BitArray")
{
	BitArray array(26);
	byte*	 bytes = array.getByteAccess();

	bytes[0] = 0b11010110;
	bytes[1] = 0b10110101;
	bytes[2] = 0b10101101;
	bytes[3] = 0b01000000;

	SECTION("ShiftLeft 1.1 2.1 3")
	{
		array.remove(2, 4);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 22);
		CHECK((int)bytes[0] == 0b11101011);
		CHECK((int)bytes[1] == 0b01011010);
		CHECK((int)bytes[2] == 0b11010100);
	}

	SECTION("ShiftLeft 1.1 2.2 3")
	{
		array.remove(4, 8);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 18);
		CHECK((int)bytes[0] == 0b11010101);
		CHECK((int)bytes[1] == 0b10101101);
		CHECK((int)bytes[2] == 0b01000000);
	}

	SECTION("ShiftLeft 1.2")
	{
		array.remove(20, 5);
		bytes = array.getByteAccess();
		CHECK(array.getSize() == 21);
		CHECK((int)bytes[0] == 0b11010110);
		CHECK((int)bytes[1] == 0b10110101);
		CHECK((int)bytes[2] == 0b10101000);
	}
}

} // namespace QOTF