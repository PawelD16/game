#pragma once
#include "AttackMoveUnit.h"

class Catapult: public AttackMoveUnit
{
public:
	Catapult(int durability, int positionX, int positionY, int unitID);
	~Catapult() override = default;
};

