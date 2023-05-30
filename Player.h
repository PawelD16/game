#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <random>

#include "UnitStatsConsts.h"
#include "AttackMoveUnit.h"
#include "Knight.h"
#include "Swordsman.h"
#include "Archer.h"
#include "Pikeman.h"
#include "Ram.h"
#include "Catapult.h"
#include "Worker.h"
#include "Base.h"

class Player
{
public:
	Player(
		const std::string& mapPath,
		const std::string& statusPath,
		const std::string& ordersPath,
		int timeLimit
	);

	void takeATurn();

private:

	// factory that allows creation of objects from their char
	static const std::map<char, std::function<AttackMoveUnit(int durability, int posX, int posY, int unitID)>> unitFactory;

	std::vector<AttackMoveUnit> ownUnits;
	std::vector<AttackMoveUnit> opponentUnits;
	Base playerBase;
	Base opponentBase;

	std::vector<std::pair<int, int>> availableMines;
	std::vector<std::vector<char>> map;

	std::string mapPath;
	std::string statusPath;
	std::string ordersPath;

	int timeLimit;
	long gold;
	char producing{};

	static bool isMineAvailable(int posX, int posY, const std::vector<AttackMoveUnit>& opponentWorkers);
	bool isSecondLeftOfTimeLimit(const std::chrono::steady_clock::time_point& startTime) const;
	void writeOrders(const std::vector<std::string>& orders) const;

	void drawMapAndGatherAvailableMines();
	void gatherCurrentStatus();
	void createUnitFromLine(const std::vector<std::string>& tokens);
	void tryToProduce(std::vector<std::string>& orders);

	std::vector<std::vector<int>> createMapClipForUnit(const AttackMoveUnit& unit) const;
	int getVerifiedMapClipValue(int posX, int posY, int currPosX, int currPosY, int maxRange) const;

};

bool doesFileExist(const std::string& filePath);
bool isAWorker(AttackMoveUnit& unit);
bool damageComparator(AttackMoveUnit& u1, AttackMoveUnit& u2);
bool distanceFromBaseComparator(std::pair<int, int> coordinates1, std::pair<int, int> coordinates2, Base& base);

