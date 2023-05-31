#include "Player.h"

const std::map<char, std::function<AttackMoveUnit(int durability, int posX, int posY, int unitID)>> Player::unitFactory{
	{KNIGHT_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Knight(durability, posX, posY, unitID); }},
	{SWORDSMAN_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Swordsman(durability, posX, posY, unitID); }},
	{ARCHER_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Archer(durability, posX, posY, unitID); }},
	{PIKEMAN_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Pikeman(durability, posX, posY, unitID); }},
	{CATAPULT_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Catapult(durability, posX,  posY, unitID); }},
	{RAM_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Ram(durability, posX, posY, unitID); }},
	{WORKER_CLASS_LETTER, [](int durability, int posX, int posY, int unitID)
	 { return Worker(durability, posX, posY, unitID); }}
};

Player::Player(
	const std::string& mapPath,
	const std::string& statusPath,
	const std::string& ordersPath,
	int timeLimit
) :
	mapPath(mapPath),
	statusPath(statusPath),
	ordersPath(ordersPath),
	timeLimit(timeLimit),
	gold(0)
{
	// all files are necessary so the program must terminate when even one of them doesn't exist
	// the program could create a file, but that would mean that it and its opponent wouldn't
	// have the same files to work with, which makes the game have no meaning

	if (!doesFileExist(mapPath))
	{
		throw std::runtime_error(NO_MAP_FILE + mapPath);
	}

	if (!doesFileExist(statusPath))
	{
		throw std::runtime_error(NO_STATUS_FILE + statusPath);
	}

	if (!doesFileExist(ordersPath))
	{
		throw std::runtime_error(NO_ORDERS_FILE + ordersPath);
	}

	if (timeLimit <= 0)
	{
		throw std::invalid_argument("The player won't be able to make a move with time = " + std::to_string(timeLimit));
	}
}



void Player::drawMapAndGatherAvailableMines()
{
	std::ifstream mapFile(mapPath);

	if (!mapFile)
	{
		mapFile.close();
		throw std::runtime_error(NO_MAP_FILE + mapPath);
	}

	// this is executed after gathering all units, so this is fine
	// filers out availableMines that cannot be used (enemy units on in the mine)
	std::vector<AttackMoveUnit> opponentWorkers;

	if (!opponentUnits.empty())
	{
		std::copy_if(opponentUnits.begin(), opponentUnits.end(), std::back_inserter(opponentWorkers), isAWorker);
	}

	// redrawing the entire map every time is as slow as having it backed up in a local file
	std::string line;
	for (int lineCounter = 0; std::getline(mapFile, line); ++lineCounter)
	{
		map.emplace_back(line.begin(), line.end());

		for (int i = 0; static_cast<size_t>(i) < map[lineCounter].size();++i)
		{
			if (map[lineCounter][i] == MINE_POSITION && isMineAvailable(lineCounter, i, opponentWorkers))
			{
				availableMines.emplace_back(i, lineCounter);
			}
		}
	}

	mapFile.close();
}

void Player::gatherCurrentStatus()
{
	std::ifstream statusFile(statusPath);

	if (!statusFile)
	{
		statusFile.close();
		throw std::runtime_error(NO_STATUS_FILE + statusPath);
	}

	std::string line;
	std::getline(statusFile, line);
	gold = std::stol(line);

	for (int lineCounter = 0; std::getline(statusFile, line); ++lineCounter)
	{
		std::istringstream iss(line);
		std::vector<std::string> tokens;
		std::string token;

		while (iss >> token)
		{
			tokens.push_back(token);
		}

		createUnitFromLine(tokens);
	}

	statusFile.close();
}

bool isAWorker(AttackMoveUnit& unit)
{
	return unit.getLetter() == WORKER_CLASS_LETTER;
}

bool damageComparator(AttackMoveUnit& u1, AttackMoveUnit& u2)
{
	return getDamage(u1.getLetter(), u2.getLetter()) > getDamage(u2.getLetter(), u1.getLetter());
}

bool distanceFromBaseComparator(std::pair<int, int> coordinates1, std::pair<int, int> coordinates2, Base& base)
{
	return calculateDistanceBetweenTwoPositions(base.getPositionX(), coordinates1.first, base.getPositionY(), coordinates1.second)
		< calculateDistanceBetweenTwoPositions(base.getPositionX(), coordinates2.first, base.getPositionY(), coordinates2.second);
}

