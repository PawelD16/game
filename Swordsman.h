#pragma once
#include "AttackMoveUnit.h"

class Swordsman : public AttackMoveUnit
{
public:
	Swordsman(int durability, int positionX, int positionY, int unitID);
	~Swordsman() override = default;
};

