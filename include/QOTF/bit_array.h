#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <vector>

namespace QOTF
{
using byte = unsigned char;

typedef unsigned int uint;

class BitArray
{
public:
	BitArray(size_t sizeInBits);

	byte* getByteAccess() { return m_bytes.data(); }

	const byte* getDirectAccess() const { return m_bytes.data(); }

	/**
	 * Return the size of the array in bits
	 */
	size_t getSize() const { return m_sizeInBits; }

	/**
	 * Set the size of the array in bits
	 */
	void resize(size_t sizeInBits);

	/**
	 * Get the byte beginning at [bitIndex]
	 * Requires :
	 *   - bitIndex <= sizeInBits - 8
	 */
	byte getByteAtBit(size_t bitIndex) const;

	/**
	 * Set the byte beginning at [bitIndex] to [b]
	 * Requires :
	 *   - bitIndex <= sizeInBits - 8
	 */
	void setByteAtBit(size_t bitIndex, byte b);

	/**
	 * Get the bit at [bitIndex]
	 * Requires :
	 *   - bitIndex < sizeInBits
	 */
	bool get(size_t bitIndex) const;

	/**
	 * Set to 1 a sequence of [number] bits beginning at [bitIndex]
	 * Requires :
	 *   - bitIndex + number <= sizeInBits
	 */
	void set(size_t bitIndex, size_t number);

	/**
	 * Set to 0 a sequence of [number] bits beginning at [bitIndex]
	 * Requires :
	 *   - bitIndex + number <= sizeInBits
	 */
	void unset(size_t bitIndex, size_t number);

	/**
	 * Append [number] bits at the end of the array, and set them to [bit]
	 */
	void append(size_t number, bool bit = false);

	/**
	 * Insert [number] bits at [bitIndex], and set them to [bit]
	 *   - bitIndex <= sizeInBits
	 */
	void insert(size_t bitIndex, size_t number, bool bit = false);

	/**
	 * Remove [number] bits at [bitIndex]
	 * Requires :
	 *   - bitIndex + number <= sizeInBits
	 */
	void remove(size_t bitIndex, size_t number);

	/**
	 * Apply the [pattern] [times] times to the BitArray, starting a [bitIndex]
	 * The pattern must be left aligned
	 * Requires :
	 * 	 - bitIndex + patternSize * times < sizeInBits
	 *   - patternSize > 0
	 *   - patternSize <= 8
	 *   - times > 0
	 */
	void applyPattern(size_t bitIndex, byte pattern, uint patternSize, uint times = 1);

private:
	std::vector<byte> m_bytes;
	size_t			  m_sizeInBits;

	/**
	 * Shift to the left [number] bits beginning at [bitIndex] by an amount of [value]
	 * Requires :
	 *   - bitIndex - value >= 0
	 *   - bitIndex + number <= sizeInBits
	 */
	void shiftLeft(size_t bitIndex, size_t number, size_t value);

	/**
	 * Shift to the right [number] bits beginning at [bitIndex] by an amount of [value]
	 * Requires :
	 *   - bitIndex + number + value <= sizeInBits
	 */
	void shiftRight(size_t bitIndex, size_t number, size_t value);
};
} // namespace QOTF

#endif
