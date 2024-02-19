#pragma once

#include <qotf/internal/Utils.hpp>
#include <qotf/utils/Types.hpp>

namespace qotf
{

template<size_t N>
struct FixedBitPattern
{
	static constexpr size_t size	   = N;
	static constexpr size_t kByteCount = utils::byteCount(size);

	byte[kByteCount] data;

	byte*  data() { return data; }
	size_t size() const { return size; }
};

} // namespace qotf