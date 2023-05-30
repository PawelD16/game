#include <IAttackable.h>

IAttackable::IAttackable(
	int durability,
	int positionX,
	int positionY,
	int unitID
) :
	durability(durability),
	positionX(positionX),
	positionY(positionY),
	unitID(unitID)
{
}

void IAttackable::getDamaged(int damage)
{
	durability -= damage;
}

int IAttackable::getDurability() const
{
	return durability;
}

int IAttackable::getPositionX() const
{
	return positionX;
}

int IAttackable::getPositionY() const
{
	return positionY;
}

int IAttackable::getUnitID() const
{
	return unitID;
}