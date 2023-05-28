#pragma once

class IAttackable
{
public:
	IAttackable(
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
	virtual ~IAttackable() = default;
	virtual char getLetter() = 0;

	void getDamaged(int damage)
	{
		durability -= damage;
	}

	int getDurability() const
	{
		return durability;
	}

	int getPositionX() const
	{
		return positionX;
	}

	int getPositionY() const
	{
		return positionY;
	}

	int getUnitID() const
	{
		return unitID;
	}

protected:
	int durability;
	int positionX;
	int positionY;
	int unitID;
};
