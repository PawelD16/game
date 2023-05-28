#include "Swordsman.h"

Swordsman::Swordsman(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		SWORDSMAN_SPEED,
		SWORDSMAN_COST,
		SWORDSMAN_RANGE,
		SWORDSMAN_BUILD_TIME,
		SWORDSMAN_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
