#include <qotf/internal/BitVector.hpp>

#include <qotf/internal/BitUtils.hpp>
#include <qotf/internal/ByteHelper.hpp>

#include <cassert>

namespace qotf::internal
{

BitVector::BitVector() :
	m_size(0) {}

BitVector::BitVector(size_t size, bool value) :
	m_bytes(bitutils::byteCount(size), value ? kFullByte : kEmptyByte),
	m_size(size) {}

BitVector::BitVector(std::initializer_list<byte> bytes) :
	m_bytes(bytes),
	m_size(bitutils::bitCount(bytes.size())) {}

void BitVector::resize(size_t size)
{
	const size_t neededByteCount = bitutils::byteCount(size);
	if(neededByteCount > m_bytes.size())
		m_bytes.resize(neededByteCount, kEmptyByte);
	m_size = size;
}

void BitVector::reserve(size_t count)
{
	const size_t neededByteCount = bitutils::byteCount(count);
	m_bytes.reserve(neededByteCount);
}

bool BitVector::get(size_t index) const
{
	return ByteHelper<const ByteVector>(m_bytes).getBit(index);
}

template<bool bit>
void BitVector::set(size_t index)
{
	ByteHelper<ByteVector>(m_bytes).setBit<bit>(index);
}

void BitVector::set(size_t index, bool bit)
{
	if(bit)
		set<true>(index);
	else
		set<false>(index);
}

template<bool bit>
void BitVector::setMany(size_t index, size_t count)
{
	ByteHelper<ByteVector>(m_bytes).setBits<bit>(index, count);
}

void BitVector::setMany(size_t index, size_t count, bool bit)
{
	if(bit)
		setMany<true>(index, count);
	else
		setMany<false>(index, count);
}

template<bool bit>
void BitVector::append(size_t count)
{
	const size_t index = m_size;
	resize(m_size + count);
	setMany<bit>(index, count);
}

void BitVector::append(size_t count, bool bit)
{
	if(bit)
		append<true>(count);
	else
		append<false>(count);
}

template<bool bit>
void BitVector::insert(size_t index, size_t count)
{
	const size_t bitCountToShift = m_size - index;
	resize(m_size + count);
	ByteHelper<ByteVector>(m_bytes).shiftBits<Direction::ToRight>(index, bitCountToShift, count);

	setMany<bit>(index, count);
}

void BitVector::insert(size_t index, size_t count, bool bit)
{
	if(bit)
		insert<true>(index, count);
	else
		insert<false>(index, count);
}

void BitVector::remove(size_t index, size_t count)
{
	const size_t indexToShift	 = index + count;
	const size_t bitCountToShift = m_size - indexToShift;
	ByteHelper<ByteVector>(m_bytes).shiftBits<Direction::ToLeft>(indexToShift, bitCountToShift, count);
	resize(m_size - count);
}

template<class Pattern>
void BitVector::applyPattern(size_t index, const Pattern& pattern, size_t repeat)
{
	const byte*	 data = pattern.data();
	const size_t size = pattern.size();

	const size_t completeByteCount = bitutils::completeByteCount(size);

	{
		const size_t maxBitIndex = index + repeat * size;

		for(; index < maxBitIndex; index += size)
			ByteHelper<ByteVector>(m_bytes).setBytes(index, data, completeByteCount);
	}

	if(!bitutils::isMultipleOfByteSize(size))
	{
		const ushort incompleteByteSize = bitutils::bitIndexInsideByte(size) + 1;

		ByteHelper<ByteVector>(m_bytes).setBytePart(index, data[completeByteCount], incompleteByteSize);
	}
}

} // namespace qotf::internal