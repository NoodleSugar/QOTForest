#pragma once

namespace qotf
{
enum class Node2Index
{
	// Morton Code for 2 dimensions
	//              XY
	LeftBottom	= 0b00,
	LeftTop		= 0b01,
	RightBottom = 0b10,
	RightTop	= 0b11
};

#define LIST_OF_INDICES_2        \
	X(Node2Index::LeftBottom)  \
	X(Node2Index::LeftTop)     \
	X(Node2Index::RightBottom) \
	X(Node2Index::RightTop)

} // namespace qotf