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
    std::ifstream mapFileRead(mapPath);

    if (!mapFileRead)
    {
        mapFileRead.close();
        throw std::runtime_error(NO_MAP_FILE + mapPath);
    }

    // finding the bases
    std::pair<int, int> tourTakerBase;
    std::pair<int, int> tourWatcherBase;

    std::string line;
    for (int i = 0; std::getline(mapFileRead, line); ++i)
    {
        std::vector<char> data(line.begin(), line.end());

        for (int j = 0; static_cast<size_t>(j) < data.size(); ++j)
        {
            std::pair<int, int> tempPair = {i, j};
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

    mapFileRead.close();

    if (!doesFileExist(statusPath))
    {
        throw std::runtime_error(NO_STATUS_FILE + mapPath);
    }

    std::ofstream statusFile(statusPath);

    statusFile << DEFAULT_GOLD_AMOUNT << std::endl;
    statusFile << BASE_OF_TOUR_TAKER << " " << 0 << " " << tourTakerBase.first << " " << tourTakerBase.second << " " << BASE_DURABILITY << " " << NOT_PRODUCING << std::endl;
    statusFile << BASE_OF_TOUR_WATCHER << " " << 1 << " " << tourWatcherBase.first << " " << tourWatcherBase.second << " " << BASE_DURABILITY << " " << NOT_PRODUCING << std::endl;

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

    if (isPlayer1Tour)
    {
        status[0] = std::to_string(player2Gold);
    }
    else
    {
        status[0] = std::to_string(player1Gold);
    }

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

void Mediator::updateGameStatusWithOrders()
{
    std::vector<std::string> orders = readLinesFromFile(ordersPath);

    std::vector<std::string> tempUnits = readLinesFromFile(statusPath);
    std::map<int, std::vector<std::string>> tokenizedUnits;    //map of units by their ID
    int currentBaseTourID = 0 ;
    int enemyBaseTourID = 0;

    for (int i = 1; static_cast<size_t>(i) < tempUnits.size(); ++i)
    {
		std::vector<std::string> tokens = tokenizeOnSpaces(tempUnits[i]);
                                    // 2 is the position of unitID
        tokenizedUnits.emplace(std::stoi(tokens[2]), tokens);

        if (tokens[0] == OWNED_BY_TOUR_TAKER && tokens[1] == BASE_CLASS_LETTER)
        {
            currentBaseTourID = std::stoi(tokens[2]);
        }
        else if (tokens[0] == OWNED_BY_TOUR_WATCHER && tokens[1] == BASE_CLASS_LETTER)
        {
            enemyBaseTourID = std::stoi(tokens[2]); 
        }
    }

    std::vector<std::string> possibleNewUnits = addUnit(
        tokenizedUnits[currentBaseTourID][3], 
        tokenizedUnits[currentBaseTourID][4], 
        tokenizedUnits[enemyBaseTourID][3], 
        tokenizedUnits[enemyBaseTourID][4],
        tokenizedUnits.size());

    // do actions
    for(const std::string& order : orders)
    {
        std::vector<std::string> tokenizedOrder = tokenizeOnSpaces(order);

        doSingleAction(tokenizedUnits, tokenizedOrder, currentBaseTourID);
    }

    std::vector<std::string> newStatus;
    for (const auto& kv : tokenizedUnits) 
    {
        std::string res;
        for (size_t i = 0; i < kv.second.size(); ++i) 
        {
        res += kv.second[i];
        
        if (i != kv.second.size() - 1) {
            res += " ";
        }
        
    }
    newStatus.push_back(res);
    }

    newStatus.insert(newStatus.end(), possibleNewUnits.begin(), possibleNewUnits.begin());
    
    printLinesToFile(statusPath, newStatus);
}

void Mediator::doSingleAction(std::map<int, std::vector<std::string>>& tokenizedUnits, std::vector<std::string>& tokenizedOrder, int currentBaseID)
{
    // correctness of action types is already checked
    // here 1 is action type
    switch(tokenizedOrder[1]){
        case ATTACK_ACTION:
            // 0 is attacker unitID, 2 is attacked unitID, 5 is durability                                                1 is unit type, which determines the damage
            tokenizedUnits[2][5] = std::to_string(std::stoi(tokenizedUnits[2][5]) - ATTACK_TABLE[tokenizedUnits[tokenizedOrder[0]][1]][tokenizedUnits[tokenizedOrder[2]][1]]);
            break;
        case MOVE_ACTION:
            tokenizedUnits[2][3] = tokenizedOrder[2];
            tokenizedUnits[2][4] = tokenizedOrder[3];
            break;
        // we already checked whether the we can build 
        case BUILD_ACTION:
            tokenizedUnits[currentBaseID][6] = tokenizedOrder[2];

            if (isPlayer1Tour)
            {
                player1Producing = tokenizedOrder[2];
                player1Gold -= UNIT_COST_TABLE[tokenizedOrder[2]];
                player1ProductionTime = UNIT_BUILD_TIME_TABLE[tokenizedOrder[2]];
            }
            else
            {
                player2Producing = tokenizedOrder[2];
                player2Gold -= UNIT_COST_TABLE[tokenizedOrder[2]];
                player2ProductionTime = UNIT_BUILD_TIME_TABLE[tokenizedOrder[2]];
            }
            break;
    }
}

std::vector<std::string> Mediator::addUnit(std::string player1BaseX, std::string player1BaseY, std::string player2BaseX, std::string player2BaseY, int lastUnitID)
{
    std::vector<std::string> res;
    
    if (player1Producing != NOT_PRODUCING)
    {
        --player1ProductionTime;
        
        if (player1ProductionTime <= 0)
        {
            std::string line;
            if (isPlayer1Tour)
            {
                line += OWNED_BY_TOUR_TAKER;
            }
            else
            {
                line += OWNED_BY_TOUR_WATCHER;
            }
            
            line += 
            " " +
            player1Producing +
            " " +
            std::to_string(lastUnitID++) +
            " " +
            player1BaseX +
            " " +
            player2BaseY +
            " " +
            UNIT_DURABILITY_MAP[player1Producing] +
            std::endl;

            player1Producing = NOT_PRODUCING;
            res.push_back(line);
        }
    }

    if (player2Producing != NOT_PRODUCING)
    {
        --player2ProductionTime;
        
        if (player2ProductionTime <= 0)
        {
            std::string line;
            if (!isPlayer1Tour)
            {
                line += OWNED_BY_TOUR_TAKER;
            }
            else
            {
                line += OWNED_BY_TOUR_WATCHER;
            }
            
            line += 
            " " +
            player2Producing +
            " " +
            std::to_string(lastUnitID) +
            " " +
            player2BaseX +
            " " +
            player2BaseY +
            " " +
            UNIT_DURABILITY_MAP[player2Producing] +
            std::endl;

            player2Producing = NOT_PRODUCING;
            res.push_back(line);
        }
    }

    return res;
}
