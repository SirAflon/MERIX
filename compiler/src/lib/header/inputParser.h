#pragma once
#include "settings.h"
#include "output.h"
#include <vector>
#include <cstring>
#include <unordered_map>

struct parsedCommand{
    char* outputFile = nullptr;
    std::vector<char*> inputFiles;
    Settings flag;
};
parsedCommand parseInput(int argc,char* args[]);