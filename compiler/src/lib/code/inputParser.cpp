#include "../header/inputParser.h"

bool endsWith(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t str_len = std::strlen(str);
    size_t suff_len = std::strlen(suffix);
    if (suff_len > str_len) return false;
    return std::strcmp(str + str_len - suff_len, suffix) == 0;
}
parsedCommand parseInput(int argc,char* args[]){
    if(argc <= 2){
        output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"please provide a inputfile file path and the output path.");
        exit(1);
    }
    parsedCommand cmd;
    std::vector<char*> flags;
    cmd.outputFile = args[1];
    bool projFileUsed = false;
    char* projFilePath;
    int curr = 2;
    cmd.flag.compilerDebugMode = true;
    if(endsWith(args[curr],".merp")){
        projFileUsed = true;
        projFilePath = args[curr];
        curr++;
    }
    while(curr < argc){
        if(!projFileUsed&&endsWith(args[curr],".mrx")){
            cmd.inputFiles.push_back(args[curr]);
            curr++;
            continue;
        }
        flags.push_back(args[curr]);
        curr++;
    }
    if(cmd.inputFiles.size()==0){
        output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"please provide a valid inputfile file path and the output path. The ending must be a '.mrx' or '.merp'.");
        exit(1);
    }
    return cmd;
}