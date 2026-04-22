#include "../../header/frontend/lexer.h"

namespace lexer{
    void threadEntry(const char* filePath,int threadID,Settings flag){
            if(flag.compilerDebugMode)
                output::println(output::UseColor(output::FG_GREEN),"REPORT<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"executing Thread.");
            std::ifstream file(filePath);
            if(!file.is_open()){
                output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"the file '",filePath,"' could not be opened.");
                return;
            }
    }
    void entry(const std::vector<char*> inputfiles,Settings flag){
        if(flag.compilerDebugMode)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",inputfiles.size()," Threads<LEXER>");
        std::thread threads[inputfiles.size()];
        for(long unsigned int i=0;i<inputfiles.size();i++){
            threads[i] = std::thread(lexer::threadEntry,inputfiles[i],i,flag);
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