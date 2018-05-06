#include "CPE.hpp"

bool CPE::IsValidBlock(int type)
{
	for (int i = BlockType::kStartOfBlockTypes; i < BlockType::kEndOfBlockTypes; ++i) {
		if (type == i)
			return true;
	}

	return false;
}
