#pragma once

#include <catch2/catch.hpp>

#include <qotf/internal/BitUtils.hpp>
#include <qotf/internal/BitVector.hpp>

namespace qotf::internal
{

TEST_CASE("BitVector::BitVector", "[BitVector]")
{
	SECTION("Empty constructor")
	{
		const size_t arraySize = 0;

		BitVector array;

		REQUIRE(arraySize == array.size());
	}

	SECTION("Constructor with size and empty bits")
	{
		const size_t arraySize = 24;

		BitVector array(arraySize);
		byte*	  bytes = array.data();

		REQUIRE(arraySize == array.size());
		REQUIRE(kEmptyByte == bytes[0]);
		REQUIRE(kEmptyByte == bytes[1]);
		REQUIRE(kEmptyByte == bytes[2]);
	}

	SECTION("Constructor with size and full bits")
	{
		const size_t arraySize = 24;

		BitVector array(arraySize, true);
		byte*	  bytes = array.data();

		REQUIRE(arraySize == array.size());
		REQUIRE(kFullByte == bytes[0]);
		REQUIRE(kFullByte == bytes[1]);
		REQUIRE(kFullByte == bytes[2]);
	}

	SECTION("Constructor with bytes")
	{
		const size_t arraySize = 16;

		BitVector array({kFullByte, kFullByte});
		byte*	  bytes = array.data();

		REQUIRE(arraySize == array.size());
		REQUIRE(kFullByte == bytes[0]);
		REQUIRE(kFullByte == bytes[1]);
	}
}

TEST_CASE("BitVector::resize", "[BitVector]")
{
	const size_t arraySize = 24;

	BitVector array(arraySize);

	SECTION("Lower size")
	{
		const size_t newArraySize = 18;

		array.resize(newArraySize);
		byte* bytes = array.data();

		REQUIRE(newArraySize == array.size());
		CHECK(kEmptyByte == bytes[0]);
		CHECK(kEmptyByte == bytes[1]);
		CHECK(kEmptyByte == bytes[2]);
	}

	SECTION("Higher size")
	{
		const size_t newArraySize = 30;

		array.resize(newArraySize);
		byte* bytes = array.data();

		REQUIRE(newArraySize == array.size());
		CHECK(kEmptyByte == bytes[0]);
		CHECK(kEmptyByte == bytes[1]);
		CHECK(kEmptyByte == bytes[2]);
		CHECK(kEmptyByte == bytes[3]);
	}
}

TEST_CASE("BitVector::reserve", "[BitVector]")
{
	const size_t arraycapacity = 18;

	BitVector array;

	array.reserve(arraycapacity);

	REQUIRE(0 == array.size());
}

TEST_CASE("BitVector::get", "[BitVector]")
{
	const size_t arraySize = 18;

	BitVector array;
}

TEST_CASE("BitVector::set", "[BitVector]")
{
}

TEST_CASE("BitVector::setMany", "[BitVector]")
{
}

TEST_CASE("BitVector::append", "[BitVector]")
{
}

TEST_CASE("BitVector::insert", "[BitVector]")
{
}

TEST_CASE("BitVector::remove", "[BitVector]")
{
}

TEST_CASE("BitVector::applyPattern", "[BitVector]")
{
	SECTION("With std::vector<byte>")
	{
	}

	SECTION("With BitVector")
	{
	}

	SECTION("With BitPattern")
	{
	}

	SECTION("With FixedBitPattern")
	{
	}
}

} // namespace qotf::internal