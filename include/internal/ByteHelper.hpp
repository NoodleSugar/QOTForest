#pragma once

#include <qotf/internal/BitUtils.hpp>
#include <qotf/utils/Types.hpp>

#include <cstring>

namespace qotf::internal
{

enum class Direction
{
	ToLeft,
	ToRight
};

namespace utils
{

template<Direction direction>
inline constexpr bool isLeft()
{
	return direction == Direction::ToLeft;
}

template<Direction direction>
inline constexpr bool isRight()
{
	return direction == Direction::ToRight;
}

template<Direction direction>
inline size_t byteIndexDiff(size_t dstByteIndex, size_t srcByteIndex)
{
	if constexpr(utils::isLeft<direction>())
		return srcByteIndex - dstByteIndex;
	else
		return dstByteIndex - srcByteIndex;
}

template<Direction direction>
inline size_t shiftBitIndex(size_t index, size_t shift)
{
	if constexpr(utils::isLeft<direction>())
		return index - shift;
	else
		return index + shift;
}

} // namespace utils

template<class ByteArray>
class ByteHelper
{
public:
	explicit ByteHelper(ByteArray& rBytes) :
		m_rBytes(rBytes) {}

	inline bool getBit(size_t index) const;
	template<bool>
	inline void setBit(size_t index);
	template<bool>
	inline void setBits(size_t index, size_t count);

	inline byte getByte(size_t bitIndex) const;
	inline void setByte(size_t dstBitIndex, byte srcByte);
	inline void setBytes(size_t dstBitIndex, const byte srcBytes[], size_t count);
	inline void setBytePart(size_t dstBitIndex, byte srcByte, ushort bitCount);

	inline void copyByte(size_t dstBitIndex, size_t srcByteIndex);
	template<Direction>
	inline void copyBytes(size_t dstBitIndex, size_t srcByteIndex, size_t count);
	inline void copyByteStart(size_t dstBitIndex, size_t srcByteIndex, ushort bitCount);
	inline void copyByteEnd(size_t dstBitIndex, size_t srcByteIndex, ushort bitCount);
	inline void copyByteMiddle(size_t dstBitIndex, size_t srcBitIndex, ushort bitCount);

	template<Direction>
	inline void shiftBits(size_t index, size_t count, size_t shift);

private:
	template<bool>
	inline void setBitsLeftByteEnd(size_t index, size_t count);
	template<bool>
	inline void setBitsMiddleBytes(size_t index, size_t count);
	template<bool>
	inline void setBitsRightByteStart(size_t index, size_t count);

	template<Direction>
	inline void shiftBitsLeftByteEnd(size_t index, size_t count, size_t shift);
	template<Direction>
	inline void shiftBitsMiddleBytes(size_t index, size_t count, size_t shift);
	template<Direction>
	inline void shiftBitsRightByteStart(size_t index, size_t count, size_t shift);

