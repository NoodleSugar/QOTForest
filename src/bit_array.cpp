#include "QOTF/bit_array.h"
#include <cstring>

namespace QOTF
{
#define BYTE_SIZE_IN_BITS 8
#define BYTE_MASK		  0xFF
#define BIT_MASK		  0x01
#define EMPTY_BYTE		  0x00

#define BYTE_INDEX_FROM_BIT_INDEX(bitIndex)				 ((bitIndex) >> 3)
#define BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(bitIndex) ((bitIndex) & ~(0b111))
#define BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(bitIndex)		 ((bitIndex)&0b111)

#define IS_NOT_MULTIPLE_OF_8(bitIndex) ((bitIndex)&0b111)

BitArray::BitArray(size_t sizeInBits) :
	m_bytes((sizeInBits + BYTE_SIZE_IN_BITS - 1) / BYTE_SIZE_IN_BITS, EMPTY_BYTE),
	m_sizeInBits(sizeInBits)
{
}

void BitArray::resize(size_t sizeInBits)
{
	const size_t neededByteNumber = (sizeInBits + BYTE_SIZE_IN_BITS - 1) / BYTE_SIZE_IN_BITS;
	if(neededByteNumber > m_bytes.size())
		m_bytes.resize(neededByteNumber, EMPTY_BYTE);
	m_sizeInBits = sizeInBits;
}

byte BitArray::getByteAtBit(size_t bitIndex) const
{
	const size_t byteIndex	   = BYTE_INDEX_FROM_BIT_INDEX(bitIndex);
	const uint	 bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(bitIndex);

	// IF the byte requested is well aligned
	if(bitInnerIndex == 0)
		return m_bytes[byteIndex];

	// Beginning of the byte
	const byte b = (m_bytes[byteIndex] & (BYTE_MASK >> bitInnerIndex)) << bitInnerIndex;
	// End of the byte
	const byte e = (m_bytes[byteIndex + 1] & ~(BYTE_MASK >> bitInnerIndex)) >> (BYTE_SIZE_IN_BITS - bitInnerIndex);

	return b | e;
}

void BitArray::setByteAtBit(size_t bitIndex, byte b)
{
	const size_t byteIndex	   = BYTE_INDEX_FROM_BIT_INDEX(bitIndex);
	const uint	 bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(bitIndex);

	// IF the byte to set is well aligned
	if(bitInnerIndex == 0)
	{
		m_bytes[byteIndex] = b;
		return;
	}

	// Beginning of the byte
	m_bytes[byteIndex] &= ~(BYTE_MASK >> bitInnerIndex);
	m_bytes[byteIndex] |= b >> bitInnerIndex;

	// End of the byte
	m_bytes[byteIndex + 1] &= BYTE_MASK >> bitInnerIndex;
	m_bytes[byteIndex + 1] |= b << (BYTE_SIZE_IN_BITS - bitInnerIndex);
}

bool BitArray::get(size_t bitIndex) const
{
	const size_t byteIndex	   = BYTE_INDEX_FROM_BIT_INDEX(bitIndex);
	const uint	 bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(bitIndex);
	return m_bytes[byteIndex] >> (BYTE_SIZE_IN_BITS - 1 - bitInnerIndex) & BIT_MASK;
}

void BitArray::set(size_t bitIndex, size_t number)
{
	size_t posLeft		 = bitIndex;		  // The pos the leftmost bit to set
	size_t posRight		 = bitIndex + number; // The pos of the bit after the rightmost bit to set
	uint   bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posLeft);

	/**
	 * forall pos in [posLeft; posRight[ do:
	 * 	 bits[pos] = 1
	 *
	 * The algorithm set bits from the right to the left following the pattern
	 *	|____ xxxx|xxxx xxxx| ... |xxxx xxxx|xxxx ____|
	 *  |    1    |            2            |    3    |
	 *
	 *  x are bits to set
	 */

