#include "Worker.h"

Worker::Worker(int durability, int positionX, int positionY, int unitID) :
	AttackMoveUnit(
		durability,
		WORKER_SPEED,
		WORKER_COST,
		WORKER_RANGE,
		WORKER_BUILD_TIME,
		WORKER_CLASS_LETTER,
		positionX,
		positionY,
		unitID
	)
{
}
