#pragma once
#include "AttackMoveUnit.h"

class Worker : public AttackMoveUnit
{
public:
	Worker(int durability, int positionX, int positionY, int unitID);
	~Worker() override = default;
};

