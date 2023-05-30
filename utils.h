#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "consts.h"

bool doesFileExist(const std::string &filePath);
bool printLinesToFile(const std::string &filePath, std::vector<std::string> &toPrint);
std::vector<std::string> readLinesFromFile(const std::string &filePath);
std::vector<std::string> tokenizeOnSpaces(std::string toTokenize);