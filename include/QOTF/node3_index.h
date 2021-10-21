#ifndef NODE3_INDEX_H
#define NODE3_INDEX_H

namespace QOTF
{
enum class Node3Index
{
	// Morton Code 3       XYZ
	LEFT_BOTTOM_BACK   = 0b000,
	LEFT_BOTTOM_FRONT  = 0b001,
	LEFT_TOP_BACK	   = 0b010,
	LEFT_TOP_FRONT	   = 0b011,
	RIGHT_BOTTOM_BACK  = 0b100,
	RIGHT_BOTTOM_FRONT = 0b101,
	RIGHT_TOP_BACK	   = 0b110,
	RIGHT_TOP_FRONT	   = 0b111
};

#define LIST_OF_INDEXS3   \
	X(LEFT_BOTTOM_BACK)   \
	X(LEFT_BOTTOM_FRONT)  \
	X(LEFT_TOP_BACK)      \
	X(LEFT_TOP_FRONT)     \
	X(RIGHT_BOTTOM_BACK)  \
	X(RIGHT_BOTTOM_FRONT) \
	X(RIGHT_TOP_BACK)     \
	X(RIGHT_TOP_FRONT)

} // namespace QOTF

#endif
