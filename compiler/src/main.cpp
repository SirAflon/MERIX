#include "header/frontend/lexer.h"
#include "lib/header/output.h"
#include "lib/header/settings.h"
#include <vector>
#include <cstring>
#include <thread>

bool endsWith(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t str_len = std::strlen(str);
    size_t suff_len = std::strlen(suffix);
    if (suff_len > str_len) return false;
    return std::strcmp(str + str_len - suff_len, suffix) == 0;
}

int main(int argc, char* args[]){
    if(argc <= 2){
        output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"please provide a inputfile file path and the output path.");
        return 1;
    }
    char* outputfile = args[1];
    std::vector<char*> inputfiles;
    std::vector<char*> flags;
    bool projFileUsed = false;
    char* projFilePath;
    int curr = 2;
    Settings flag;
    flag.compilerDebugMode = true;
    if(endsWith(args[curr],".merp")){
        projFileUsed = true;
        projFilePath = args[curr];
        curr++;
    }
    while(curr < argc){
        if(!projFileUsed&&endsWith(args[curr],".mrx")){
            inputfiles.push_back(args[curr]);
            curr++;
            continue;
        }
        flags.push_back(args[curr]);
        curr++;
    }
    if(projFileUsed){

    }else{
        if(flag.compilerDebugMode)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",inputfiles.size()," Threads<LEXER>");
        std::thread threads[inputfiles.size()];
        for(int i=0;i<inputfiles.size();i++){
            threads[i] = std::thread(lexer::entry,inputfiles[i],i,flag);
            if(flag.compilerDebugMode)
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"a new Thread<LEXER>[",i,"] was created.");
        }
        if(flag.compilerDebugMode){
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are created.");
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"Waiting for Threads<LEXER> to finish.");
        }
        for (auto& t : threads)
            if(t.joinable())
                t.join();
        if(flag.compilerDebugMode)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are finished.");
    }
}