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
      isPlayer1Tour(true)
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

Mediator::generateStartingStatus()
{
    // no need to check if the bases exist and are in a correct amount
    // it is already validated by the validator
    std::ifstream mapFileRead(mapPath);

	if (!mapFileRead)
	{
		mapFileRead.close();
		throw std::runtime_error(NO_MAP_FILE + mapPath);
	}

    //finding the bases
    std::pair<int, int> tourTakerBase;
    std::pair<int, int> tourWatcherBase;

	std::string line;
	for (int i = 0; std::getline(mapFileRead, line); ++i)
	{
		std::vector<char> data(line.begin(), line.end());

		for (int j = 0; static_cast<size_t>(j) < data.size();++j)
		{
			switch(data[j])
            {
                case BASE_OF_TOUR_TAKER
                    tourTakerBase = {i, j};
                    break;
                case BASE_OF_TOUR_WATCHER
                    tourWatcherBase = {i, j};
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
void switchTourAndDataForOtherPlayer()
{
    // no need to check if the bases exist and are in a correct amount
    // it is already validated by the validator
    std::ifstream mapFileRead(mapPath);

	if (!mapFileRead)
	{
		mapFileRead.close();
		throw std::runtime_error(NO_MAP_FILE + mapPath);
	}

    std::vector<std::string> map;
    std::string line;

    //switching base numbers
	for (int i = 0; std::getline(mapFileRead, line); ++i)
	{
		map.push_back(std::vector<char>(line.begin(), line.end()));

		for (int j = 0; static_cast<size_t>(j) < map[i].size();++j)
		{
			switch(map[i][j])
            {
                case BASE_OF_TOUR_TAKER
                    map[i][j] = BASE_OF_TOUR_WATCHER;
                    break;
                case BASE_OF_TOUR_WATCHER
                    map[i][j] = BASE_OF_TOUR_TAKER;
                    break;
                default:
                    break;
            }
		}
	}

	mapFileRead.close();

    printLinesToFile(mapPath, map);

    for (int i = 0; std::getline(mapFileRead, line); ++i)
	{
		map.push_back(std::vector<char>(line.begin(), line.end()));

		for (int j = 0; static_cast<size_t>(j) < map[i].size();++j)
		{
			switch(map[i][j])
            {
                case BASE_OF_TOUR_TAKER
                    map[i][j] = BASE_OF_TOUR_WATCHER;
                    break;
                case BASE_OF_TOUR_WATCHER
                    map[i][j] = BASE_OF_TOUR_TAKER;
                    break;
                default:
                    break;
            }
		}
	}

    std::ifstream statusFileRead(statusPath);

	if (!statusFileRead)
	{
		statusFileRead.close();
		throw std::runtime_error(NO_STATUS_FILE + statusPath);
	}

    // getting rid of the first line with gold amount
	std::getline(statusFileRead, line);

    std::vector<std::string> status;

    if (isPlayer1Tour)
    {
        status.push_back(std::to_string(player2Gold));
    }
    else
    {
        status.push_back(std::to_string(player1Gold));
    }

	for (int i = 0; std::getline(statusFile, line); ++i)
	{
		if (line[0] == OWNED_BY_TOUR_TAKER)
            {
                line[0] = OWNED_BY_TOUR_WATCHER;
            }
            else
            {
                line[0] = OWNED_BY_TOUR_TAKER;
            }
        status.push_back(line);
	}

	statusFile.close();

    printLinesToFile(statusPath, status);

    isPlayer1Tour = !isPlayer1Tour;
}

bool doesFileExist(const std::string &filePath)
{
    std::ifstream file(filePath);
    bool res = file.good();
    file.close();
    return res;
}

bool printLinesToFile(const std::string& filePath, std::vector<std::string>& toPrint)
{
    if (!doesFileExist(filePath)
    {
        return false;
    })

    std::ofstream file(filePath);

    for (const std::string& oneLine : toPrint)
	{
		mapFileWrite << oneLine << std::endl;
	}

    file.close();

    return true;
}