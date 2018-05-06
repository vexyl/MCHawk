#ifndef CPE_H_
#define CPE_H_

namespace CPE {
enum BlockType {
	kStartOfBlockTypes=0x32,
	kCobblestoneSlab=kStartOfBlockTypes,
	kRope,
	kSandstone,
	kSnow,
	kFire,
	kLightPinkWool,
	kForestGreenWool,
	kBrownWool,
	kDeepBlue,
	kTurquoise,
	kIce,
	kCeramicTile,
	kMagma,
	kPillar,
	kCrate,
	kStoneBrick,
	kEndOfBlockTypes
};

bool IsValidBlock(int type);
} // namespace CPE

#endif // CPE_H_
