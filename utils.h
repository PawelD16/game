#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
//#include <cstdlib> 

#include "consts.h"

bool doesFileExist(const std::string &filePath);
bool printLinesToFile(const std::string &filePath, std::vector<std::string> &toPrint);
std::vector<std::string> readLinesFromFile(const std::string &filePath);
std::vector<std::string> tokenizeOnSpaces(std::string toTokenize);
int calculateDistance(int posX1, int posY1, int posX2, int posY2);
bool isNaturalNumber(const std::string& toValidation);