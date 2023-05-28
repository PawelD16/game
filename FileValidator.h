#pragma once
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>


class FileValidator
{
public:

	bool checkIfFilesAreCorrect(const std::string& mapPath, const std::string& statusPath, const std::string& ordersPath);
private:

	// this collection allows for easy verification of existing unit types
	static const std::vector<char> possibleUnitTypes;

	// this collection allows for easy verification of existing players and their base numbers
	// It could be easily expanded if there were to be more players
	static const std::map<char, int> playerTypesToBaseNumbers;

	// maps actions to units that are allowed to do them
	static const std::map<char, std::vector<char>> allowedActionsToUnitTypes;

	// represents the map on a 2d vector
	std::vector<std::vector<char>> mapRepresentation;
	bool validateStatusLineAndPlaceOnMap(const std::string& line);

	// placing on map to verify the status
	// (whether one of the players corrupted the file in any way)
	// and to check orders next (gather units by owner)
	bool placeOnMapAndVerifyIfOwnershipOverlap(int posX, int posY, std::string owner);

	// that collects units to check their id and verify their action

	bool validateMap(const std::string& mapPath);
	bool validateStatus(const std::string& statusPath);
	bool validateOrders(const std::string& ordersPath);
};

