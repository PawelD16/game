#include "FileValidator.h"

bool FileValidator::checkIfFilesAreCorrect(const std::string &mapPath, const std::string &statusPath, const std::string &ordersPath)
{
	// reseting the collection with every iteration of the game
	mapRepresentation.clear();
	units.clear();
	hasAttacked = false;

	return !validateMap(mapPath) && !validateStatus(statusPath) && !validateOrders(ordersPath);
}

bool FileValidator::validateStatusLineAndPlaceOnMap(const std::string &line)
{

	std::vector<std::string> tokens = tokenizeOnSpaces(line);

	const size_t size = tokens.size();

	// only two sizes are possible
	if (size != 6 && size != 7)
	{
		return false;
	}

	std::string owner = tokens[0];
	std::string unitType = tokens[1];

	if (!isNaturalNumber(tokens[2]) || !isNaturalNumber(tokens[3]) || !isNaturalNumber(tokens[4]))
	{
		return false;
	}

	int unitID = std::stoi(tokens[2]);
	int posX = std::stoi(tokens[3]);
	int posY = std::stoi(tokens[4]);
	int durability = std::stoi(tokens[5]);
	char productionUnit;

	// position in X and Y axis shouldn't go outside the map in any of the directions
	if (posX < 0 || posX > mapRepresentation.size() - 1 || posY < 0 || posY > mapRepresentation[0].size() - 1)
	{
		return false;
	}

	// verifying unit type (whether it is a correct char)
	if (unitType.length() != 1 || std::find(POSSIBLE_UNIT_TYPES.begin(), POSSIBLE_UNIT_TYPES.end(), unitType[0]) == POSSIBLE_UNIT_TYPES.end())
	{
		return false;
	}

	// checking if durability isnt skewed
	if (durability <= UNIT_DURABILITY_MAP.at(unitType[0]) && durability > 0)
	{
		return false;
	}

	// checking if the base produces/is able to produce (has a '0') units correctly
	if (size == 7)
	{
		std::string productionTest = tokens[6];
		if (productionTest.length() != 1 || (productionTest[0] != NOT_PRODUCING && std::find(POSSIBLE_UNIT_TYPES.begin(), POSSIBLE_UNIT_TYPES.end(), productionTest[0]) == POSSIBLE_UNIT_TYPES.end()))
		{
			return false;
		}

		productionUnit = productionTest[0];
	}
	else
	{
		productionUnit = NOT_PRODUCING;
	}

	std::tuple<char, char, int, int, char, int> unitTuple(owner[0], unitType[0], posX, posY, productionUnit, UNIT_SPEED.at(unitType[0]));
	auto result = units.try_emplace(unitID, unitTuple);

	// result is a tuple with an iterator and a bool with information about success of emplacing the element.
	// emplacing fails if unit with similar id exists already (which shouldn't happpen)
	return result.second && placeOnMapAndVerifyIfOwnershipOverlap(posX, posY, owner);
}

bool FileValidator::placeOnMapAndVerifyIfOwnershipOverlap(int posX, int posY, std::string owner)
{
	char currElem = mapRepresentation[posY][posX];

	std::vector<char> keys;
	for (const auto &kv : PLAYER_TYPE_TO_BASE_NUM)
	{
		keys.push_back(kv.first);
	}

	// verifying owner (whether it is a correct char)
	if (owner.length() != 1 || std::find(keys.begin(), keys.end(), owner[0]) == keys.end())
	{
		return false;
	}

	if (currElem != NEUTRAL_POSITION && currElem != owner[0] && currElem != PLAYER_TYPE_TO_BASE_NUM.at(owner[0]))
	{
		return false;
	}

	mapRepresentation[posY][posX] = owner[0];

	return true;
}

bool FileValidator::validateMap(const std::string &mapPath)
{
	std::ifstream mapFile(mapPath);

	if (!mapFile)
	{
		mapFile.close();
		throw std::runtime_error("Map file not found at path: " + mapPath);
	}

	// could have used just a "09612" string
	// but doing it this way makes sure that is will work
	// even if we were to add more players in the future
	std::vector<int> values;
	for (const auto &kv : PLAYER_TYPE_TO_BASE_NUM)
	{
		values.push_back(kv.second);
	}

	std::string possibleChars(values.begin(), values.end());
	possibleChars += NEUTRAL_POSITION;
	possibleChars += FORBINDDEN_POSITION + MINE_POSITION;

	std::string line;

	for (int lineCounter = 0; std::getline(mapFile, line); ++lineCounter)
	{
		std::vector<char> data(line.begin(), line.end());
		mapRepresentation[lineCounter] = std::move(data);

		// checking whether every line contains the same amount of chars
		if (mapRepresentation[lineCounter].size() != mapRepresentation[0].size())
		{
			mapFile.close();
			return false;
		}

		// checking whether there aren't any character that shouldn't be there
		// if there were that would mean that the last player to interact with the file changed it
		// even though that isn't allowed
		if (line.find_first_not_of(possibleChars) != std::string::npos)
		{
			return false;
		}
	}

	return true;
}

