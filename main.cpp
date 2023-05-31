#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "boost_process/boost/process.hpp"
#include "consts.h"
#include "FileValidator.h"
#include "Mediator.h"

namespace bp = boost::process;

int main(int argc, char** argv)
{
    
    std::string player1ProgramPath;
    std::string player2ProgramPath;
    std::string mapFilePath;
    std::string statusFilePath;
    std::string ordersFilePath;
    int timeLimit = 5;
    
    std::cout << "Input player 1 program path: ";
    std::cin >> player1ProgramPath;

    std::cout << "Input player 2 program path: ";
    std::cin >> player2ProgramPath;

    std::cout << "Input map file path: ";
    std::cin >> mapFilePath;

    std::cout << "Input status file path: ";
    std::cin >> statusFilePath;

    std::cout << "Orders file path: ";
    std::cin >> ordersFilePath;

    std::cout << "Time limit: ";
    std::cin >> timeLimit;
    

    FileValidator validator;
    Mediator mediator(mapFilePath, statusFilePath, ordersFilePath, timeLimit);
    mediator.generateStartingStatus();

    std::vector<std::string> args = {mapFilePath, statusFilePath, ordersFilePath, std::to_string(timeLimit)};
    std::string player1StartPath = player1ProgramPath;
    std::string player2StartPath = player2ProgramPath;

    if (access(player1ProgramPath.c_str(), X_OK) != 0 || access(player1ProgramPath.c_str(), X_OK) != 0)
    {
        throw std::invalid_argument(INCORRECT_PLAYER);
    }

    // even index means program 1 odd means program 2
    for(int i = 0; i < TOURS_LIMIT; ++i)
    {
        std::cout << "TOUR NUMBER: " << std::to_string(i) << " ";
        std::string programPath = (i % 2 == 0 ? player1StartPath : player1StartPath);

        bp::child playerProcess(programPath, args);

        bool playerFinished = true;

        std::chrono::seconds timeout(timeLimit);

        auto start = std::chrono::steady_clock::now();

        while (playerProcess.running() && std::chrono::steady_clock::now() - start < timeout) {}

        // If the program is still running, terminate it.
        if (playerProcess.running())
        {
            playerProcess.terminate();
            playerFinished = false;
        }

        // run player 1 or 2 program while checking if time isn't up
        if (!validator.checkIfFilesAreCorrect(mapFilePath, statusFilePath, ordersFilePath) || !playerFinished)
        {
            // last player would have couse the havoc, so it is changed
            if (i % 2 == 0)
            {
                std::cout << PLAYER2_DISQUALIFIED << std::endl;
            }
            else
            {
                std::cout << PLAYER1_DISQUALIFIED << std::endl;
            }
            return 0;
        }

        if (mediator.updateGameStatusWithOrdersAndValidateWinner())
        {
            if (i % 2 == 0)
            {
                std::cout << PLAYER1_WON << std::endl;
            }
            else
            {
                std::cout << PLAYER2_WON << std::endl;
            }
            return 0;
        }
        
        mediator.switchTourAndDataForOtherPlayer();
    }

    std::pair<int, int> aliveUnits = mediator.tallyUpUnits();

    if (aliveUnits.first > aliveUnits.second)
    {
        std::cout << PLAYER1_WON << std::endl;;
    } 
    else if (aliveUnits.first < aliveUnits.second)
    {
        std::cout << PLAYER2_WON << std::endl;;
    }
    else
    {
        std::cout << DRAW << std::endl;
    }
    
    return 0;
}