	/*
	 * 1 IF the leftmost bit to set
	 *      is not at the start of the current byte
	 */
	if(bitInnerIndex != 0)
	{
		/*
		 * 1.1 IF there are more bits to set than bits accepting the copy on the current byte
		 *	   THEN fill the end of the current byte with 1
		 */
		if(bitInnerIndex + number > BYTE_SIZE_IN_BITS - 1)
		{
			const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			const byte	 mask	   = BYTE_MASK >> bitInnerIndex;
			posLeft				   = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft + BYTE_SIZE_IN_BITS);

			m_bytes[byteIndex] |= mask;
		}
		/*
		 * 1.2 ELSE fill the middle of the current byte with 1
		 */
		else
		{
			const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			const byte	 mask	   = BYTE_MASK >> bitInnerIndex &
							  ~(BYTE_MASK >> (bitInnerIndex + number));

			m_bytes[byteIndex] |= mask;
			return;
		}
	}

	/*
	 * 2 IF the leftmost bit to set
	 *      is not on the same byte as
	 *      the bit after the rightmost bit to set
	 *   THEN fill full bytes with 1
	 */
	while(posRight - posLeft > BYTE_SIZE_IN_BITS - 1)
	{
		const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
		posLeft += BYTE_SIZE_IN_BITS;

		m_bytes[byteIndex] |= BYTE_MASK;
	}

	/*
	 * 3 IF the rightmost bit to set
	 *      is not at the left of
	 *      the leftmost bit to set
	 * THEN fill the end of the current byte with 1
	 */
	if(posRight != posLeft)
	{
		const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posRight);
		const byte	 mask	   = BYTE_MASK >> BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posRight);

		m_bytes[byteIndex] |= ~mask;
	}
}

void BitArray::unset(size_t bitIndex, size_t number)
{
	size_t posLeft		 = bitIndex;		  // The pos the leftmost bit to set
	size_t posRight		 = bitIndex + number; // The pos of the bit after the rightmost bit to set
	uint   bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posLeft);

	/**
	 * forall pos in [posLeft; posRight[ do:
	 * 	 bits[pos] = 1
	 *
	 * The algorithm set bits from the right to the left following the pattern
	 *	|____ xxxx|xxxx xxxx| ... |xxxx xxxx|xxxx ____|
	 *  |    1    |            2            |    3    |
	 *
	 *  x are bits to set
	 */

	/*
	 * 1 IF the leftmost bit to unset
	 *      is not at the start of the current byte
	 */
	if(bitInnerIndex != 0)
	{
		/*
		 * 1.1 IF there are more bits to set than bits on the current byte
		 *	   THEN fill the end of the current byte with 0
		 */
		if(bitInnerIndex + number > BYTE_SIZE_IN_BITS - 1)
		{
			const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			const byte	 mask	   = BYTE_MASK >> bitInnerIndex;
			posLeft				   = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft + BYTE_SIZE_IN_BITS);

			m_bytes[byteIndex] &= ~mask;
		}
		/*
		 * 1.2 ELSE fill the middle of the current byte with 0
		 */
		else
		{
			const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			const byte	 mask	   = BYTE_MASK >> bitInnerIndex &
							  ~(BYTE_MASK >> (bitInnerIndex + number));

			m_bytes[byteIndex] &= ~mask;
			return;
		}
	}

	/*
	 * 2 IF the leftmost bit to unset
	 *      is not on the same byte as
	 *      the bit after the rightmost bit to unset
	 *   THEN fill full bytes with 0
	 */
	while(posRight - posLeft > BYTE_SIZE_IN_BITS - 1)
	{
		const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
		posLeft += BYTE_SIZE_IN_BITS;

		m_bytes[byteIndex] &= ~BYTE_MASK;
	}

	/*
	 * 3 IF the rightmost bit to unset
	 *      is not at the left of
	 *      the leftmost bit to unset
	 * THEN fill the end of the current byte with 0
	 */
	if(posRight != posLeft)
	{
		const size_t byteIndex = BYTE_INDEX_FROM_BIT_INDEX(posLeft);
		const byte	 mask	   = BYTE_MASK >> BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posRight);

		m_bytes[byteIndex] &= mask;
	}
}

void BitArray::append(size_t number, bool bit)
{
	const size_t bitIndex = m_sizeInBits;
	resize(m_sizeInBits + number);

	if(bit)
		set(bitIndex, number);
}

void BitArray::insert(size_t bitIndex, size_t number, bool bit)
{
	const size_t numberToShift = m_sizeInBits - bitIndex;
	resize(m_sizeInBits + number);
	shiftRight(bitIndex, numberToShift, number);

	if(bit)
		set(bitIndex, number);
	else
		unset(bitIndex, number);
}

