#include "Pikeman.h"

Pikeman::Pikeman(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		PIKEMAN_SPEED,
		PIKEMAN_COST,
		PIKEMAN_RANGE,
		PIKEMAN_BUILD_TIME,
		PIKEMAN_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
