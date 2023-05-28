#pragma once
#include "AttackMoveUnit.h"

class Knight : public AttackMoveUnit
{
public:
	Knight(int durability, int positionX, int positionY, int unitID);
	~Knight() override = default;
};
