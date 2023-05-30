#pragma once

class IAttackable
{
public:
	IAttackable(
		int durability,
		int positionX,
		int positionY,
		int unitID
	);
	virtual ~IAttackable() = default;
	virtual char getLetter() = 0;

	void getDamaged(int damage);
	int getDurability() const;

	int getPositionX() const;

	int getPositionY() const;

	int getUnitID() const;

protected:
	int durability;
	int positionX;
	int positionY;
	int unitID;
};
