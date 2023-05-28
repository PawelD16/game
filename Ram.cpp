#include "Ram.h"

Ram::Ram(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		RAM_SPEED,
		RAM_COST,
		RAM_RANGE,
		RAM_BUILD_TIME,
		RAM_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
