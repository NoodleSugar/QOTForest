#pragma once

#include <qotf/utils/Types.hpp>

#include <vector>

namespace qotf
{

struct BitPattern
{
	size_t			  size;
	std::vector<byte> data;

	
	byte*  data() { return data.data(); }
	size_t size() const { return size; }
};

} // namespace qotf