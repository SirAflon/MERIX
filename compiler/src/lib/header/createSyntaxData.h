#pragma once
#include <vector>
#include <string>
#include "../../header/frontend/lexer.h"

namespace syntax{
struct metaDataType{
        std::vector<std::string> parentStructs;
        std::string intBits;
        std::string floatBits;
        std::string reg="";
        int arrayLength=0;
        bool isArray=false;
        bool isVoid = true;
        bool isRef = false;
        bool isPointer = false;
    };
    struct metaVReg{
        std::vector<std::string> para;
        std::string name;
        std::string reg="";
        bool isCall=false;
    };
    struct metaArray{
        std::vector<lexer::Token> para;
        std::string name;
        bool isCall=true;
        bool isRange=false;
    };
}