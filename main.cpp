#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "consts.h"
#include "FileValidator.h"
#include "Mediator.h"


int main(int argc, char** argv)
{
    
    std::string player1ProgramPath;
    std::string player2ProgramPath;
    std::string mapFilePath;
    std::string statusFilePath;
    std::string ordersFilePath;
    int timeLimit;

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

    std::string endOfStartPath = " " + mapFilePath + " " + statusFilePath + " " + ordersFilePath;
    std::string player1StartPath = "./" + player1ProgramPath + endOfStartPath;
    std::string player2StartPath = "./" + player2ProgramPath + endOfStartPath;

    std::cout << player1StartPath << std::endl; 

    if (!access(player1ProgramPath.c_str(), X_OK) || !access(player1ProgramPath.c_str(), X_OK))
    {
        throw std::invalid_argument(INCORRECT_PLAYER);
    }

    // even index means program 1 odd means program 2
    for(int i = 0; i < TOURS_LIMIT; ++i)
    {
        std::cout << "TOUR NUMBER: " << std::to_string(i) << std::endl;
        std::string programPath = (i % 2 == 0 ? player1StartPath : player1StartPath);
        std::chrono::seconds timeout(timeLimit);

        std::thread program([&programPath]() 
        {
            std::system(programPath.c_str());
        });

        bool programFinished = false;
        auto start = std::chrono::steady_clock::now();

        while (!programFinished && std::chrono::steady_clock::now() - start < timeout) 
        {
        // Check if the program has finished
        int exitCode = std::system(("pgrep -f " + programPath + " > /dev/null").c_str());
        programFinished = (exitCode != 0);
        }

        // If the program is still running, terminate it. This would look different of Windows
        if (!programFinished) 
        {
            std::string killCommand = "pkill -f " + programPath;
            std::system(killCommand.c_str());
        }

        // run player 1 or 2 program while checking if time isn't up
         if (!validator.checkIfFilesAreCorrect(mapFilePath, statusFilePath, ordersFilePath) || !programFinished)
         {
            if (i % 2 == 0)
            {
                std::cout << PLAYER1_DISQUALIFIED;
            }
            else
            {
                std::cout << PLAYER2_DISQUALIFIED;
            }
         }

        if (mediator.updateGameStatusWithOrdersAndValidateWinner())
        {
            if (i % 2 == 0)
            {
                std::cout << PLAYER1_WON;
            }
            else
            {
                std::cout << PLAYER2_WON;
            }
        }
        mediator.switchTourAndDataForOtherPlayer();
    }

    std::pair<int, int> aliveUnits = mediator.tallyUpUnits();

    if (aliveUnits.first > aliveUnits.second)
    {
        std::cout << PLAYER1_WON;
    } 
    else if (aliveUnits.first < aliveUnits.second)
    {
        std::cout << PLAYER2_WON;
    }
    else
    {
        std::cout << DRAW;
    }
    
    return 0;
}