#include "../../header/frontend/lexer.h"

namespace lexer{
    void entry(char* filePath,int threadID,Settings flag){
        if(flag.compilerDebugMode)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"executing Thread.");
        std::ifstream file(filePath);
        if(!file.is_open()){
            output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"the file '",filePath,"' could not be opened.");
            return;
        }
    }
}