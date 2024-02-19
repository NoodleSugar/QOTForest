#pragma once

#include <qotf/utils/Types.hpp>

#include <vector>

namespace qotf::internal
{

class BitVector
{
	// TODO
	struct Iterator
	{
		class Reference
		{
		};

		using iterator_category = std::random_access_iterator_tag ;
		using difference_type	= std::size_t;
		using value_type		= bool;
		using pointer			= Reference;
		using reference			= Reference;

	};

public:
	BitVector();
	explicit BitVector(size_t size, bool value = false);
	explicit BitVector(std::initializer_list<byte> bytes);
	BitVector(const BitVector&) = default;
	BitVector(BitVector&&)		= default;

	BitVector& operator=(const BitVector&) = default;
	BitVector& operator=(BitVector&&)	   = default;

	byte* data() { return m_bytes.data(); }

	const byte* data() const { return m_bytes.data(); }

	/**
	 * Return the size of the array in bits
	 */
	size_t size() const { return m_size; }

	/**
	 * Set the size of the array in bits
	 */
	void resize(size_t size);

	/**
	 * Reserve a specific amount of memory for the desired number of bits
	 */
	void reserve(size_t count);

	/**
	 * Get the bit at [index]
	 * Requires :
	 *   - index < size
	 */
	bool get(size_t index) const;

	/**
	 * Set to [b] the bit at [index]
	 * Requires :
	 *   - index < size
	 */
	template<bool b = true>
	void set(size_t index);
	void set(size_t index, bool b);

	/**
	 * Set to [b] a sequence of [count] bits beginning at [index]
	 * Requires :
	 *   - index + count <= size
	 */
	template<bool b = true>
	void setMany(size_t index, size_t count);
	void setMany(size_t index, size_t count, bool bit);

	/**
	 * Append [count] bits at the end of the array, and set them to [bit]
	 */
	template<bool b = false>
	void append(size_t count);
	void append(size_t count, bool bit);

	/**
	 * Insert [count] bits at [index], and set them to [bit]
	 *   - index <= size
	 */
	template<bool bit = false>
	void insert(size_t index, size_t count);
	void insert(size_t index, size_t count, bool bit);

	/**
	 * Remove [count] bits at [index]
	 * Requires :
	 *   - index + count <= size
	 */
	void remove(size_t index, size_t count);

	/**
	 * Apply the [pattern] [repeat] times to the BitVector, starting a [index]
	 * The pattern must be left aligned
	 * Requires :
	 *   - Pattern class contains methods
	 *     - size() (size_t like)
	 *     - data() (array like)
	 * 	 - index + pattern.size * repeat < size
	 */
	template<class Pattern>
	void applyPattern(size_t index, const Pattern& pattern, size_t repeat = 1);

	bool operator[](size_t index) const { return get(index); }

private:
	using ByteVector = std::vector<byte>;

	ByteVector m_bytes;
	size_t	   m_size;
};

} // namespace qotf::internal