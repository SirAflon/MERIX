#include "lib/header/inputParser.h"
#include "header/frontend/lexer.h"
#include "header/frontend/syntax.h"

int main(int argc, char* args[]){

    parsedCommand cmd = parseInput(argc,args);
    std::vector<lexer::TokenFile> lexedFiles;
    lexer::entry(cmd.inputFiles,cmd.flag,lexedFiles);
    
    syntax::entry(lexedFiles,cmd.flag);
}