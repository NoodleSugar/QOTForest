#pragma once

namespace qotf
{
enum class Node3Index
{
	// Morton Code for 3 dimensions
	//                   XYZ
	LeftBottomBack	 = 0b000,
	LeftBottomFront	 = 0b001,
	LeftTopBack		 = 0b010,
	LeftTopFront	 = 0b011,
	RightBottomBack	 = 0b100,
	RightBottomFront = 0b101,
	RightTopBack	 = 0b110,
	RightTopFront	 = 0b111
};

#define LIST_OF_INDICES_3             \
	X(Node3Index::LeftBottomBack)   \
	X(Node3Index::LeftBottomFront)  \
	X(Node3Index::LeftTopBack)      \
	X(Node3Index::LeftTopFront)     \
	X(Node3Index::RightBottomBack)  \
	X(Node3Index::RightBottomFront) \
	X(Node3Index::RightTopBack)     \
	X(Node3Index::RightTopFront)

} // namespace qotf