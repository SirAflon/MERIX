#pragma once
#include <fstream>
#include <vector>
#include <thread>
#include <optional>
#include <cstddef>
#include "../../lib/header/output.h"
#include "../../lib/header/settings.h"
#include "../../lib/header/lexerTokens.h"

namespace lexer{
    void entry(std::vector<char*>,Settings);
}