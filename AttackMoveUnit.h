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
	// this method ensures that an illegal attack won't be carried out
	bool tryToAttack(IAttackable& opponentUnit);

	char getLetter() override;
	int getSpeed() const;
	int getRange() const;

protected:
	char classLetter;
	int cost;
	int range;
	int build_time;
	int currentTourSpeed;

	int calculateDistanceFromCurrentPos(int positionX, int positionY) const;
};


int calculateDistanceBetweenTwoPositions(int posx1, int posx2, int posy1, int posy2);
int getDamage(char u1, char u2);
