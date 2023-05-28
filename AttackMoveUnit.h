#pragma once
#include <cstdlib> 
#include <map>
#include <vector>
#include "IAttackable.h"
#include "UnitStatsConsts.h"

// a AttackMoveUnit is a unit that can moveCloserToGoal and tryToAttack, so all units beside Base
class AttackMoveUnit : public IAttackable
{
public:
	AttackMoveUnit(
		int durability,
		int speed,
		int cost,
		int range,
		int build_time,
		char classLetter,
		int positionX,
		int positionY,
		int unitID
	);
	~AttackMoveUnit() override = default;;

	void moveCloserToGoal(int goalPosX, int goalPosY, std::vector<std::vector<int>>& mapClip, int distance);
	bool tryToAttack(IAttackable& opponentUnit);
	// AttackMoveUnit& refreshForNewTourAndReturnThis(int leftOverDurability);

	char getLetter() override;
	int getSpeed() const;
	int getRange() const;

protected:
	char classLetter;

	// int durability;
	// int speed;
	int cost;
	int range;
	int build_time;
	// int positionX{};
	// int positionY{};

	// bool hasAttacked;
	int currentTourSpeed;

	int calculateDistanceFromCurrentPos(int positionX, int positionY) const;
};


int calculateDistanceBetweenTwoPositions(int posx1, int posx2, int posy1, int posy2);
int getDamage(char u1, char u2);
