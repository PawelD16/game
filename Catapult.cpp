#include "Catapult.h"

Catapult::Catapult(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		CATAPULT_SPEED,
		CATAPULT_COST,
		CATAPULT_RANGE,
		CATAPULT_BUILD_TIME,
		CATAPULT_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
