#pragma once

#include <catch2/catch.hpp>

#include <qotf/internal/ByteHelper.hpp>

namespace qotf::internal
{

TEST_CASE("ByteHelper::getByte", "[ByteHelper]")
{
	struct TestData
	{
		size_t index;
		byte   expectedByte;
	};

	byte bytes[] = {
	 byte{0b1010'0101},
	 byte{0b0101'1010}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{0, byte{0b1010'0101}},
	 TestData{1, byte{0b0100'1010}},
	 TestData{2, byte{0b1001'0101}},
	 TestData{3, byte{0b0010'1010}},
	 TestData{4, byte{0b0101'0101}},
	 TestData{5, byte{0b1010'1011}},
	 TestData{6, byte{0b0101'0110}},
	 TestData{7, byte{0b1010'1101}},
	 TestData{8, byte{0b0101'1010}});

	CHECK(data.expectedByte == helper.getByte(data.index));
}

TEST_CASE("ByteHelper::getBit", "[ByteHelper]")
{
	struct TestData
	{
		size_t index;
		bool   expectedBit;
	};

	byte bytes[] = {
	 byte{0b1010'0101}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{0, true},
	 TestData{1, false},
	 TestData{2, true},
	 TestData{3, false},
	 TestData{4, false},
	 TestData{5, true},
	 TestData{6, false},
	 TestData{7, true});

	CHECK(data.expectedBit == helper.getBit(data.index));
}

TEST_CASE("ByteHelper::setByte", "[ByteHelper]")
{
	struct TestData
	{
		size_t index;
		byte   expectedByte0;
		byte   expectedByte1;
	};

	SECTION("Empty bytes")
	{
		byte bytes[] = {
		 byte{0b0000'0000},
		 byte{0b0000'0000}};

		ByteHelper helper(bytes);

		SECTION("Set full byte")
		{
			const byte srcByte = byte{0b01111'1111};

			auto data = GENERATE(
			 TestData{0, byte{0b1111'1111}, byte{0b0000'0000}},
			 TestData{1, byte{0b0111'1111}, byte{0b1000'0000}},
			 TestData{2, byte{0b0011'1111}, byte{0b1100'0000}},
			 TestData{3, byte{0b0001'1111}, byte{0b1110'0000}},
			 TestData{4, byte{0b0000'1111}, byte{0b1111'0000}},
			 TestData{5, byte{0b0000'0111}, byte{0b1111'1000}},
			 TestData{6, byte{0b0000'0011}, byte{0b1111'1100}},
			 TestData{7, byte{0b0000'0001}, byte{0b1111'1110}},
			 TestData{8, byte{0b0000'0000}, byte{0b1111'1111}});

			helper.setByte(data.index, srcByte);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
		}

		SECTION("Set not full byte")
		{
			const byte srcByte = byte{0b1010'1010};

			auto data = GENERATE(
			 TestData{0, byte{0b1010'1010}, byte{0b0000'0000}},
			 TestData{1, byte{0b0101'0101}, byte{0b0000'0000}},
			 TestData{2, byte{0b0010'1010}, byte{0b1000'0000}},
			 TestData{3, byte{0b0001'0101}, byte{0b0100'0000}},
			 TestData{4, byte{0b0000'1010}, byte{0b1010'0000}},
			 TestData{5, byte{0b0000'0101}, byte{0b0101'0000}},
			 TestData{6, byte{0b0000'0010}, byte{0b1010'1000}},
			 TestData{7, byte{0b0000'0001}, byte{0b0101'0100}},
			 TestData{8, byte{0b0000'0000}, byte{0b1010'1010}});

			helper.setByte(data.index, srcByte);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
		}
	}

	SECTION("Full bytes")
	{
		byte bytes[] = {
		 byte{0b1111'1111},
		 byte{0b1111'1111}};

		ByteHelper helper(bytes);

		SECTION("Set empty byte")
		{
			const byte srcByte = byte{0b0000'0000};

			auto data = GENERATE(
			 TestData{0, byte{0b0000'0000}, byte{0b1111'1111}},
			 TestData{1, byte{0b1000'0000}, byte{0b0111'1111}},
			 TestData{2, byte{0b1100'0000}, byte{0b0011'1111}},
			 TestData{3, byte{0b1110'0000}, byte{0b0001'1111}},
			 TestData{4, byte{0b1111'0000}, byte{0b0000'1111}},
			 TestData{5, byte{0b1111'1000}, byte{0b0000'0111}},
			 TestData{6, byte{0b1111'1100}, byte{0b0000'0011}},
			 TestData{7, byte{0b1111'1110}, byte{0b0000'0001}},
			 TestData{8, byte{0b1111'1111}, byte{0b0000'0000}});

			helper.setByte(data.index, srcByte);
			CHECK(bytes[0] == data.expectedByte0);
			CHECK(bytes[1] == data.expectedByte1);
		}

		SECTION("Set not empty byte")
		{
			const byte srcByte = byte{0b1010'1010};

			auto data = GENERATE(
			 TestData{0, byte{0b1010'1010}, byte{0b1111'1111}},
			 TestData{1, byte{0b1101'0101}, byte{0b0111'1111}},
			 TestData{2, byte{0b1110'1010}, byte{0b1011'1111}},
			 TestData{3, byte{0b1111'0101}, byte{0b0101'1111}},
			 TestData{4, byte{0b1111'1010}, byte{0b1010'1111}},
			 TestData{5, byte{0b1111'1101}, byte{0b0101'0111}},
			 TestData{6, byte{0b1111'1110}, byte{0b1010'1011}},
			 TestData{7, byte{0b1111'1111}, byte{0b0101'0101}},
			 TestData{8, byte{0b1111'1111}, byte{0b1010'1010}});

			helper.setByte(data.index, srcByte);
			CHECK(bytes[0] == data.expectedByte0);
			CHECK(bytes[1] == data.expectedByte1);
		}
	}
}

TEST_CASE("ByteHelper::setBytes", "[ByteHelper]")
{
	struct TestData
	{
		size_t index;
		size_t byteCount;
		byte   expectedByte0;
		byte   expectedByte1;
		byte   expectedByte2;
	};

	SECTION("Empty bytes")
	{
		byte bytes[] = {
		 byte{0b0000'0000},
		 byte{0b0000'0000},
		 byte{0b0000'0000}};

		ByteHelper helper(bytes);

		SECTION("Set full bytes")
		{
			const byte srcBytes[] = {
			 byte{0b01111'1111},
			 byte{0b01111'1111}};

			auto data = GENERATE(
			 TestData{0, 2, byte{0b1111'1111}, byte{0b1111'1111}, byte{0b0000'0000}},
			 TestData{1, 2, byte{0b0111'1111}, byte{0b1111'1111}, byte{0b1000'0000}},
			 TestData{2, 2, byte{0b0011'1111}, byte{0b1111'1111}, byte{0b1100'0000}},
			 TestData{3, 2, byte{0b0001'1111}, byte{0b1111'1111}, byte{0b1110'0000}},
			 TestData{4, 2, byte{0b0000'1111}, byte{0b1111'1111}, byte{0b1111'0000}},
			 TestData{5, 2, byte{0b0000'0111}, byte{0b1111'1111}, byte{0b1111'1000}},
			 TestData{6, 2, byte{0b0000'0011}, byte{0b1111'1111}, byte{0b1111'1100}},
			 TestData{7, 2, byte{0b0000'0001}, byte{0b1111'1111}, byte{0b1111'1110}},
			 TestData{8, 2, byte{0b0000'0000}, byte{0b1111'1111}, byte{0b1111'1111}});

			helper.setBytes(data.index, srcBytes, data.byteCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
			CHECK(data.expectedByte2 == bytes[2]);
		}

		SECTION("Set not full bytes")
		{
			const byte srcBytes[] = {
			 byte{0b1010'1010},
			 byte{0b0101'0101}};

			auto data = GENERATE(
			 TestData{0, 2, byte{0b1010'1010}, byte{0b0101'0101}, byte{0b0000'0000}},
			 TestData{1, 2, byte{0b0101'0101}, byte{0b0010'1010}, byte{0b1000'0000}},
			 TestData{2, 2, byte{0b0010'1010}, byte{0b1001'0101}, byte{0b0100'0000}},
			 TestData{3, 2, byte{0b0001'0101}, byte{0b0100'1010}, byte{0b1010'0000}},
			 TestData{4, 2, byte{0b0000'1010}, byte{0b1010'0101}, byte{0b0101'0000}},
			 TestData{5, 2, byte{0b0000'0101}, byte{0b0101'0010}, byte{0b1010'1000}},
			 TestData{6, 2, byte{0b0000'0010}, byte{0b1010'1001}, byte{0b0101'0100}},
			 TestData{7, 2, byte{0b0000'0001}, byte{0b0101'0100}, byte{0b1010'1010}},
			 TestData{8, 2, byte{0b0000'0000}, byte{0b1010'1010}, byte{0b0101'0101}});

			helper.setBytes(data.index, srcBytes, data.byteCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
			CHECK(data.expectedByte2 == bytes[2]);
		}
	}

	SECTION("Full bytes")
	{
		byte bytes[] = {
		 byte{0b1111'1111},
		 byte{0b1111'1111},
		 byte{0b1111'1111}};

		ByteHelper helper(bytes);

		SECTION("Set empty bytes")
		{
			const byte srcBytes[] = {
			 byte{0b0000'0000},
			 byte{0b0000'0000}};

			auto data = GENERATE(
			 TestData{0, 2, byte{0b0000'0000}, byte{0b0000'0000}, byte{0b1111'1111}},
			 TestData{1, 2, byte{0b1000'0000}, byte{0b0000'0000}, byte{0b0111'1111}},
			 TestData{2, 2, byte{0b1100'0000}, byte{0b0000'0000}, byte{0b0011'1111}},
			 TestData{3, 2, byte{0b1110'0000}, byte{0b0000'0000}, byte{0b0001'1111}},
			 TestData{4, 2, byte{0b1111'0000}, byte{0b0000'0000}, byte{0b0000'1111}},
			 TestData{5, 2, byte{0b1111'1000}, byte{0b0000'0000}, byte{0b0000'0111}},
			 TestData{6, 2, byte{0b1111'1100}, byte{0b0000'0000}, byte{0b0000'0011}},
			 TestData{7, 2, byte{0b1111'1110}, byte{0b0000'0000}, byte{0b0000'0001}},
			 TestData{8, 2, byte{0b1111'1111}, byte{0b0000'0000}, byte{0b0000'0000}});

			helper.setBytes(data.index, srcBytes, data.byteCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
			CHECK(data.expectedByte2 == bytes[2]);
		}

		SECTION("Set not full bytes")
		{
			const byte srcBytes[] = {
			 byte{0b1010'1010},
			 byte{0b0101'0101}};

			auto data = GENERATE(
			 TestData{0, 2, byte{0b1010'1010}, byte{0b0101'0101}, byte{0b1111'1111}},
			 TestData{1, 2, byte{0b1101'0101}, byte{0b0010'1010}, byte{0b1111'1111}},
			 TestData{2, 2, byte{0b1110'1010}, byte{0b1001'0101}, byte{0b0111'1111}},
			 TestData{3, 2, byte{0b1111'0101}, byte{0b0100'1010}, byte{0b1011'1111}},
			 TestData{4, 2, byte{0b1111'1010}, byte{0b1010'0101}, byte{0b0101'1111}},
			 TestData{5, 2, byte{0b1111'1101}, byte{0b0101'0010}, byte{0b1010'1111}},
			 TestData{6, 2, byte{0b1111'1110}, byte{0b1010'1001}, byte{0b0101'0111}},
			 TestData{7, 2, byte{0b1111'1111}, byte{0b0101'0100}, byte{0b1010'1011}},
			 TestData{8, 2, byte{0b1111'1111}, byte{0b1010'1010}, byte{0b0101'0101}});

			helper.setBytes(data.index, srcBytes, data.byteCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
			CHECK(data.expectedByte2 == bytes[2]);
		}
	}
}

TEST_CASE("ByteHelper::setBytePart", "[ByteHelper]")
{
	struct TestData
	{
		size_t index;
		ushort bitCount;
		byte   expectedByte0;
		byte   expectedByte1;
	};

	SECTION("Empty bytes")
	{
		byte bytes[] = {
		 byte{0b0000'0000},
		 byte{0b0000'0000}};

		ByteHelper helper(bytes);

		SECTION("Set full byte part")
		{
			const byte srcByte = byte{0b1111'1111};

			auto data = GENERATE(
			 TestData{0, 6, byte{0b1111'1100}, byte{0b0000'0000}},
			 TestData{1, 6, byte{0b0111'1110}, byte{0b0000'0000}},
			 TestData{2, 6, byte{0b0011'1111}, byte{0b0000'0000}},
			 TestData{3, 6, byte{0b0001'1111}, byte{0b1000'0000}},
			 TestData{4, 6, byte{0b0000'1111}, byte{0b1100'0000}},
			 TestData{5, 6, byte{0b0000'0111}, byte{0b1110'0000}},
			 TestData{6, 6, byte{0b0000'0011}, byte{0b1111'0000}},
			 TestData{7, 6, byte{0b0000'0001}, byte{0b1111'1000}},
			 TestData{8, 6, byte{0b0000'0000}, byte{0b1111'1100}});

			helper.setBytePart(data.index, srcByte, data.bitCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
		}

		SECTION("Set full byte part")
		{
			const byte srcByte = byte{0b0101'0100};

			auto data = GENERATE(
			 TestData{0, 6, byte{0b0101'0100}, byte{0b0000'0000}},
			 TestData{1, 6, byte{0b0010'1010}, byte{0b0000'0000}},
			 TestData{2, 6, byte{0b0001'0101}, byte{0b0000'0000}},
			 TestData{3, 6, byte{0b0000'1010}, byte{0b1000'0000}},
			 TestData{4, 6, byte{0b0000'0101}, byte{0b0100'0000}},
			 TestData{5, 6, byte{0b0000'0010}, byte{0b1010'0000}},
			 TestData{6, 6, byte{0b0000'0001}, byte{0b0101'0000}},
			 TestData{7, 6, byte{0b0000'0000}, byte{0b1010'1000}},
			 TestData{8, 6, byte{0b0000'0000}, byte{0b0101'0100}});

			helper.setBytePart(data.index, srcByte, data.bitCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
		}
	}

	SECTION("Full bytes")
	{
		byte bytes[] = {
		 byte{0b1111'1111},
		 byte{0b1111'1111}};

		ByteHelper helper(bytes);

		SECTION("Set empty byte part")
		{
			const byte srcByte = byte{0b0000'0000};

			auto data = GENERATE(
			 TestData{0, 6, byte{0b0000'0011}, byte{0b1111'1111}},
			 TestData{1, 6, byte{0b1000'0001}, byte{0b1111'1111}},
			 TestData{2, 6, byte{0b1100'0000}, byte{0b1111'1111}},
			 TestData{3, 6, byte{0b1110'0000}, byte{0b0111'1111}},
			 TestData{4, 6, byte{0b1111'0000}, byte{0b0011'1111}},
			 TestData{5, 6, byte{0b1111'1000}, byte{0b0001'1111}},
			 TestData{6, 6, byte{0b1111'1100}, byte{0b0000'1111}},
			 TestData{7, 6, byte{0b1111'1110}, byte{0b0000'0111}},
			 TestData{8, 6, byte{0b1111'1111}, byte{0b0000'0011}});

			helper.setBytePart(data.index, srcByte, data.bitCount);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
		}

		SECTION("Set full byte part")
		{
			const byte srcByte = byte{0b1010'1000};

			auto data = GENERATE(
			 TestData{0, 6, byte{0b1010'1011}, byte{0b1111'1111}},
			 TestData{1, 6, byte{0b1101'0101}, byte{0b1111'1111}},
			 TestData{2, 6, byte{0b1110'1010}, byte{0b1111'1111}},
			 TestData{3, 6, byte{0b1111'0101}, byte{0b0111'1111}},
			 TestData{4, 6, byte{0b1111'1010}, byte{0b1011'1111}},
			 TestData{5, 6, byte{0b1111'1101}, byte{0b0101'1111}},
			 TestData{6, 6, byte{0b1111'1110}, byte{0b1010'1111}},
			 TestData{7, 6, byte{0b1111'1111}, byte{0b0101'0111}},
			 TestData{8, 6, byte{0b1111'1111}, byte{0b1010'1011}});

			helper.setBytePart(data.index, srcByte, 6);
			CHECK(data.expectedByte0 == bytes[0]);
			CHECK(data.expectedByte1 == bytes[1]);
		}
	}
}

TEST_CASE("ByteHelper::copyBytes", "[ByteHelper]")
{
	struct TestData
	{
		size_t srcByteIndex;
		size_t dstBitIndex;
		size_t byteCount;
		byte   expectedByte0;
		byte   expectedByte1;
		byte   expectedByte2;
	};

	byte bytes[] = {
	 byte{0b1010'0101},
	 byte{0b0101'1010},
	 byte{0b0000'0000}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{0, 0, 2, byte{0b1010'0101}, byte{0b0101'1010}, byte{0b0000'0000}},
	 TestData{0, 1, 2, byte{0b1101'0010}, byte{0b1010'1101}, byte{0b0000'0000}},
	 TestData{0, 2, 2, byte{0b1010'1001}, byte{0b0101'0110}, byte{0b1000'0000}},
	 TestData{0, 3, 2, byte{0b1011'0100}, byte{0b1010'1011}, byte{0b0100'0000}},
	 TestData{0, 4, 2, byte{0b1010'1010}, byte{0b0101'0101}, byte{0b1010'0000}},
	 TestData{0, 5, 2, byte{0b1010'0101}, byte{0b0010'1010}, byte{0b1101'0000}},
	 TestData{0, 6, 2, byte{0b1010'0110}, byte{0b1001'0101}, byte{0b0110'1000}},
	 TestData{0, 7, 2, byte{0b1010'0101}, byte{0b0100'1010}, byte{0b1011'0100}},
	 TestData{0, 8, 2, byte{0b1010'0101}, byte{0b1010'0101}, byte{0b0101'1010}});

	helper.copyBytes<Direction::ToRight>(data.dstBitIndex, data.srcByteIndex, data.byteCount);
	CHECK(data.expectedByte0 == bytes[0]);
	CHECK(data.expectedByte1 == bytes[1]);
	CHECK(data.expectedByte2 == bytes[2]);
}

TEST_CASE("ByteHelper::copyByte", "[ByteHelper]")
{
	struct TestData
	{
		size_t srcByteIndex;
		size_t dstBitIndex;
		byte   expectedByte0;
		byte   expectedByte1;
	};

	byte bytes[] = {
	 byte{0b1010'0101},
	 byte{0b0000'0000}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{0, 0, byte{0b1010'0101}, byte{0b0000'0000}},
	 TestData{0, 1, byte{0b1101'0010}, byte{0b1000'0000}},
	 TestData{0, 2, byte{0b1010'1001}, byte{0b0100'0000}},
	 TestData{0, 3, byte{0b1011'0100}, byte{0b1010'0000}},
	 TestData{0, 4, byte{0b1010'1010}, byte{0b0101'0000}},
	 TestData{0, 5, byte{0b1010'0101}, byte{0b0010'1000}},
	 TestData{0, 6, byte{0b1010'0110}, byte{0b1001'0100}},
	 TestData{0, 7, byte{0b1010'0101}, byte{0b0100'1010}},
	 TestData{0, 8, byte{0b1010'0101}, byte{0b1010'0101}});

	helper.copyByte(data.dstBitIndex, data.srcByteIndex);
	CHECK(data.expectedByte0 == bytes[0]);
	CHECK(data.expectedByte1 == bytes[1]);
}

TEST_CASE("ByteHelper::copyByteStart", "[ByteHelper]")
{
	struct TestData
	{
		size_t srcByteIndex;
		size_t dstBitIndex;
		ushort bitCount;
		byte   expectedByte0;
		byte   expectedByte1;
	};

	byte bytes[] = {
	 byte{0b1010'0101},
	 byte{0b0000'0000}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{0, 0, 5, byte{0b1010'0101}, byte{0b0000'0000}},
	 TestData{0, 1, 5, byte{0b1101'0001}, byte{0b0000'0000}},
	 TestData{0, 2, 5, byte{0b1010'1001}, byte{0b0000'0000}},
	 TestData{0, 3, 5, byte{0b1011'0100}, byte{0b0000'0000}},
	 TestData{0, 4, 5, byte{0b1010'1010}, byte{0b0000'0000}},
	 TestData{0, 5, 5, byte{0b1010'0101}, byte{0b0000'0000}},
	 TestData{0, 6, 5, byte{0b1010'0110}, byte{0b1000'0000}},
	 TestData{0, 7, 5, byte{0b1010'0101}, byte{0b0100'0000}},
	 TestData{0, 8, 5, byte{0b1010'0101}, byte{0b1010'0000}});

	helper.copyByteStart(data.dstBitIndex, data.srcByteIndex, data.bitCount);
	CHECK(data.expectedByte0 == bytes[0]);
	CHECK(data.expectedByte1 == bytes[1]);
}

TEST_CASE("ByteHelper::copyByteEnd", "[ByteHelper]")
{
	struct TestData
	{
		size_t srcByteIndex;
		size_t dstBitIndex;
		ushort bitCount;
		byte   expectedByte0;
		byte   expectedByte1;
	};

	byte bytes[] = {
	 byte{0b1010'0101},
	 byte{0b0000'0000}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{0, 0, 5, byte{0b0010'1101}, byte{0b0000'0000}},
	 TestData{0, 1, 5, byte{0b1001'0101}, byte{0b0000'0000}},
	 TestData{0, 2, 5, byte{0b1000'1011}, byte{0b0000'0000}},
	 TestData{0, 3, 5, byte{0b1010'0101}, byte{0b0000'0000}},
	 TestData{0, 4, 5, byte{0b1010'0010}, byte{0b1000'0000}},
	 TestData{0, 5, 5, byte{0b1010'0001}, byte{0b0100'0000}},
	 TestData{0, 6, 5, byte{0b1010'0100}, byte{0b1010'0000}},
	 TestData{0, 7, 5, byte{0b1010'0100}, byte{0b0101'0000}},
	 TestData{0, 8, 5, byte{0b1010'0101}, byte{0b0010'1000}});

	helper.copyByteEnd(data.dstBitIndex, data.srcByteIndex, data.bitCount);
	CHECK(data.expectedByte0 == bytes[0]);
	CHECK(data.expectedByte1 == bytes[1]);
}

TEST_CASE("ByteHelper::copyByteMiddle", "[ByteHelper]")
{
	struct TestData
	{
		size_t srcBitIndex;
		size_t dstBitIndex;
		ushort bitCount;
		byte   expectedByte0;
		byte   expectedByte1;
	};

	byte bytes[] = {
	 byte{0b1010'0101},
	 byte{0b0000'0000}};

	ByteHelper helper(bytes);

	auto data = GENERATE(
	 TestData{2, 0, 4, byte{0b1001'0101}, byte{0b0000'0000}},
	 TestData{2, 1, 4, byte{0b1100'1101}, byte{0b0000'0000}},
	 TestData{2, 2, 4, byte{0b1010'0101}, byte{0b0000'0000}},
	 TestData{2, 3, 4, byte{0b1011'0011}, byte{0b0000'0000}},
	 TestData{2, 4, 4, byte{0b1010'1001}, byte{0b0000'0000}},
	 TestData{2, 5, 4, byte{0b1010'0100}, byte{0b1000'0000}},
	 TestData{2, 6, 4, byte{0b1010'0110}, byte{0b0100'0000}},
	 TestData{2, 7, 4, byte{0b1010'0101}, byte{0b0010'0000}},
	 TestData{2, 8, 4, byte{0b1010'0101}, byte{0b1001'0000}});

	helper.copyByteMiddle(data.dstBitIndex, data.srcBitIndex, data.bitCount);
	CHECK(data.expectedByte0 == bytes[0]);
	CHECK(data.expectedByte1 == bytes[1]);
}

TEST_CASE("ByteHelper::shiftBits", "[ByteHelper]")
{
	struct TestData
	{
		size_t index;
		size_t count;
		size_t shift;
		byte   expectedByte0;
		byte   expectedByte1;
		byte   expectedByte2;
	};

	SECTION("Shift left")
	{
		byte bytes[] = {
		 byte{0b1010'0101},
		 byte{0b1111'1111},
		 byte{0b1010'0101}};

		ByteHelper helper(bytes);

		auto data = GENERATE(
		 TestData{4, 2, 2, byte{0b1001'0101}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 2, 4, byte{0b0110'0101}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 4, 2, byte{0b1001'0101}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 4, 4, byte{0b0101'0101}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 8, 2, byte{0b1001'0111}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 8, 4, byte{0b0101'1111}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 12, 2, byte{0b1001'0111}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 12, 4, byte{0b0101'1111}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{4, 16, 2, byte{0b1001'0111}, byte{0b1111'1110}, byte{0b1010'0101}},
		 TestData{4, 16, 4, byte{0b0101'1111}, byte{0b1111'1010}, byte{0b1010'0101}},
		 TestData{4, 20, 2, byte{0b1001'0111}, byte{0b1111'1110}, byte{0b1001'0101}},
		 TestData{4, 20, 4, byte{0b0101'1111}, byte{0b1111'1010}, byte{0b0101'0101}});

		helper.shiftBits<Direction::ToLeft>(data.index, data.count, data.shift);
		CHECK(data.expectedByte0 == bytes[0]);
		CHECK(data.expectedByte1 == bytes[1]);
		CHECK(data.expectedByte2 == bytes[2]);
	}

	SECTION("Shift right")
	{
		byte bytes[] = {
		 byte{0b1010'0101},
		 byte{0b1111'1111},
		 byte{0b1010'0101}};

		ByteHelper helper(bytes);

		auto data = GENERATE(
		 TestData{0, 2, 2, byte{0b1010'0101}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{0, 2, 4, byte{0b1010'1001}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{0, 4, 2, byte{0b1010'1001}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{0, 4, 4, byte{0b1010'1010}, byte{0b1111'1111}, byte{0b1010'0101}},
		 TestData{0, 8, 2, byte{0b1010'1001}, byte{0b0111'1111}, byte{0b1010'0101}},
		 TestData{0, 8, 4, byte{0b1010'1010}, byte{0b0101'1111}, byte{0b1010'0101}},
		 TestData{0, 12, 2, byte{0b1010'1001}, byte{0b0111'1111}, byte{0b1010'0101}},
		 TestData{0, 12, 4, byte{0b1010'1010}, byte{0b0101'1111}, byte{0b1010'0101}},
		 TestData{0, 16, 2, byte{0b1010'1001}, byte{0b0111'1111}, byte{0b1110'0101}},
		 TestData{0, 16, 4, byte{0b1010'1010}, byte{0b0101'1111}, byte{0b1111'0101}},
		 TestData{0, 20, 2, byte{0b1010'1001}, byte{0b0111'1111}, byte{0b1110'1001}},
		 TestData{0, 20, 4, byte{0b1010'1010}, byte{0b0101'1111}, byte{0b1111'1010}});

		helper.shiftBits<Direction::ToRight>(data.index, data.count, data.shift);
		CHECK(data.expectedByte0 == bytes[0]);
		CHECK(data.expectedByte1 == bytes[1]);
		CHECK(data.expectedByte2 == bytes[2]);
	}
}

} // namespace qotf::internal