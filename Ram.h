#pragma once
#include "AttackMoveUnit.h"

class Ram : public AttackMoveUnit
{
public:
	Ram(int durability, int positionX, int positionY, int unitID);
	~Ram() override = default;
};

