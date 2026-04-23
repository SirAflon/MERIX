#pragma once
#include "settings.h"
#include "output.h"
#include <vector>
#include <cstring>
#include <charconv>
#include <string_view>

struct parsedCommand{
    char* outputFile = nullptr;
    std::vector<char*> inputFiles;
    Settings flag;
};
parsedCommand parseInput(int argc,char* args[]);