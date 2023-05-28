#pragma once
#include "AttackMoveUnit.h"

class Archer : public AttackMoveUnit
{
public:
	Archer(int durability, int positionX, int positionY, int unitID);
	~Archer() override = default;
};

