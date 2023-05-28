#include "AttackMoveUnit.h"

AttackMoveUnit::AttackMoveUnit(
	int durability,
	int speed,
	int cost,
	int range,
	int build_time,
	char classLetter,
	int positionX,
	int positionY,
	int unitID
) :
	IAttackable(
		durability,
		positionX,
		positionY,
		unitID
	),
	classLetter(classLetter),
	cost(cost),
	range(range),
	build_time(build_time),
	currentTourSpeed(speed)
{
}


void AttackMoveUnit::moveCloserToGoal(int goalPosX, int goalPosY, std::vector<std::vector<int>>& mapClip, int distance)
{
	/*
	This is a matrix which is as sig as the units movement allows.
	INT_MAX is where the unit cannot moveCloserToGoal, rest is 0.
	We calculate the distance between the position that the unit could moveCloserToGoal to
	and the (enemy base - range of units tryToAttack) (THE GOAL is to destroy the base, so rushing it is preferred)
	we can keep distance from the base.

	For example for a Pikeman the starting matrix could look like this:
	00000
	00000
	00x00
	00000
	00000
	The position of the Pikeman was marked with an x (there is a 0 there normally).
	*/

	// if the unit can already is close enough to its target, there is no need to move closer
	if (calculateDistanceBetweenTwoPositions(positionX, goalPosX, positionY, goalPosY) - distance <= 0)
	{
		return;
	}

	for (int i = 0; static_cast<size_t>(i) < mapClip.size();++i)
	{
		for (int j = 0; static_cast<size_t>(j) < mapClip[i].size();++j)
		{
			if (mapClip[i][j] != WRONG_POS_ON_MAPCLIP)
			{
				int temp = calculateDistanceBetweenTwoPositions((i - currentTourSpeed + positionX), goalPosX, (j - currentTourSpeed + positionY), goalPosY);

				// moving only as close as it is necessary, could allow to attack one more tour
				// this rewards standing as far as possible
				if (temp <= distance)
				{
					temp = (-1) * temp;
				}

				mapClip[i][j] = temp;
			}
		}
	}

	int minimum = WRONG_POS_ON_MAPCLIP;
	int minX = 0;
	int minY = 0;

	for (int i = 0; static_cast<size_t>(i) < mapClip.size(); ++i) {
		for (int j = 0; static_cast<size_t>(j) < mapClip[i].size(); ++j) {
			if (mapClip[i][j] < minimum) {
				minimum = mapClip[i][j];
				minX = i;
				minY = j;
			}
		}
	}

	// absolute location
	minX = minX - currentTourSpeed + positionX;
	minY = minY - currentTourSpeed + positionY;

	const int res = calculateDistanceFromCurrentPos(minX, minY);
	if (res <= currentTourSpeed)
	{
		positionX = minX;
		positionY = minY;
		currentTourSpeed -= res;
	}
}

bool AttackMoveUnit::tryToAttack(IAttackable& opponentUnit)
{
	const int res = calculateDistanceFromCurrentPos(opponentUnit.getPositionX(), opponentUnit.getPositionY());
	const bool eval = res <= range && currentTourSpeed > 0;

	if (eval)
	{
		currentTourSpeed--;
		opponentUnit.getDamaged(getDamage(getLetter(), opponentUnit.getLetter()));
	}

	return eval;
}

/*
AttackMoveUnit& AttackMoveUnit::refreshForNewTourAndReturnThis(int leftOverDurability)
{
	durability = leftOverDurability;
	return *this;
}
*/

char AttackMoveUnit::getLetter()
{
	return classLetter;
}

int AttackMoveUnit::getSpeed() const
{
	return currentTourSpeed;
}

int getDamage(char u1, char u2)
{
	return attackTable[unitsToIndexInAttackTable.at(u1)][unitsToIndexInAttackTable.at(u2)];
}

int AttackMoveUnit::getRange() const
{
	return range;
}

int AttackMoveUnit::calculateDistanceFromCurrentPos(int posX, int posY) const
{
	return calculateDistanceBetweenTwoPositions(positionX, posX, positionY, posY);
}

int calculateDistanceBetweenTwoPositions(const int posx1, const int posx2, const int posy1, const int posy2)
{
	return std::abs(posx1 - posx2) + std::abs(posy1 - posy2);
}

