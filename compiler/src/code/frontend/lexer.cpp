#include "../../header/frontend/lexer.h"

namespace lexer{
    void threadEntry(const char* filePath,int threadID,Settings flag){
            if(flag.showStatus)
                output::println(output::UseColor(output::FG_GREEN),"REPORT<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"executing Thread.");
            std::ifstream file(filePath);
            if(file.fail()){
                output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"the file '",filePath,"' could not be opened.");
                return;
            }
            bool running=true;
            unsigned long lineCount=0;
            while(running){
                lineCount++;
                if(file.peek() == EOF)
                    running=false;
            }
    }
    void entry(const std::vector<char*> inputfiles,Settings flag){
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",inputfiles.size()," Threads<LEXER>");
        std::vector<std::thread> threads(inputfiles.size());
        unsigned int length = (flag.maxThreads <= 0)?static_cast<unsigned int>(inputfiles.size()):(static_cast<unsigned int>(inputfiles.size())>flag.maxThreads)?flag.maxThreads:static_cast<unsigned int>(inputfiles.size());
        unsigned int threadCount=0;
        bool run = true;
        while(run){
            for(unsigned int i=0;((i<length)&&run);i++){
                threads[i] = std::thread(lexer::threadEntry,inputfiles[i],i,flag);
                threadCount++;
                if(flag.showStatus)
                    output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"the new Thread<LEXER>[",i,"] was created.");
            }
            if(flag.showStatus){
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are created.");
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"Waiting for Threads<LEXER> to finish.");
            }
            for (auto& t : threads)
                if(t.joinable())
                    t.join();
            if(inputfiles.size() == threadCount)
                run = false;
        }
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are finished.");
    }
}