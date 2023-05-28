#include "Archer.h"

Archer::Archer(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		ARCHER_SPEED,
		ARCHER_COST,
		ARCHER_RANGE,
		ARCHER_BUILD_TIME,
		ARCHER_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
