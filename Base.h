#pragma once
#include "UnitStatsConsts.h"
#include "IAttackable.h"

class Base : public IAttackable
{
public:
	Base();
	Base(int durability, int positionX, int positionY, int unitID);
	~Base() override = default;

	char getLetter() override;

	//private:
		// int durability;
		//int positionX;
		//int positionY;
};
