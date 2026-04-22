#pragma once
#include <fstream>
#include <vector>
#include <thread>
#include "../../lib/header/output.h"
#include "../../lib/header/settings.h"

namespace lexer{
    void entry(std::vector<char*>,Settings);
}