void Player::writeOrders(const std::vector<std::string>& orders) const
{
	if (!doesFileExist(ordersPath))
	{
		throw std::runtime_error(NO_ORDERS_FILE + ordersPath);
	}

	std::ofstream orderFile(ordersPath);

	for (const auto& order : orders)
	{
		orderFile << order << std::endl;
	}

	orderFile.close();
}


bool Player::isMineAvailable(int posX, int posY, const std::vector<AttackMoveUnit>& opponentWorkers)
{
	for (auto& opponentMiner : opponentWorkers)
	{
		if (opponentMiner.getPositionX() == posX && opponentMiner.getPositionY() == posY)
		{
			return false;
		}
	}
	return true;
}

bool Player::isSecondLeftOfTimeLimit(const std::chrono::high_resolution_clock::time_point& startTime) const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() >= timeLimit * 1000 - 1000;
}

void Player::takeATurn()
{
	const auto startTime = std::chrono::high_resolution_clock::now();

	gatherCurrentStatus();
	drawMapAndGatherAvailableMines();

	std::vector<std::string> orders;

	// this structure is repeated, but this is the easiest way of stopping the program
	// when taking it to an outside method the only way is to throw an exception
	// which is less elegant
	// this also doesn't require an additional thread to time the method
	//______________________________________
	if (isSecondLeftOfTimeLimit(startTime))
	{
		writeOrders(orders);
		return;
	}
	//______________________________________


	std::sort(availableMines.begin(), availableMines.end(), [this](auto&& PH1, auto&& PH2)
	{
		return distanceFromBaseComparator(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2),
		                                  playerBase);
	});

	tryToProduce(orders);

	//______________________________________
	if (isSecondLeftOfTimeLimit(startTime))
	{
		writeOrders(orders);
		return;
	}
	//______________________________________

	for (auto& i : ownUnits)
	{
		std::vector<std::vector<int>> mapClip = createMapClipForUnit(i);
		if (isAWorker(i) && !availableMines.empty())
		{
			i.moveCloserToGoal(availableMines[0].first, availableMines[0].second, mapClip, 0);
		}
		else
		{
			i.moveCloserToGoal(opponentBase.getPositionX(), opponentBase.getPositionY(), mapClip, i.getRange());
		}

		orders.push_back(
			std::to_string(i.getUnitID()) +
			" M " +
			std::to_string(i.getPositionX()) +
			" " +
			std::to_string(i.getPositionY())
		);

		//______________________________________
		if (isSecondLeftOfTimeLimit(startTime))
		{
			writeOrders(orders);
			return;
		}
		//______________________________________
	}

	std::sort(ownUnits.begin(), ownUnits.end(), damageComparator);

	// at first ignoring all other units, attacking the enemies base is most important
	bool hasAttacked = false;

	for (int i = 0;i < ownUnits.size() && !hasAttacked;++i)
	{
		if (ownUnits[i].tryToAttack(opponentBase))
		{
			orders.push_back(
				std::to_string(ownUnits[i].getUnitID()) +
				" A " +
				std::to_string(opponentBase.getUnitID())
			);
			hasAttacked = true;
		}
	}

	//______________________________________
	if (isSecondLeftOfTimeLimit(startTime) || hasAttacked)
	{
		writeOrders(orders);
		return;
	}
	//______________________________________

	//otherwise attack any unit in range
	for (int i = 0; static_cast<size_t>(i) < ownUnits.size() && !hasAttacked;++i)
	{
		for (int j = 0; static_cast<size_t>(j) < opponentUnits.size() && !hasAttacked;++j)
		{
			if (ownUnits[i].tryToAttack(opponentUnits[j]))
			{
				orders.push_back(
					std::to_string(ownUnits[i].getUnitID()) +
					" A " +
					std::to_string(opponentUnits[j].getUnitID())
				);
				hasAttacked = true;
			}

			//______________________________________
			if (isSecondLeftOfTimeLimit(startTime))
			{
				writeOrders(orders);
				return;
			}
			//______________________________________
		}
	}

	writeOrders(orders);
}

