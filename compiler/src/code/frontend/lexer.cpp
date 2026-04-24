#include "../../header/frontend/lexer.h"

namespace lexer{
    struct Token{
        TokenKind kind;
        std::string lexeme;
        unsigned int line;
        unsigned int col;
    };
    void threadEntry(const char* filePath,int threadID,Settings flag){
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"executing Thread.");
        std::ifstream file(filePath,std::ios::binary);
        if(file){
            output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"the file '",filePath,"' could not be opened.");
            return;
        }
        std::string source((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
        size_t size = source.size();
        std::vector<Token> tokens((size*3));
        unsigned int line =1,col = 1;
        size_t pos=0;
        auto tokenMapEnd = tokenMapSymbols.end();
        auto tokenTemp = tokenMapEnd;
        bool valid=false;
        std::string strTmp;
        while(pos<size){
            char ch = source[pos];
            if (ch == ' ' || ch == '\t' || ch == '\r') {
                col++;
                pos++;
                continue;
            }
            if (ch == '\n') {
                line++;
                col = 1;
                pos++;
                continue;
            }
            if(ch=='/'&& pos+1 < size &&source[pos+1]=='/'){
                pos += 2;
                col += 2;
                while(pos<size && source[pos]!='\n'){
                    if(source[pos]=='\n')
                        break;
                    col++;                        
                    pos++;
                }
                continue;
            }
            if(ch=='/'&& pos+1 < size &&source[pos+1]=='*'){
                pos += 2;
                col += 2;
                while(pos +1 <size && !(source[pos]=='*'&&source[pos+1]=='/')){
                    if(source[pos]=='\n')
                        line++;
                    col++;
                    pos++;
                }
                pos += 2;
                col += 2;
                continue;
            }
            for (int len = MAX_TOKEN_LEN; len >= 1; --len) {
                if (pos + len > size) 
                    continue;
                std::string cand = source.substr(pos, len);
                tokenTemp = tokenMapSymbols.find(cand);
                if (tokenTemp != tokenMapEnd) {
                    pos += len;
                    col += len;
                    tokens.push_back(Token{tokenTemp->second, cand, line, col - len});
                    valid = true;
                    break;
                }
            }
            if(valid){
                valid=false;
                continue;
            }
            strTmp = ch;
            while(!(tokenBreakChar.contains(strTmp.back()))&&pos<size){
                pos++;
                col++;
                strTmp += source[pos];
                valid=true;
            }
            if(valid){
                pos--;
                col--;
                valid=false;
                tokenTemp = tokenMapKeywords.find(strTmp);
                if(tokenTemp != tokenMapEnd){
                    tokens.push_back(Token{tokenTemp->second,strTmp,line,col});
                }else{
                    tokens.push_back(Token{TokenKind::TOKEN_IDENTIFIER,strTmp,line,col});
                }
                continue;
            }
        }
    }
    unsigned int freeID=0;
    unsigned int getID(){
        freeID++;
        return freeID-1;
    }
    void entry(const std::vector<char*> inputfiles,Settings flag){
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",inputfiles.size()," Threads<LEXER>");
        std::vector<std::thread> threads(inputfiles.size());
        unsigned int length = (flag.maxThreads == 0)?static_cast<unsigned int>(inputfiles.size()):(static_cast<unsigned int>(inputfiles.size())>flag.maxThreads)?flag.maxThreads:static_cast<unsigned int>(inputfiles.size());
        unsigned int threadCount=0;
        bool run = true;
        while(run){
            for(unsigned int i=0;((i<length)&&run);i++){
                unsigned int id = getID();
                threads[i] = std::thread(lexer::threadEntry,inputfiles[i],id,flag);
                threadCount++;
                if(flag.showStatus)
                    output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"the new Thread<LEXER>[",id,"] was created.");
            }
            if(flag.showStatus){
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are created.");
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"Waiting for Threads<LEXER> to finish.");
            }
            for (auto& t : threads)
                if(t.joinable())
                    t.join();
            if(inputfiles.size() <= threadCount)
                run = false;
        }
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are finished.");
    }
}