void BitArray::remove(size_t bitIndex, size_t number)
{
	// Shift bits to the left
	const size_t bitIndexToShift = bitIndex + number;
	const size_t numberToShift	 = m_sizeInBits - bitIndexToShift;
	shiftLeft(bitIndexToShift, numberToShift, number);

	// Clean the overflowing bits
	resize(m_sizeInBits - number);
	unset(m_sizeInBits, number);
}

void BitArray::shiftRight(size_t bitIndex, size_t number, size_t value)
{
	size_t posLeft		 = bitIndex + value - 1; // The pos of bit before the leftmost bit accepting the copy
	size_t posRight		 = posLeft + number;	 // The pos of the rightmost bit accepting the copy
	uint   bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posRight);

	/**
	 * forall pos in ]posLeft; posRight] do:
	 * 	 bits[pos] = old_bits[pos - value]
	 *
	 * The algorithm copy from the right to the left following the pattern
	 *	|____ xxxx|xxxx xxxx| ... |xxxx xxxx|xxxx ____|
	 *  |    3    |            2            |    1    |
	 *
	 *  x are bits accepting the copy
	 */

	/*
	 * 1 IF the rightmost bit accepting the copy
	 *      is not at the end of the current byte
	 */
	if(bitInnerIndex != BYTE_SIZE_IN_BITS - 1)
	{
		/*
		 * 1.1 IF there are more bits to copy than bits accepting the copy on the current byte
		 *	   THEN fill the beginning of the current byte accepting the copy
		 */
		if(bitInnerIndex < number)
		{
			bitInnerIndex++;
			const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posRight);
			const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posRight) - value;
			const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
			const byte	 mask			= BYTE_MASK >> bitInnerIndex;
			posRight					= BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posRight) - 1;

			m_bytes[byteIndex] &= mask;
			m_bytes[byteIndex] |= targetByte & ~mask;
		}

		/*
		 * 1.2 ELSE fill the middle of the current byte accepting the copy
		 */
		else
		{
			bitInnerIndex++;
			const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posRight);
			const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posRight) - value;
			const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
			const byte	 mask			= (BYTE_MASK >> bitInnerIndex) |
							  (BYTE_MASK << (BYTE_SIZE_IN_BITS - bitInnerIndex + number));

			m_bytes[byteIndex] &= mask;
			m_bytes[byteIndex] |= targetByte & ~mask;
			return;
		}
	}

	/*
	 * 2 IF the rightmost bit accepting the copy
	 *      is not on the same byte as
	 *      the bit before the leftmost bit
	 *   THEN fill full bytes accepting the copy
	 */
	if(posRight - posLeft > BYTE_SIZE_IN_BITS - 1)
	{
		/*
		 * 2.1
		 */
		if(IS_NOT_MULTIPLE_OF_8(value))
		{
			do
			{
				const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posRight);
				const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posRight) - value;
				const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
				posRight -= BYTE_SIZE_IN_BITS;

				m_bytes[byteIndex] &= 0;
				m_bytes[byteIndex] |= targetByte;
			} while(posRight - posLeft > BYTE_SIZE_IN_BITS - 1);
		}

		/*
		 * 2.2
		 */
		else
		{
			byte* const	 src = m_bytes.data() + BYTE_INDEX_FROM_BIT_INDEX(bitIndex + BYTE_SIZE_IN_BITS - 1);
			byte* const	 dst = m_bytes.data() + BYTE_INDEX_FROM_BIT_INDEX(posLeft + BYTE_SIZE_IN_BITS - 1);
			const size_t dif = BYTE_INDEX_FROM_BIT_INDEX(posRight - BYTE_SIZE_IN_BITS + 1) -
							   BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			posRight -= dif * BYTE_SIZE_IN_BITS;

			memmove(dst, src, dif);
		}
	}

	/*
	 * 3 IF the rightmost bit accepting the copy
	 *      is not at the left of
	 *      the leftmost bit accepting the copy
	 * THEN fill the end of the current byte accepting the copy
	 */
	if(posRight != posLeft)
	{
		posLeft++;
		bitInnerIndex				= BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posLeft);
		const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posLeft);
		const size_t bitIndexToCopy = bitIndex;
		const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
		const byte	 mask			= BYTE_MASK >> bitInnerIndex;

		m_bytes[byteIndex] &= ~mask;
		m_bytes[byteIndex] |= targetByte >> bitInnerIndex;
	}
}

