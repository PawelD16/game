#include "Knight.h"

Knight::Knight(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		KNIGHT_SPEED,
		KNIGHT_COST,
		KNIGHT_RANGE,
		KNIGHT_BUILD_TIME,
		KNIGHT_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
