#pragma once

#include "qotf/utils/Types.hpp"

namespace qotf
{

template<uint Dimension, class Label>
struct Node final
{
    uint depth;
    Label label;
};

} // namespace qotf