void BitArray::shiftLeft(size_t bitIndex, size_t number, size_t value)
{
	size_t posLeft		 = bitIndex - value;	 // The pos of the leftmost bit accepting the copy
	size_t posRight		 = posLeft + number + 1; // The pos of the bit after the rightmost bit accepting the copy
	uint   bitInnerIndex = BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posLeft);

	/**
	 * forall pos in [posLeft; posRight[ do:
	 * 	 bits[pos] = old_bits[pos + value]
	 *
	 * The algorithm copy from the left to the right following the pattern
	 *	|____ xxxx|xxxx xxxx| ... |xxxx xxxx|xxxx ____|
	 *  |    1    |            2            |    3    |
	 *
	 *  x are bits accepting the copy
	 */

	/*
	 * 1 IF the leftmost bit accepting the copy
	 *      is not at the start of the current byte
	 */
	if(bitInnerIndex != 0)
	{
		/*
		 * 1.1 IF there are more bits to copy than bits accepting the copy on the current byte
		 *	   THEN fill the end of the current byte accepting the copy
		 */
		if(bitInnerIndex + number > BYTE_SIZE_IN_BITS - 1)
		{
			const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft) + value;
			const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
			const byte	 mask			= BYTE_MASK >> bitInnerIndex;
			posLeft						= BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft + BYTE_SIZE_IN_BITS);

			m_bytes[byteIndex] &= ~mask;
			m_bytes[byteIndex] |= targetByte & mask;
		}

		/*
		 * 1.2 ELSE fill the middle of the current byte accepting the copy
		 */
		else
		{
			const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posLeft);
			const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft) + value;
			const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
			const byte	 mask			= (BYTE_MASK << bitInnerIndex) |
							  (BYTE_MASK >> (BYTE_SIZE_IN_BITS - bitInnerIndex + number));

			m_bytes[byteIndex] &= mask;
			m_bytes[byteIndex] |= targetByte & ~mask;
			return;
		}
	}

	/*
	 * 2 IF the leftmost bit accepting the copy
	 *      is not on the same byte as
	 *      the bit after the rightmost bit
	 *   THEN fill full bytes accepting the copy
	 */
	if(posRight - posLeft > BYTE_SIZE_IN_BITS - 1)
	{
		/*
		 * 2.1
		 */
		if(IS_NOT_MULTIPLE_OF_8(value))
		{
			do
			{
				const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posLeft);
				const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft) + value;
				const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
				posLeft += BYTE_SIZE_IN_BITS;

				m_bytes[byteIndex] &= 0;
				m_bytes[byteIndex] |= targetByte;
			} while(posRight - posLeft > BYTE_SIZE_IN_BITS - 1);
		}

		/*
		 * 2.2
		 */
		else
		{
			byte* const	 src = m_bytes.data() + BYTE_INDEX_FROM_BIT_INDEX(bitIndex + BYTE_SIZE_IN_BITS - 1);
			byte* const	 dst = m_bytes.data() + BYTE_INDEX_FROM_BIT_INDEX(posLeft + BYTE_SIZE_IN_BITS - 1);
			const size_t dif = BYTE_INDEX_FROM_BIT_INDEX(posRight) -
							   BYTE_INDEX_FROM_BIT_INDEX(posLeft + BYTE_SIZE_IN_BITS - 1);
			posLeft += dif * BYTE_SIZE_IN_BITS;

			memmove(dst, src, dif);
		}
	}

	/*
	 * 3 IF the rightmost bit accepting the copy
	 *      is not at the left of
	 *      the last leftmost bit accepting the copy
	 * THEN fill the end of the current byte accepting the copy
	 */
	if(posRight != posLeft)
	{
		posRight--;
		bitInnerIndex				= BIT_INDEX_IN_BYTE_FROM_BIT_INDEX(posRight);
		const size_t byteIndex		= BYTE_INDEX_FROM_BIT_INDEX(posRight);
		const size_t bitIndexToCopy = BIT_INDEX_AT_BYTE_START_FROM_BIT_INDEX(posLeft) + value - (BYTE_SIZE_IN_BITS - bitInnerIndex);
		const byte	 targetByte		= getByteAtBit(bitIndexToCopy);
		const byte	 mask			= BYTE_MASK >> bitInnerIndex;

		m_bytes[byteIndex] &= mask;
		m_bytes[byteIndex] |= targetByte << (BYTE_SIZE_IN_BITS - bitInnerIndex);
	}
}

} // namespace QOTF