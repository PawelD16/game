#include <iostream>
#include <string>
#include <ctime>
#include <chrono>


int main(int argc, char** argv)
{
    /*
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


    std::time_t start = std::time(nullptr);
    std::string programPath = "path_to_program.exe";
    std::string command = programPath + " &";

    std::system(command.c_str());

    // Set a timeout duration (e.g., 5 seconds)
    std::chrono::seconds timeout(5);

    // Check if the timeout has exceeded
    while (std::difftime(std::time(nullptr), start) < timeout.count()) {
        // Optionally perform other logic while waiting
    }

    // If the timeout has exceeded, terminate the program
    std::string killCommand;

    #ifdef _WIN32
        killCommand = "taskkill /F /IM " + programPath;
    #else
        killCommand = "pkill -f " + programPath;
    #endif

    std::system(killCommand.c_str());
    */
    return 0;
}