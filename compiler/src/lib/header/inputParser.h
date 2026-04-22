#pragma once
#include "settings.h"
#include "output.h"
#include <vector>
#include <cstring>

struct parsedCommand{
    char* outputFile;
    std::vector<char*> inputFiles;
    Settings flag;
};
parsedCommand parseInput(int argc,char* args[]);