	ByteArray& m_rBytes;
};

template<class ByteArray>
inline bool ByteHelper<ByteArray>::getBit(size_t index) const
{
	const size_t byteIndex	= bitutils::byteIndex(index);
	const ushort rightShift = bitutils::rightShiftInsideByte(index);
	const byte	 result		= m_rBytes[byteIndex] >> rightShift;

	return static_cast<bool>(result & kBitMask);
}

template<class ByteArray>
template<bool bit>
inline void ByteHelper<ByteArray>::setBit(size_t index)
{
	const size_t byteIndex	= bitutils::byteIndex(index);
	const ushort rightShift = bitutils::rightShiftInsideByte(index);
	const byte	 bitInByte	= kBit << rightShift;

	if constexpr(bit)
		m_rBytes[byteIndex] |= bitInByte;
	else
		m_rBytes[byteIndex] &= ~(bitInByte);
}

template<class ByteArray>
template<bool bit>
inline void ByteHelper<ByteArray>::setBits(size_t index, size_t count)
{
	setBitsLeftByteEnd<bit>(index, count);
	setBitsMiddleBytes<bit>(index, count);
	setBitsRightByteStart<bit>(index, count);
}

template<class ByteArray>
template<bool bit>
inline void ByteHelper<ByteArray>::setBitsLeftByteEnd(size_t index, size_t count)
{
	const size_t leftmostBitIndex = index;

	if(bitutils::leftShiftInsideByte(leftmostBitIndex) != 0)
	{
		const ushort bitCount = bitutils::rightShiftInsideByte(leftmostBitIndex) + 1;

		setBytePart(leftmostBitIndex, kFullByte, bitCount);
	}
}

template<class ByteArray>
template<bool bit>
inline void ByteHelper<ByteArray>::setBitsMiddleBytes(size_t index, size_t count)
{
	const size_t leftmostBitIndex  = index;
	const size_t rightmostBitIndex = index + count - 1;

	const ushort maxBitIndexInByte		= kByteSize - 1;
	const size_t leftmostFullByteIndex	= bitutils::byteIndex(leftmostBitIndex + maxBitIndexInByte);
	const size_t rightmostFullByteIndex = bitutils::byteIndex(rightmostBitIndex - maxBitIndexInByte);

	const size_t byteCount = rightmostFullByteIndex - leftmostFullByteIndex + 1;

	const size_t dstBitIndex = bitutils::bitIndex(leftmostFullByteIndex);

	for(size_t i = 0; i < byteCount; i++)
		setByte(dstBitIndex + i * kByteSize, kFullByte);
}

template<class ByteArray>
template<bool bit>
inline void ByteHelper<ByteArray>::setBitsRightByteStart(size_t index, size_t count)
{
	const size_t rightmostBitIndex = index + count - 1;

	if(bitutils::rightShiftInsideByte(rightmostBitIndex) != 0)
	{
		const size_t dstBitIndex = bitutils::bitIndexAtByteStart(rightmostBitIndex);

		setBytePart(dstBitIndex, kFullByte, count);
	}
}

template<class ByteArray>
inline byte ByteHelper<ByteArray>::getByte(size_t bitIndex) const
{
	const size_t byteIndex		   = bitutils::byteIndex(bitIndex);
	const size_t leftmostBitIndex  = bitIndex;
	const size_t rightmostBitIndex = bitIndex + kByteSize - 1;
	const ushort leftShift		   = bitutils::leftShiftInsideByte(leftmostBitIndex);
	const ushort rightShift		   = bitutils::rightShiftInsideByte(rightmostBitIndex);

	if(leftShift == kNullShift)
		return m_rBytes[byteIndex];

	const byte start = m_rBytes[byteIndex] << leftShift;
	const byte end	 = m_rBytes[byteIndex + 1] >> rightShift;

	return start | end;
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::setByte(size_t dstBitIndex, byte srcByte)
{
	const size_t dstByteIndex  = bitutils::byteIndex(dstBitIndex);
	const ushort dstLeftShift  = bitutils::leftShiftInsideByte(dstBitIndex);
	const ushort dstRightShift = bitutils::rightShiftInsideByte(dstBitIndex) + 1;

	{
		const byte mask	   = bitutils::maskFirstBits(dstLeftShift);
		const byte dstByte = srcByte >> dstLeftShift;

		m_rBytes[dstByteIndex] &= mask;
		m_rBytes[dstByteIndex] |= dstByte;
	}
	if(dstLeftShift)
	{
		const byte mask	   = bitutils::maskLastBits(dstRightShift);
		const byte dstByte = srcByte << dstRightShift;

		m_rBytes[dstByteIndex + 1] &= mask;
		m_rBytes[dstByteIndex + 1] |= dstByte;
	}
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::setBytes(size_t dstBitIndex, const byte srcBytes[], size_t byteCount)
{
	if(bitutils::isMultipleOfByteSize(dstBitIndex))
	{
		const size_t dstByteIndex = bitutils::byteIndex(dstBitIndex);

		void*		dst = &m_rBytes[dstByteIndex];
		const void* src = srcBytes;

		std::memcpy(dst, src, byteCount);
	}
	else
	{
		for(size_t i = 0; i < byteCount; i++)
			setByte(dstBitIndex + i * kByteSize, srcBytes[i]);
	}
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::setBytePart(size_t dstFirstBitIndex, byte srcByte, ushort bitCount)
{
	srcByte &= bitutils::maskFirstBits(bitCount);

	const size_t dstLastBitIndex = dstFirstBitIndex + bitCount - 1;

	const size_t dstByteIndex1 = bitutils::byteIndex(dstFirstBitIndex);
	const size_t dstByteIndex2 = bitutils::byteIndex(dstLastBitIndex);

	const ushort dstFirstLeftShift	= bitutils::leftShiftInsideByte(dstFirstBitIndex);
	const ushort dstFirstRightShift = bitutils::rightShiftInsideByte(dstFirstBitIndex);
	const ushort dstLastRightShift	= bitutils::rightShiftInsideByte(dstLastBitIndex);

	if(dstByteIndex1 == dstByteIndex2)
	{
		const byte mask1   = bitutils::maskFirstBits(dstFirstLeftShift);
		const byte mask2   = bitutils::maskLastBits(dstLastRightShift);
		const byte mask	   = mask1 | mask2;
		const byte dstByte = srcByte >> dstFirstLeftShift;

		m_rBytes[dstByteIndex1] &= mask;
		m_rBytes[dstByteIndex1] |= dstByte;
	}
	else
	{
		{
			const byte mask	   = bitutils::maskFirstBits(dstFirstLeftShift);
			const byte dstByte = srcByte >> dstFirstLeftShift;

			m_rBytes[dstByteIndex1] &= mask;
			m_rBytes[dstByteIndex1] |= dstByte;
		}
		{
			const byte	 mask	   = bitutils::maskLastBits(dstLastRightShift);
			const ushort byteShift = bitutils::rightShiftInsideByte(bitCount);
			const byte	 dstByte   = srcByte << (dstFirstRightShift + 1);

			m_rBytes[dstByteIndex2] &= mask;
			m_rBytes[dstByteIndex2] |= dstByte;
		}
	}
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::copyByte(size_t dstBitIndex, size_t srcByteIndex)
{
	const byte srcByte = m_rBytes[srcByteIndex];
	ByteHelper<ByteArray>::setByte(dstBitIndex, srcByte);
}

template<class ByteArray>
template<Direction direction>
inline void ByteHelper<ByteArray>::copyBytes(size_t dstBitIndex, size_t srcByteIndex, size_t count)
{
	if(bitutils::isMultipleOfByteSize(dstBitIndex))
	{
		const size_t dstByteIndex = bitutils::byteIndex(dstBitIndex);

		void*		dst = &m_rBytes[dstByteIndex];
		const void* src = &m_rBytes[srcByteIndex];

		const size_t diff	 = utils::byteIndexDiff<direction>(dstByteIndex, srcByteIndex);
		const int	 overlap = count - diff;

		if(overlap > 0)
			std::memmove(dst, src, count);
		else
			std::memcpy(dst, src, count);
	}
	else
	{
		if constexpr(utils::isRight<direction>())
		{
			const size_t deltaIndex = count - 1;
			srcByteIndex += deltaIndex;
			dstBitIndex += kByteSize * deltaIndex;
		}

		for(size_t i = 0; i < count; i++)
		{
			copyByte(dstBitIndex, srcByteIndex);
			if constexpr(utils::isLeft<direction>())
				dstBitIndex += kByteSize, srcByteIndex++;
			else
				dstBitIndex -= kByteSize, srcByteIndex--;
		}
	}
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::copyByteStart(size_t dstBitIndex, size_t srcByteIndex, ushort bitCount)
{
	const byte srcByte = m_rBytes[srcByteIndex];

	setBytePart(dstBitIndex, srcByte, bitCount);
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::copyByteEnd(size_t dstBitIndex, size_t srcByteIndex, ushort bitCount)
{
	const size_t bitIndexInByte = kByteSize - bitCount;
	const ushort leftShift		= bitutils::leftShiftInsideByte(bitIndexInByte);
	const byte	 srcByte		= m_rBytes[srcByteIndex] << leftShift;

	setBytePart(dstBitIndex, srcByte, bitCount);
}

template<class ByteArray>
inline void ByteHelper<ByteArray>::copyByteMiddle(size_t dstBitIndex, size_t srcBitIndex, ushort bitCount)
{
	const size_t srcByteIndex = bitutils::byteIndex(srcBitIndex);
	const ushort leftShift	  = bitutils::leftShiftInsideByte(srcBitIndex);
	const byte	 srcByte	  = m_rBytes[srcByteIndex] << leftShift;

	setBytePart(dstBitIndex, srcByte, bitCount);
}

template<class ByteArray>
template<Direction direction>
inline void ByteHelper<ByteArray>::shiftBits(size_t index, size_t count, size_t shift)
{
	if(bitutils::bitsAreInMiddleOfByte(index, count))
	{
		const size_t dstBitIndex = utils::shiftBitIndex<direction>(index, shift);
		copyByteMiddle(dstBitIndex, index, count);
		return;
	}

	if constexpr(utils::isLeft<direction>())
	{
		shiftBitsLeftByteEnd<direction>(index, count, shift);
		shiftBitsMiddleBytes<direction>(index, count, shift);
		shiftBitsRightByteStart<direction>(index, count, shift);
	}
	else
	{
		shiftBitsRightByteStart<direction>(index, count, shift);
		shiftBitsMiddleBytes<direction>(index, count, shift);
		shiftBitsLeftByteEnd<direction>(index, count, shift);
	}
}

template<class ByteArray>
template<Direction direction>
inline void ByteHelper<ByteArray>::shiftBitsLeftByteEnd(size_t index, size_t count, size_t shift)
{
	const size_t leftmostBitIndex = index;

	if(bitutils::leftShiftInsideByte(leftmostBitIndex) != 0)
	{
		const size_t srcBitIndex  = bitutils::bitIndexInsideByte(leftmostBitIndex);
		const size_t srcByteIndex = bitutils::byteIndex(srcBitIndex);
		const size_t dstBitIndex  = utils::shiftBitIndex<direction>(leftmostBitIndex, shift);

		const ushort bitCount = bitutils::rightShiftInsideByte(leftmostBitIndex) + 1;

		copyByteEnd(dstBitIndex, srcByteIndex, bitCount);
	}
}

template<class ByteArray>
template<Direction direction>
inline void ByteHelper<ByteArray>::shiftBitsMiddleBytes(size_t index, size_t count, size_t shift)
{
	if(!bitutils::hasCompleteByte(index, count))
		return;

	const size_t leftmostBitIndex  = index;
	const size_t rightmostBitIndex = index + count - 1;

	const ushort maxBitIndexInByte			= kByteSize - 1;
	const size_t leftmostCompleteByteIndex	= bitutils::byteIndex(leftmostBitIndex + maxBitIndexInByte);
	const size_t rightmostCompleteByteIndex = bitutils::byteIndex(rightmostBitIndex - maxBitIndexInByte);

	const size_t byteCount = rightmostCompleteByteIndex - leftmostCompleteByteIndex + 1;

	if(byteCount != 0)
	{
		const size_t srcByteIndex = leftmostCompleteByteIndex;
		const size_t srcBitIndex  = bitutils::bitIndex(srcByteIndex);
		const size_t dstBitIndex  = utils::shiftBitIndex<direction>(srcBitIndex, shift);

		copyBytes<direction>(dstBitIndex, srcByteIndex, byteCount);
	}
}

template<class ByteArray>
template<Direction direction>
inline void ByteHelper<ByteArray>::shiftBitsRightByteStart(size_t index, size_t count, size_t shift)
{
	const size_t rightmostBitIndex = index + count - 1;

	if(bitutils::rightShiftInsideByte(rightmostBitIndex) != 0)
	{
		const size_t srcBitIndex  = bitutils::bitIndexAtByteStart(rightmostBitIndex);
		const size_t srcByteIndex = bitutils::byteIndex(srcBitIndex);
		const size_t dstBitIndex  = utils::shiftBitIndex<direction>(srcBitIndex, shift);

		const ushort bitCount = bitutils::leftShiftInsideByte(rightmostBitIndex) + 1;

		copyByteStart(dstBitIndex, srcByteIndex, bitCount);
	}
}

} // namespace qotf::internal