#include "header/frontend/lexer.h"
#include "lib/header/output.h"
#include "lib/header/settings.h"
#include "lib/header/inputParser.h"



int main(int argc, char* args[]){
    parsedCommand cmd = parseInput(argc,args);
    lexer::entry(cmd.inputFiles,cmd.flag);
}