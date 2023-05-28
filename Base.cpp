#include "Base.h"

Base::Base() :
	IAttackable(
		BASE_DURABILITY,
		0,
		0,
		0
	)
{
}

Base::Base(
	int durability,
	int positionX,
	int positionY,
	int unitID
) :
	IAttackable(
		durability,
		positionX,
		positionY,
		unitID
	)
{
}

char Base::getLetter()
{
	return BASE_CLASS_LETTER;
}

