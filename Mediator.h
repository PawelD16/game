#pragma once
#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>

#include "UnitStatsConsts.h"
#include "FileValidator.h"

class Mediator
{
public:
    Mediator(
        const std::string &mapPath,
        const std::string &statusPath,
        const std::string &ordersPath,
        int timeLimit
        );

    std::string mapPath;
	std::string statusPath;
	std::string ordersPath;

	int timeLimit;
    int player1Gold;
    int player2Gold;
    bool isPlayer1Tour;

    void generateStartingStatus();
    void switchTourAndDataForOtherPlayer();
    void updateGameStatusWIthOrders();

    FileValidator validator{};

};

bool doesFileExist(const std::string& filePath);
bool printLinesToFile(const std::string& filePath, std::vector<std::string>& toPrint);