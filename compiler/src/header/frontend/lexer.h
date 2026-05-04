#pragma once
#include <fstream>
#include <vector>
#include <thread>
#include <optional>
#include <cstddef>
#include <atomic>
#include "../../lib/header/output.h"
#include "../../lib/header/settings.h"
#include "../../lib/header/lexerTokens.h"

namespace lexer{
    struct Token{
        TokenKind kind;
        std::string lexeme;
        unsigned int line;
        unsigned int col;
    };
    struct TokenFile{
        std::vector<Token> tokens;
        char* filePath;
    };
    void entry(std::vector<char*>,Settings,std::vector<TokenFile>&);
}