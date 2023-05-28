#pragma once
#include "AttackMoveUnit.h"

class Pikeman : public AttackMoveUnit
{
public:
	Pikeman(int durability, int positionX, int positionY, int unitID);
	~Pikeman() override = default;
};

