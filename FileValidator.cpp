#include "fileValidator.h"


const std::vector<char> FileValidator::possibleUnitTypes{ 'K', 'S', 'A', 'P', 'R', 'C', 'W', 'B' };

const std::map<char, int> FileValidator::playerTypesToBaseNumbers{
	{'P', 1},
	{'E', 2}
};

const std::map<char, std::vector<char>> FileValidator::allowedActionsToUnitTypes{
	{'A', {'K', 'S', 'A', 'P', 'R', 'C', 'W'}},
	{'A', {'K', 'S', 'A', 'P', 'R', 'C', 'W'}},
	{'B', {'B'}}
};

bool FileValidator::checkIfFilesAreCorrect(const std::string& mapPath, const std::string& statusPath, const std::string& ordersPath)
{
	// reseting the collection with every iteration of the game
	mapRepresentation.clear();


	if (!validateMap(mapPath))
	{
		return false;
	}
	if (!validateStatus(statusPath))
	{
		return false;
	}
	if (!validateOrders(ordersPath))
	{
		return false;
	}

	return true;
}


bool FileValidator::validateStatusLineAndPlaceOnMap(const std::string& line)
{
	//tokenizing the string on space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;

	while (iss >> token)
	{
		tokens.push_back(token);
	}

	const size_t size = tokens.size();

	// only two sizes are possible
	if (size != 8 && size != 7)
	{
		return false;
	}

	std::string unitType = tokens[1];
	std::string owner = tokens[0];
	int unitID = std::stoi(tokens[2]);
	int posX = std::stoi(tokens[3]);
	int posY = std::stoi(tokens[4]);
	int durability = std::stoi(tokens[5]);

	// position in X and Y axis shouldn't go outside the map in any of the directions
	if (posX < 0 || posX > mapRepresentation.size() - 1 || posY < 0 || posY > mapRepresentation[0].size() - 1)
	{
		return false;
	}


	// verifying unit type (whether it is a correct char)
	if (unitType.length() != 1 || std::find(possibleUnitTypes.begin(), possibleUnitTypes.end(), unitType[0]) == possibleUnitTypes.end())
	{
		return false;
	}


	// checking if the base produces/is able to produce units correctly
	if (size == 8)
	{
		std::string productionUnit = tokens[6];
		if (productionUnit.length() != 1 || (productionUnit[0] != '0' && std::find(possibleUnitTypes.begin(), possibleUnitTypes.end(), productionUnit[0]) == possibleUnitTypes.end()))
		{
			return false;
		}
	}

	return placeOnMapAndVerifyIfOwnershipOverlap(posX, posY, owner);
}

bool FileValidator::placeOnMapAndVerifyIfOwnershipOverlap(int posX, int posY, std::string owner)
{
	int currElem = mapRepresentation[posX][posY];

	std::vector<int> keys;
	for (const auto& [k, v] : playerTypesToBaseNumbers) {
		keys.push_back(k);
	}

	// verifying owner (whether it is a correct char)
	if (owner.length() != 1 || std::find(keys.begin(), keys.end(), owner[0]) == keys.end())
	{
		return false;
	}


	if (currElem != '0' && currElem != owner[0] && currElem != playerTypesToBaseNumbers.at(owner[0]))
	{
		return false;
	}

	mapRepresentation[posX][posY] = owner[0];

	return true;
}

bool FileValidator::validateMap(const std::string& mapPath)
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
	for (const auto& [k, v] : playerTypesToBaseNumbers) {
		values.push_back(v);
	}

	std::string possibleChars(values.begin(), values.end());
	possibleChars += "096";

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

bool FileValidator::validateStatus(const std::string& statusPath)
{
	std::ifstream statusFile(statusPath);

	if (!statusFile)
	{
		statusFile.close();
		throw std::runtime_error("Status file not found at path: " + statusPath);
	}


	std::string line;
	std::getline(statusFile, line);
	long gold = std::stol(line);

	for (int lineCounter = 0; std::getline(statusFile, line); ++lineCounter)
	{

		// incorrect size or data means that the file is in some way corrupted
		// working with it has no use
		if (!validateStatusLineAndPlaceOnMap(line))
		{
			statusFile.close();
			return false;
		}
	}
	return true;
}

bool FileValidator::validateOrders(const std::string& ordersPath)
{
	return true;
}