bool FileValidator::validateStatus(const std::string &statusPath)
{
	std::vector<std::string> status = readLinesFromFile(statusPath);

	// checking if the first line is indeed a number
	if (!isNaturalNumber(status[0]))
	{
		return false;
	}

	gold = std::stol(status[0]);

	for (int i = 1; static_cast<size_t>(i) < status.size(); ++i)
	{

		// incorrect size or data means that the file is in some way corrupted
		// working with it has no use
		if (!validateStatusLineAndPlaceOnMap(status[i]))
		{
			return false;
		}
	}

	return true;
}

bool FileValidator::validateOrders(const std::string &ordersPath)
{

	std::vector<std::string> orders = readLinesFromFile(ordersPath);

	std::vector<char> allowedActions;
	for (const auto &kv : ALLOWED_ACTIONS_BY_UNIT_TYPE)
	{
		allowedActions.push_back(kv.first);
	}
	for (const std::string &order : orders)
	{
		std::vector<std::string> tokenizedOrder = tokenizeOnSpaces(order);

		// checking if action type is correct and checking unitID is a number
		if (tokenizedOrder[1].size() != 1 ||
			std::find(allowedActions.begin(), allowedActions.end(), tokenizedOrder[1][0]) == allowedActions.end() ||
			!isNaturalNumber(tokenizedOrder[0]))
		{
			return false;
		}

		auto performingUnit = units.find(std::stoi(tokenizedOrder[0]));

		if (performingUnit == units.end())
		{
			return false;
		}

		char pOwner, eOwner;
		char pType, eType;
		int pX, eX;
		int pY, eY;
		char pBuilds, eBuilds;
		int pSpeed, eSpeed;

		std::tie(pOwner, pType, pX, pY, pBuilds, pSpeed) = performingUnit->second;

		switch (tokenizedOrder[1][0])
		{
		case ATTACK_ACTION:
		{
			auto canAttack = ALLOWED_ACTIONS_BY_UNIT_TYPE.at(ATTACK_ACTION);

			if (!hasAttacked || std::find(canAttack.begin(), canAttack.end(), pType) != canAttack.end() || // can this unit even attack?
				tokenizedOrder.size() != 3 ||
				!isNaturalNumber(tokenizedOrder[2]))
			{
				return false;
			}

			auto attackedUnit = units.find(std::stoi(tokenizedOrder[2]));

			if (attackedUnit == units.end())
			{
				return false;
			}

			std::tie(eOwner, eType, eX, eY, eBuilds, eSpeed) = attackedUnit->second;

			if (eOwner == pOwner || calculateDistance(pX, pY, eX, eY) > UNIT_RANGE.at(pType))
			{
				return false;
			}

			units[std::stoi(tokenizedOrder[0])] = std::make_tuple(pOwner, pType, pX, pY, pBuilds, pSpeed - 1);
			hasAttacked = true;	//only one attacka allowed per tour
			break;
		}
		case MOVE_ACTION:
		{
			auto canMove = ALLOWED_ACTIONS_BY_UNIT_TYPE.at(MOVE_ACTION);

			if (std::find(canMove.begin(), canMove.end(), pType) != canMove.end() || // can this unit even move?
				tokenizedOrder.size() != 4 ||
				!isNaturalNumber(tokenizedOrder[2]) ||
				!isNaturalNumber(tokenizedOrder[3]))
			{
				return false;
			}

			int distance = calculateDistance(pX, pY, eX, eY);
			if (std::stoi(tokenizedOrder[2]) < mapRepresentation[0].size() ||
				std::stoi(tokenizedOrder[3]) < mapRepresentation.size() ||
				distance > pSpeed ||
				mapRepresentation[pY][pX] != OWNED_BY_TOUR_WATCHER ||
				mapRepresentation[pY][pX] != FORBINDDEN_POSITION)
			{
				return false;
			}

			units[std::stoi(tokenizedOrder[0])] = std::make_tuple(pOwner, pType, pX, pY, pBuilds, pSpeed - distance);
			break;
		}
		case BUILD_ACTION:
			auto canBuild = ALLOWED_ACTIONS_BY_UNIT_TYPE.at(BUILD_ACTION);

			if (std::find(canBuild.begin(), canBuild.end(), pType) != canBuild.end() ||
				pBuilds != NOT_PRODUCING)
			{
				return false;
			}

			break;
		}
	}
	return true;
}
