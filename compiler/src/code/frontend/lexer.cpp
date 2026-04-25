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
        if(!file){
            output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"the file '",filePath,"' could not be opened.");
            return;
        }
        std::string source((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
        size_t size = source.size();
        std::vector<Token> tokens;
        tokens.reserve(size*0.25);
        unsigned int line =1,col = 1;
        size_t pos=0;
        auto tokenMapSymbolEnd = tokenMapSymbols.end();
        auto tokenMapKeywordEnd = tokenMapKeywords.end();
        auto tokenTempSymbol = tokenMapSymbolEnd;
        auto tokenTempKeyword = tokenMapKeywordEnd;
        bool valid=false;
        bool isFloat=false;
        std::string strTmp;
        while(pos<size){
            char ch = source[pos];
            switch(ch) {
                case '\n':
                    pos++;
                    line++;
                    col = 1;
                    valid = true;
                    break;
                case ' ':
                case '\t':
                case '\r':
                    col++;
                    pos++;
                    valid = true;
                    break;
            }
            if(valid){
                valid=false;
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
            if((ch >= '0' && ch <= '9') || (ch == '.' && pos+1 < size && source[pos+1] >= '0' && source[pos+1] <= '9')){
                strTmp = ch;
                isFloat=false;
                valid=true;
                while(pos+1 < size && (
                        (source[pos+1] >= '0' && source[pos+1] <= '9') ||
                        (source[pos+1] == '.' && pos+2 < size && source[pos+2] >= '0' && source[pos+2] <= '9')
                    )){
                    pos++;
                    col++;
                    strTmp += source[pos];
                    if(source[pos]=='.'){
                        if(isFloat)
                            valid=false;
                        else
                            isFloat=true;
                    }
                }
                pos++;
                col++;
                if(valid)
                    tokens.push_back(Token{(isFloat)?TokenKind::TOKEN_FLOAT:TokenKind::TOKEN_INTEGER,strTmp,line,col});
                else
                    tokens.push_back(Token{TokenKind::TOKEN_ERROR,strTmp,line,col});
                continue;
            }
            for (int len = MAX_TOKEN_LEN; len >= 1; --len) {
                if (pos + len > size)
                    continue;
                std::string cand = source.substr(pos, len);
                tokenTempSymbol = tokenMapSymbols.find(cand);
                if (tokenTempSymbol == tokenMapSymbolEnd)
                    continue;
                if(tokenTempSymbol->second == TokenKind::TOKEN_COMMENT){
                        pos += 2;
                        col += 2;
                        while(pos<size && source[pos]!='\n'){
                            if(source[pos]=='\n')
                                break;
                            col++;
                            pos++;
                        }
                        valid=true;
                        break;
                    }else{
                        pos += len;
                        col += len;
                        tokens.push_back(Token{tokenTempSymbol->second, cand, line, col - len});
                        valid = true;
                        break;
                    }
                }
            if(valid){
                valid=false;
                continue;
            }
            if(ch == '\''){
                if(pos+2 < size&&source[pos+2]=='\''){
                    strTmp = source[pos+1];
                    tokens.push_back(Token{TokenKind::TOKEN_CHAR,strTmp,line,col});
                    pos +=3;
                    col +=3;
                    continue;
                }
                if(pos+3 < size&&source[pos+1] == '\\' && source[pos+3]=='\''){
                    strTmp = source[pos+1];
                    strTmp += source[pos+2];
                    tokens.push_back(Token{TokenKind::TOKEN_CHAR,strTmp,line,col});
                    pos +=4;
                    col +=4;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_ERROR,"could not Lex Char",line,col});
                pos++;
                col++;
                continue;
            }
            if(ch == '"'){
                strTmp = "";
                auto isEscapedQuote = [&](size_t quotePos) {
                    int backslashes = 0;
                    size_t check = quotePos;
                    while(check > 0 && source[check-1] == '\\'){
                        backslashes++;
                        check--;
                    }
                    return (backslashes % 2) != 0;
                };
                while(pos+1 < size && (source[pos+1]!='"'||isEscapedQuote(pos+1))){
                    pos++;
                    col++;
                    if(source[pos]=='\n'){
                        line++;
                        col=1;
                    }
                    strTmp += source[pos];
                }
                tokens.push_back(Token{TokenKind::TOKEN_STRING,strTmp,line,col});
                pos+=2;
                col+=2;
                continue;
            }
            strTmp = ch;
            while(pos+1<size&&!(tokenBreakChar.contains(source[pos+1]))){
                pos++;
                col++;
                strTmp += source[pos];
            }
            tokenTempKeyword = tokenMapKeywords.find(strTmp);
            if(tokenTempKeyword != tokenMapKeywordEnd){
                tokens.push_back(Token{tokenTempKeyword->second,strTmp,line,col});
            }else{
                tokens.push_back(Token{TokenKind::TOKEN_IDENTIFIER,strTmp,line,col});
            }
            pos++;
            col++;
            continue;
        }
    }
    std::atomic<unsigned int> freeID=0;
    unsigned int getID(){
        return freeID.fetch_add(1);
    }
    void entry(const std::vector<char*> inputfiles,Settings flag){
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",inputfiles.size()," Threads<LEXER>");
        unsigned int length = (flag.maxThreads == 0)?static_cast<unsigned int>(inputfiles.size()):(static_cast<unsigned int>(inputfiles.size())>flag.maxThreads)?flag.maxThreads:static_cast<unsigned int>(inputfiles.size());
        std::vector<std::thread> threads(length);
        unsigned int threadCount=0;
        bool run = true;
        while(run){
            for(unsigned int i=0;((i<length)&&threadCount<inputfiles.size());i++){
                unsigned int id = getID();
                threads[i] = std::thread(lexer::threadEntry,inputfiles[threadCount],id,flag);
                threadCount++;
                if(flag.showStatus)
                    output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"the new Thread<LEXER>[",id,"] was created.");
            }
            if(flag.showStatus){
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are created.");
                output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"Waiting for Threads<LEXER> to finish.");
            }
            for (auto& t : threads)
                if(t.joinable()){
                    t.join();
                    t = std::thread{};
                }
            if(inputfiles.size() <= threadCount)
                run = false;
        }
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are finished.");
    }
}
