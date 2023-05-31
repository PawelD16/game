#include <Mediator.h>

Mediator::Mediator(
    const std::string &mapPath,
    const std::string &statusPath,
    const std::string &ordersPath,
    int timeLimit)
    : mapPath(mapPath),
      statusPath(statusPath),
      ordersPath(ordersPath),
      timeLimit(timeLimit),
      player1Gold(DEFAULT_GOLD_AMOUNT),
      player2Gold(DEFAULT_GOLD_AMOUNT),
      isPlayer1Tour(true), // player 1 always starts
      player1Producing(NOT_PRODUCING),
      player2Producing(NOT_PRODUCING)
{

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

void Mediator::generateStartingStatus()
{
    // no need to check if the bases exist and are in a correct amount
    // it is already validated by the validator
    std::vector<std::string> map = readLinesFromFile(mapPath);

    // for finding the bases coords
    std::pair<int, int> tourTakerBase;
    std::pair<int, int> tourWatcherBase;

    std::string line;
    for (int i = 0; static_cast<size_t>(i) < map.size(); ++i)
    {
        std::vector<char> data(map[i].begin(), map[i].end());

        for (int j = 0; static_cast<size_t>(j) < data.size(); ++j)
        {
            std::pair<int, int> tempPair = {j, i};
            switch (data[j])
            {
            case BASE_OF_TOUR_TAKER:
                tourTakerBase = tempPair;
                break;
            case BASE_OF_TOUR_WATCHER:
                tourWatcherBase = tempPair;
                break;
            default:
                break;
            }
        }
    }

    if (!doesFileExist(statusPath))
    {
        throw std::runtime_error(NO_STATUS_FILE + mapPath);
    }

    std::ofstream statusFile(statusPath);

    statusFile << DEFAULT_GOLD_AMOUNT << std::endl;
    statusFile << OWNED_BY_TOUR_TAKER << " " << BASE_CLASS_LETTER << " " << 0 << " " << tourTakerBase.first << " " << tourTakerBase.second << " " << BASE_DURABILITY << " " << NOT_PRODUCING << std::endl;
    statusFile << OWNED_BY_TOUR_WATCHER << " " << BASE_CLASS_LETTER << " " << 1 << " " << tourWatcherBase.first << " " << tourWatcherBase.second << " " << BASE_DURABILITY << " " << NOT_PRODUCING << std::endl;

    statusFile.close();
}

// this has to be run after applying the changes to the status map (last step before giving the files to the next player)
// switching the base numbers and unit symbols (and gold), as programs think that are always THE player
void Mediator::switchTourAndDataForOtherPlayer()
{
    // no need to check if the bases exist and are in a correct amount
    // it is already validated by the validator

    std::vector<std::string> map = readLinesFromFile(mapPath);

    // switching base numbers
    for (int i = 0; static_cast<size_t>(i) < map.size(); ++i)
    {
        for (int j = 0; static_cast<size_t>(j) < map[i].size(); ++j)
        {
            switch (map[i][j])
            {
            case BASE_OF_TOUR_TAKER:
                map[i][j] = BASE_OF_TOUR_WATCHER;
                break;
            case BASE_OF_TOUR_WATCHER:
                map[i][j] = BASE_OF_TOUR_TAKER;
                break;
            default:
                break;
            }
        }
    }

    printLinesToFile(mapPath, map);

    std::vector<std::string> status = readLinesFromFile(statusPath);

    status[0] = std::to_string((isPlayer1Tour ? player2Gold : player1Gold));

    for (int i = 1; static_cast<size_t>(i) < status.size(); ++i)
    {
        if (status[i][0] == OWNED_BY_TOUR_TAKER)
        {
            status[i][0] = OWNED_BY_TOUR_WATCHER;
        }
        else
        {
            status[i][0] = OWNED_BY_TOUR_TAKER;
        }
    }

    printLinesToFile(statusPath, status);

    isPlayer1Tour = !isPlayer1Tour;
}

bool Mediator::updateGameStatusWithOrdersAndValidateWinner()
{
    std::vector<std::string> orders = readLinesFromFile(ordersPath);

    std::vector<std::string> tempUnits = readLinesFromFile(statusPath);
    
    std::map<int, std::vector<std::string>> tokenizedUnits; // map of units by their ID
    int currentBaseTourID = 0;
    int watcherBaseTourID = 0;

    for (int i = 1; static_cast<size_t>(i) < tempUnits.size(); ++i)
    {
        std::vector<std::string> tokens = tokenizeOnSpaces(tempUnits[i]);
        // 2 is the position of unitID
        tokenizedUnits.emplace(std::stoi(tokens[2]), tokens);

        // these will always be a single char, so taking [0] allows us to get this char
        if (tokens[0][0] == OWNED_BY_TOUR_TAKER && tokens[1][0] == BASE_CLASS_LETTER)
        {
            currentBaseTourID = std::stoi(tokens[2]);
        }
        else if (tokens[0][0] == OWNED_BY_TOUR_WATCHER && tokens[1][0] == BASE_CLASS_LETTER)
        {
            watcherBaseTourID = std::stoi(tokens[2]);
        }
    }

    std::string player1X = (isPlayer1Tour ? tokenizedUnits[currentBaseTourID][3] : tokenizedUnits[watcherBaseTourID][3]);
    std::string player1Y = (isPlayer1Tour ? tokenizedUnits[currentBaseTourID][4] : tokenizedUnits[watcherBaseTourID][4]);
    std::string player2X = (!isPlayer1Tour ? tokenizedUnits[currentBaseTourID][3] : tokenizedUnits[watcherBaseTourID][3]);
    std::string player2Y = (!isPlayer1Tour ? tokenizedUnits[currentBaseTourID][4] : tokenizedUnits[watcherBaseTourID][4]);

    std::vector<std::string> possibleNewUnits = addUnit(
        player1X,
        player1Y,
        player2X,
        player2Y,
        static_cast<int>(tokenizedUnits.size())
        );

    // do actions
    for (const std::string &order : orders)
    {
        std::vector<std::string> tokenizedOrder = tokenizeOnSpaces(order);

        doSingleAction(tokenizedUnits, tokenizedOrder, currentBaseTourID);
    }

    std::vector<std::string> newStatus;
    newStatus.push_back(std::to_string(isPlayer1Tour ? player1Gold : player2Gold));
    for (const auto &kv : tokenizedUnits)
    {
        // deleting (by not adding) killed units
        if (std::stoi(kv.second[5]) <= 0)
        {
            // since we have validated the file the unit could have only attacked enbemy base
            // so checking if the opponents base is dead
            if (kv.second[0][0] == OWNED_BY_TOUR_WATCHER && kv.second[1][0] == BASE_CLASS_LETTER)
            {
                return true;
            }
            continue;
        }

        // recreating the units from vector to proper string
        std::string res;
        for (size_t i = 0; i < kv.second.size(); ++i)
        {
            res += kv.second[i];

            if (i != kv.second.size() - 1)
            {
                res += " ";
            }
        }

        newStatus.push_back(res);
    }

    newStatus.insert(newStatus.end(), possibleNewUnits.begin(), possibleNewUnits.end());

    printLinesToFile(statusPath, newStatus);
    return false;
}

void Mediator::doSingleAction(std::map<int, std::vector<std::string>> &tokenizedUnits, std::vector<std::string> &tokenizedOrder, int currentBaseID)
{
    // correctness of action types is already checked
    // here 1 is action type
    switch (tokenizedOrder[1][0])
    {
    case ATTACK_ACTION:
        // 0 is attacker unitID, 2 is attacked unitID, 5 is durability, 1 is unit type, which determines the damage
        tokenizedUnits[std::stoi(tokenizedOrder[2])][5] =
            std::to_string(
                std::stoi(tokenizedUnits[std::stoi(tokenizedOrder[2])][5]) -
                ATTACK_TABLE.at(
                    UNITS_TO_INDEX_FOR_ATTACK_TABLE.at(
                        tokenizedUnits[std::stoi(tokenizedOrder[0])][1][0])
                    ).at(
                    UNITS_TO_INDEX_FOR_ATTACK_TABLE.at(
                        tokenizedUnits[std::stoi(tokenizedOrder[2])][1][0]))
                    );
        break;
    case MOVE_ACTION:
        tokenizedUnits[std::stoi(tokenizedOrder[0])][3] = tokenizedOrder[2];
        tokenizedUnits[std::stoi(tokenizedOrder[0])][4] = tokenizedOrder[3];
        break;
    // we already checked whether the we can build
    case BUILD_ACTION:
        tokenizedUnits[currentBaseID][6] = tokenizedOrder[2];

        if (isPlayer1Tour)
        {
            player1Producing = tokenizedOrder[2][0];
            player1Gold -= UNIT_COST_TABLE.at(tokenizedOrder[2][0]);
            player1ProductionTime = UNIT_BUILD_TIME_TABLE.at(tokenizedOrder[2][0]);
        }
        else
        {
            player2Producing = tokenizedOrder[2][0];
            player2Gold -= UNIT_COST_TABLE.at(tokenizedOrder[2][0]);
            player2ProductionTime = UNIT_BUILD_TIME_TABLE.at(tokenizedOrder[2][0]);
        }
        break;
    }
}

std::vector<std::string> Mediator::addUnit(std::string player1BaseX, std::string player1BaseY, std::string player2BaseX, std::string player2BaseY, int lastUnitID)
{
    std::vector<std::string> res;

    auto processPlayerProduction = [&](char& playerProducing, int& playerProductionTime, bool tour, std::string& playerBaseX, std::string& playerBaseY)
    {
        if (playerProducing != NOT_PRODUCING)
        {
            if (playerProductionTime <= 0)
            {
                std::string line;
                line += (tour ? OWNED_BY_TOUR_TAKER : OWNED_BY_TOUR_WATCHER);
                line += " ";
                line += playerProducing;
                line +=  " " +
                        std::to_string(lastUnitID++) + " " +
                        playerBaseX + " " +
                        playerBaseY + " " +
                        std::to_string(UNIT_DURABILITY_MAP.at(playerProducing)) + "\n";

                playerProducing = NOT_PRODUCING;
                res.push_back(line);
            }
        }
    };
    --player1ProductionTime;
    --player2ProductionTime;
    processPlayerProduction(player1Producing, player1ProductionTime, isPlayer1Tour, player1BaseX, player1BaseY);
    processPlayerProduction(player2Producing, player2ProductionTime, !isPlayer1Tour, player2BaseX, player2BaseY);

    return res;
}

std::pair<int, int> Mediator::tallyUpUnits()
{
    int player1UnitCounter = 0;
    int player2UnitCounter = 0;
    char player1CurrentChar = (isPlayer1Tour? OWNED_BY_TOUR_TAKER: OWNED_BY_TOUR_WATCHER);

    std::vector<std::string> tempUnits = readLinesFromFile(statusPath);

    for (int i = 1; static_cast<size_t>(i) < tempUnits.size(); ++i)
    {
        std::vector<std::string> tokens = tokenizeOnSpaces(tempUnits[i]);
    
         // File validator chacks taht only two types of units exist
        if (tokens[0][0] == player1CurrentChar)
        {
            ++player1UnitCounter;
        }
        else 
        {
            ++player2UnitCounter;
        }
    }

    return {player1UnitCounter, player2UnitCounter};
}
