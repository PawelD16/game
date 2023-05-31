#include "utils.h"

bool doesFileExist(const std::string &filePath)
{
    std::ifstream file(filePath);
    bool res = file.good();
    file.close();
    return res;
}

bool printLinesToFile(const std::string &filePath, std::vector<std::string> &toPrint)
{
    if (!doesFileExist(filePath))
    {
        return false;
    }

    std::ofstream file(filePath);

    for (const std::string &oneLine : toPrint)
    {
        file << oneLine << std::endl;
    }

    file.close();

    return true;
}

std::vector<std::string> readLinesFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);

    if (!file)
    {
        file.close();
        throw std::runtime_error(NO_FILE + filePath);
    }

    std::vector<std::string> lines;
    std::string line;

    // switching base numbers
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> tokenizeOnSpaces(std::string toTokenize)
{
    std::istringstream iss(toTokenize);
	std::vector<std::string> tokens;
	std::string token;

	while (iss >> token)
	{
		tokens.push_back(token);
    }

    return tokens;
}

int calculateDistance(int posx1, int posy1, int posx2, int posy2)
{
    return std::abs(posx1 - posx2) + std::abs(posy1 - posy2);
}

bool isNaturalNumber(const std::string& toValidation)
{
    return toValidation.find_first_not_of("0123456789") != std::string::npos;
}