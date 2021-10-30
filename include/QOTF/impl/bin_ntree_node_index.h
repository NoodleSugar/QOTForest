#ifndef BIN_NTREE_NODE_INDEX_H
#define BIN_NTREE_NODE_INDEX_H

#include <cstdint>

namespace QOTF
{
typedef unsigned int uint;

namespace _BinNTree
{
#define FIRST_BYTE_NODE_BIT_SHIFT  6
#define SECOND_BYTE_NODE_BIT_SHIFT 4
#define THIRD_BYTE_NODE_BIT_SHIFT  2
#define FOURTH_BYTE_NODE_BIT_SHIFT 0

	struct NodeIndex
	{
		size_t byteIndex;
		uint   bitShift;

		NodeIndex() :
			byteIndex(0), bitShift(FIRST_BYTE_NODE_BIT_SHIFT) {}

		NodeIndex(const NodeIndex&) = default;

		NodeIndex(size_t bitPos) :
			byteIndex(bitPos >> 3), bitShift(6 - (bitPos & 0b111)) {}

		size_t toBitPos() const { return (byteIndex << 3) + 6 - bitShift; }

		void next()
		{
			if(bitShift == FOURTH_BYTE_NODE_BIT_SHIFT)
			{
				byteIndex++;
				bitShift = FIRST_BYTE_NODE_BIT_SHIFT;
			}
			else
				bitShift -= 2;
		}

		void prev()
		{
			if(bitShift == FIRST_BYTE_NODE_BIT_SHIFT)
			{
				byteIndex--;
				bitShift = FOURTH_BYTE_NODE_BIT_SHIFT;
			}
			else
				bitShift += 2;
		}
	};

} // namespace _BinNTree

} // namespace QOTF

#endif