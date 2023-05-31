#pragma once
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>
#include <algorithm>

#include "consts.h"
#include "utils.h"


class FileValidator
{
public:
	FileValidator() = default;
	~FileValidator() = default;

	bool checkIfFilesAreCorrect(const std::string& mapPath, const std::string& statusPath, const std::string& ordersPath);

private:

	bool validateMap(const std::string& mapPath);
	bool validateStatus(const std::string& statusPath);
	bool validateOrders(const std::string& ordersPath);

	// represents the map on a 2d vector
	std::vector<std::vector<char>> mapRepresentation;
	bool validateStatusLineAndPlaceOnMap(const std::string& line);

	// placing on map to verify the status
	// (whether one of the players corrupted the file in any way)
	// and to check orders next (gather units by owner)
	bool placeOnMapAndVerifyIfOwnershipOverlap(int posX, int posY, std::string owner);

	// store units to verify actions
	//								  owner, type,  x,  y,builds, leftOverSpeed 
	std::unordered_map<int, std::tuple<char, char, int, int, char, int>> units;

	long gold;
	bool hasAttacked;
};