std::vector<std::vector<int>> Player::createMapClipForUnit(const AttackMoveUnit& unit) const
{
	const int size = unit.getSpeed() * 2 + 1;

	std::vector<std::vector<int>> mapClip(size, std::vector<int>(size, 0));

	for (int i = 0;i < size; ++i)
	{
		for (int j = 0;j < size;++j)
		{
			mapClip[j][i] = getVerifiedMapClipValue(
				j - unit.getSpeed() + unit.getPositionX(),
				i - unit.getSpeed() + unit.getPositionY(),
				unit.getPositionX(),
				unit.getPositionY(),
				unit.getSpeed());
		}
	}

	return mapClip;
}

int Player::getVerifiedMapClipValue(int posX, int posY, int currPosX, int currPosY, int speed) const
{
	if (posY < 0 || static_cast<size_t>(posY) >= map.size() ||
		posX < 0 || static_cast<size_t>(posX) >= map[posY].size() ||
		map[posY][posX] == FORBINDDEN_POSITION ||
		calculateDistanceBetweenTwoPositions(posX, currPosX, posY, currPosY) > speed ||
		(posY == opponentBase.getPositionX() && posX == opponentBase.getPositionY())
		)
	{
		return WRONG_POS_ON_MAPCLIP;
	}

	for (const auto& i : opponentUnits)
	{
		if (i.getPositionX() == posX && i.getPositionY() == posY)
		{
			return WRONG_POS_ON_MAPCLIP;
		}
	}

	return 0;
}

void Player::createUnitFromLine(const std::vector<std::string>& tokens)
{
	// no need to verify these, as the mediator takes care of that
	// reevaluating each unit would take up time, which is suboptimal
	const char unitType = tokens[1][0];
	const char owner = tokens[0][0];
	const int unitID = std::stoi(tokens[2]);
	const int posX = std::stoi(tokens[3]);
	const int posY = std::stoi(tokens[4]);
	const int durability = std::stoi(tokens[5]);

	// if there are 7 tokens, the base in an opponentBase
	if (tokens.size() > 6)
	{
		if (owner == FRIENDLY_UNIT_OWNER)
		{
			playerBase = Base(durability, posX, posY, unitID);
			producing = tokens[6][0];
		}
		else
		{
			opponentBase = Base(durability, posX, posY, unitID);
		}
		return;
	}

	// these will throw an error if there is a problem
	// but the mediator is going to take care of the correctness of the files
	// so to speed up the process there is no need to check for correctness either
	if (owner == FRIENDLY_UNIT_OWNER)
	{
		ownUnits.push_back(std::move(unitFactory.at(unitType)(durability, posX, posY, unitID)));
	}
	else
	{
		opponentUnits.push_back(std::move(unitFactory.at(unitType)(durability, posX, posY, unitID)));
	}
}

void Player::tryToProduce(std::vector<std::string>& orders)
{
	std::vector<AttackMoveUnit> workers;

	if (!ownUnits.empty())
	{
		std::copy_if(ownUnits.begin(), ownUnits.end(), std::back_inserter(workers), isAWorker);
	}

	if (producing == NOT_PRODUCING)
	{
		char productionLetter = '0';

		// no need to lower amount of available gold, as producing anything will always take more than 0 tours
		if (availableMines.size() > 2 * workers.size() && gold >= WORKER_COST && gold <= DEFAULT_GOLD_AMOUNT / 2)
		{
			productionLetter = WORKER_CLASS_LETTER;
		}
		else if (gold >= CATAPULT_COST)
		{
			productionLetter = CATAPULT_CLASS_LETTER;
		}
		else if (gold >= RAM_COST)
		{
			productionLetter = RAM_CLASS_LETTER;
		}
		else if (gold >= KNIGHT_COST)
		{
			productionLetter = KNIGHT_CLASS_LETTER;
		}
		else if (gold >= SWORDSMAN_COST && gold >= ARCHER_COST) //in case the cost would change. This will prevent the program from possible debt
		{
			//swordsman and archer cost the same. So choosing one can be done by a coin-flip
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, 1);

			if (dist(gen) == 0)
			{
				productionLetter = SWORDSMAN_CLASS_LETTER;
			}
			else
			{
				productionLetter = ARCHER_CLASS_LETTER;
			}
		}
		else if (gold >= PIKEMAN_COST)
		{
			productionLetter = PIKEMAN_CLASS_LETTER;
		}
		else
		{
			return;
		}

		orders.push_back(
			std::to_string(playerBase.getUnitID()) +
			" B " +
			productionLetter
		);
	}
}

bool doesFileExist(const std::string& filePath)
{
	std::ifstream file(filePath);
	const bool res = file.good();
	file.close();
	return res;
}
