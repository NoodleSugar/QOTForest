#ifndef NODE2_INDEX_H
#define NODE2_INDEX_H

namespace QOTF
{
enum class Node2Index
{
	// Morton Code 2 XY
	LEFT_BOTTOM	 = 0b00,
	LEFT_TOP	 = 0b01,
	RIGHT_BOTTOM = 0b10,
	RIGHT_TOP	 = 0b11
};

#define LIST_OF_INDEXS2 \
	X(LEFT_BOTTOM)      \
	X(LEFT_TOP)         \
	X(RIGHT_BOTTOM)     \
	X(RIGHT_TOP)

} // namespace QOTF

#endif
