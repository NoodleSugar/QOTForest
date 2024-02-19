#pragma once

#include <qotf/utils/Types.hpp>

namespace qotf::internal
{

constexpr ushort kNullShift = 0;
constexpr ushort kByteSize	= 8;

constexpr byte kByteMask  = byte{0xFF};
constexpr byte kBitMask	  = byte{0x01};
constexpr byte kFullByte  = byte{0xFF};
constexpr byte kBit		  = byte{0x01};
constexpr byte kEmptyByte = byte{0x00};

namespace bitutils
{

inline constexpr bool isMultipleOfByteSize(size_t n) { return !static_cast<bool>(n & 0b111); }

inline constexpr size_t bitCount(size_t byteCount) { return byteCount << 3; }
inline constexpr size_t byteCount(size_t bitCount) { return (bitCount + kByteSize - 1) >> 3; }
inline constexpr size_t completeByteCount(size_t bitCount) { return bitCount >> 3; }

inline constexpr size_t bitIndex(size_t byteIndex) { return byteIndex << 3; }
inline constexpr size_t byteIndex(size_t bitIndex) { return bitIndex >> 3; }
inline constexpr size_t bitIndexAtByteStart(size_t bitIndex) { return bitIndex & ~(0b111); }

inline constexpr ushort bitIndexInsideByte(size_t bitIndex) { return bitIndex & 0b111; }
inline constexpr ushort leftShiftInsideByte(size_t bitIndex) { return bitIndexInsideByte(bitIndex); }
inline constexpr ushort rightShiftInsideByte(size_t bitIndex) { return kByteSize - 1 - bitIndexInsideByte(bitIndex); }

inline constexpr byte maskFirstBits(ushort bitCount) { return ~(kByteMask >> bitCount); }
inline constexpr byte maskLastBits(ushort bitCount) { return ~(kByteMask << bitCount); }

inline constexpr bool bitsAreInMiddleOfByte(size_t index, ushort count)
{
	const size_t leftmostBitIndex  = index;
	const size_t rightmostBitIndex = index + count;

	return count < kByteSize - 1 &&
		   bitutils::leftShiftInsideByte(leftmostBitIndex) != kNullShift &&
		   bitutils::rightShiftInsideByte(rightmostBitIndex) != kNullShift;
}

inline constexpr bool hasCompleteByte(size_t index, size_t count)
{
	return count >= (kByteSize + bitutils::leftShiftInsideByte(index));
}

} // namespace bitutils
} // namespace qotf::internal