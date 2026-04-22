#include "lib/header/inputParser.h"

#include "header/frontend/lexer.h"


int main(int argc, char* args[]){
    parsedCommand cmd = parseInput(argc,args);
    lexer::entry(cmd.inputFiles,cmd.flag);
}