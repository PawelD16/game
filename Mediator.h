#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

#include "consts.h"
#include "FileValidator.h"
#include "utils.h"

class Mediator
{
public:
    Mediator(
        const std::string &mapPath,
        const std::string &statusPath,
        const std::string &ordersPath,
        int timeLimit);

    std::string mapPath;
    std::string statusPath;
    std::string ordersPath;

private:
    int timeLimit;
    int player1Gold;
    int player2Gold;

    char player1Producing;
    char player2Producing;

    int player1ProductionTime;
    int player2ProductionTime;

    bool isPlayer1Tour;

    void generateStartingStatus();
    void switchTourAndDataForOtherPlayer();
    void updateGameStatusWithOrders();

    void doSingleAction(std::map<int, std::vector<std::string>>& tokenizedUnits, std::vector<std::string>& tokenizedOrder, int currectTourBaseID);

    std::vector<std::string> addUnit(std::string player1BaseX, std::string player1BaseY, std::string player2BaseX, std::string player2BaseY, int lastUnitID);

    FileValidator validator;
};