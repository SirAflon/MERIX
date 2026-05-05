#include "../../header/frontend/lexer.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace lexer{
    static ThreadPool pool(std::max(1u, std::thread::hardware_concurrency()));
    std::atomic<unsigned int> freeID = 0;
    unsigned int getID(){
        return freeID.fetch_add(1);
    }
    void threadEntry(const char* filePath, unsigned int threadID, Settings flag, std::vector<Token>& tokens){
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"executing Thread.");
        int fd = open(filePath, O_RDONLY);
        if(fd == -1){
            output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"the file '",filePath,"' could not be opened.");
            return;
        }
        struct stat st;
        fstat(fd, &st);
        size_t size = static_cast<size_t>(st.st_size);
        if(size == 0){
            close(fd);
            return;
        }
        const char* source = static_cast<const char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));
        close(fd);
        if(source == MAP_FAILED){
            output::println(output::UseColor(output::FG_RED),"ERROR<LEXER>[",threadID,"]: ",output::UseColor(output::FG_DEFAULT),"mmap failed for '",filePath,"'.");
            return;
        }
        madvise((void*)source, size, MADV_SEQUENTIAL);
        tokens.reserve(size >> 2);
        unsigned int line = 1, col = 1;
        size_t pos = 0;
        auto tokenMapKeywordEnd = tokenMapKeywords.end();
        bool isFloat = false;
        std::string strTmp;
        strTmp.reserve(64);
        while(pos < size){
            char ch = source[pos];
            if(ch == '\n'){
                pos++; line++; col = 1;
                continue;
            }
            if(ch == ' ' || ch == '\t' || ch == '\r'){
                pos++; col++;
                continue;
            }
            if(ch == '/' && pos+1 < size && source[pos+1] == '*'){
                pos += 2; col += 2;
                while(pos+1 < size && !(source[pos] == '*' && source[pos+1] == '/')){
                    if(source[pos] == '\n') line++;
                    col++; pos++;
                }
                pos += 2; col += 2;
                continue;
            }
            if((ch >= '0' && ch <= '9') || (ch == '.' && pos+1 < size && source[pos+1] >= '0' && source[pos+1] <= '9')){
                strTmp = ch;
                isFloat = false;
                bool valid = true;
                while(pos+1 < size && (
                        (source[pos+1] >= '0' && source[pos+1] <= '9') ||
                        (source[pos+1] == '.' && pos+2 < size && source[pos+2] >= '0' && source[pos+2] <= '9')
                    )){
                    pos++; col++;
                    strTmp += source[pos];
                    if(source[pos] == '.'){
                        if(isFloat) valid = false;
                        else isFloat = true;
                    }
                }
                pos++; col++;
                if(valid)
                    tokens.push_back(Token{(isFloat) ? TokenKind::TOKEN_FLOAT : TokenKind::TOKEN_INTEGER, strTmp, line, col});
                else
                    tokens.push_back(Token{TokenKind::TOKEN_ERROR, strTmp, line, col});
                continue;
            }
            {
                bool symbolFound = false;
                switch(ch){
                    case '+':
                        if(pos+1 < size){
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_PLUS_EQ,"+=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                            if(source[pos+1] == '+'){ tokens.push_back(Token{TOKEN_INC,"++",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_PLUS,"+",line,col}); pos++; col++; symbolFound=true; break;
                    case '-':
                        if(pos+1 < size){
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_MINUS_EQ,"-=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                            if(source[pos+1] == '-'){ tokens.push_back(Token{TOKEN_DEC,"--",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                            if(source[pos+1] == '>'){ tokens.push_back(Token{TOKEN_ARROW,"->",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_MINUS,"-",line,col}); pos++; col++; symbolFound=true; break;
                    case '*':
                        if(pos+1 < size && source[pos+1] == '='){ tokens.push_back(Token{TOKEN_STAR_EQ,"*=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_STAR,"*",line,col}); pos++; col++; symbolFound=true; break;
                    case '/':
                        if(pos+1 < size){
                            if(source[pos+1] == '/'){
                                pos += 2; col += 2;
                                while(pos < size && source[pos] != '\n'){ col++; pos++; }
                                symbolFound=true; break;
                            }
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_SLASH_EQ,"/=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_SLASH,"/",line,col}); pos++; col++; symbolFound=true; break;
                    case '%':
                        if(pos+1 < size && source[pos+1] == '='){ tokens.push_back(Token{TOKEN_PERCENT_EQ,"%=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_PERCENT,"%",line,col}); pos++; col++; symbolFound=true; break;
                    case '&':
                        if(pos+1 < size){
                            if(source[pos+1] == '&'){ tokens.push_back(Token{TOKEN_AND_AND,"&&",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_AMP_EQ,"&=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_AMP,"&",line,col}); pos++; col++; symbolFound=true; break;
                    case '|':
                        if(pos+1 < size){
                            if(source[pos+1] == '|'){ tokens.push_back(Token{TOKEN_PIPE_PIPE,"||",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_PIPE_EQ,"|=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_PIPE,"|",line,col}); pos++; col++; symbolFound=true; break;
                    case '^':
                        if(pos+1 < size && source[pos+1] == '='){ tokens.push_back(Token{TOKEN_CARET_EQ,"^=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_CARET,"^",line,col}); pos++; col++; symbolFound=true; break;
                    case '<':
                        if(pos+1 < size){
                            if(source[pos+1] == '<'){
                                if(pos+2 < size && source[pos+2] == '='){ tokens.push_back(Token{TOKEN_SHIFT_LEFT_EQ,"<<=",line,col}); pos+=3; col+=3; symbolFound=true; break; }
                                tokens.push_back(Token{TOKEN_SHIFT_LEFT,"<<",line,col}); pos+=2; col+=2; symbolFound=true; break;
                            }
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_LT_EQ,"<=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_LT,"<",line,col}); pos++; col++; symbolFound=true; break;
                    case '>':
                        if(pos+1 < size){
                            if(source[pos+1] == '>'){
                                if(pos+2 < size && source[pos+2] == '='){ tokens.push_back(Token{TOKEN_SHIFT_RIGHT_EQ,">>=",line,col}); pos+=3; col+=3; symbolFound=true; break; }
                                tokens.push_back(Token{TOKEN_SHIFT_RIGHT,">>",line,col}); pos+=2; col+=2; symbolFound=true; break;
                            }
                            if(source[pos+1] == '='){ tokens.push_back(Token{TOKEN_GT_EQ,">=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        }
                        tokens.push_back(Token{TOKEN_GT,">",line,col}); pos++; col++; symbolFound=true; break;
                    case '=':
                        if(pos+1 < size && source[pos+1] == '='){ tokens.push_back(Token{TOKEN_EQ_EQ,"==",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_ASSIGN,"=",line,col}); pos++; col++; symbolFound=true; break;
                    case '!':
                        if(pos+1 < size && source[pos+1] == '='){ tokens.push_back(Token{TOKEN_NOT_EQ,"!=",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_EXCLAM,"!",line,col}); pos++; col++; symbolFound=true; break;
                    case ':':
                        if(pos+1 < size && source[pos+1] == ':'){ tokens.push_back(Token{TOKEN_SCOPE,"::",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_COLON,":",line,col}); pos++; col++; symbolFound=true; break;
                    case '.':
                        if(pos+1 < size && source[pos+1] == '.'){ tokens.push_back(Token{TOKEN_DOTDOT,"..",line,col}); pos+=2; col+=2; symbolFound=true; break; }
                        tokens.push_back(Token{TOKEN_DOT,".",line,col}); pos++; col++; symbolFound=true; break;
                    case '(': tokens.push_back(Token{TOKEN_LPAREN,"(",line,col}); pos++; col++; symbolFound=true; break;
                    case ')': tokens.push_back(Token{TOKEN_RPAREN,")",line,col}); pos++; col++; symbolFound=true; break;
                    case '{': tokens.push_back(Token{TOKEN_LBRACE,"{",line,col}); pos++; col++; symbolFound=true; break;
                    case '}': tokens.push_back(Token{TOKEN_RBRACE,"}",line,col}); pos++; col++; symbolFound=true; break;
                    case '[': tokens.push_back(Token{TOKEN_LBRACKET,"[",line,col}); pos++; col++; symbolFound=true; break;
                    case ']': tokens.push_back(Token{TOKEN_RBRACKET,"]",line,col}); pos++; col++; symbolFound=true; break;
                    case ',': tokens.push_back(Token{TOKEN_COMMA,",",line,col}); pos++; col++; symbolFound=true; break;
                    case ';': tokens.push_back(Token{TOKEN_SEMICOLON,";",line,col}); pos++; col++; symbolFound=true; break;
                    case '~': tokens.push_back(Token{TOKEN_TILDE,"~",line,col}); pos++; col++; symbolFound=true; break;
                    case '?': tokens.push_back(Token{TOKEN_QUESTIONMARK,"?",line,col}); pos++; col++; symbolFound=true; break;
                    default: break;
                }
                if(symbolFound) continue;
            }
            if(ch == '\''){
                if(pos+2 < size && source[pos+2] == '\''){
                    strTmp.assign(&source[pos+1], 1);
                    tokens.push_back(Token{TokenKind::TOKEN_CHAR, strTmp, line, col});
                    pos += 3; col += 3;
                    continue;
                }
                if(pos+3 < size && source[pos+1] == '\\' && source[pos+3] == '\''){
                    strTmp.assign(&source[pos+1], 2);
                    tokens.push_back(Token{TokenKind::TOKEN_CHAR, strTmp, line, col});
                    pos += 4; col += 4;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_ERROR, "could not Lex Char", line, col});
                pos++; col++;
                continue;
            }
            if(ch == '"'){
                strTmp.clear();
                pos++; col++;
                while(pos < size && source[pos] != '"'){
                    if(source[pos] == '\\' && pos+1 < size){
                        strTmp += source[pos];
                        strTmp += source[pos+1];
                        pos += 2; col += 2;
                    } else {
                        if(source[pos] == '\n'){ line++; col = 1; }
                        strTmp += source[pos];
                        pos++; col++;
                    }
                }
                tokens.push_back(Token{TokenKind::TOKEN_STRING, strTmp, line, col});
                pos++; col++;
                continue;
            }
            strTmp.clear();
            const char* start = &source[pos];
            size_t startPos = pos;
            while(pos < size && !tokenBreakChar.contains(source[pos])){
                pos++; col++;
            }
            strTmp.assign(start, pos - startPos);
            auto tokenTempKeyword = tokenMapKeywords.find(std::string_view(strTmp));
            if(tokenTempKeyword != tokenMapKeywordEnd){
                tokens.push_back(Token{tokenTempKeyword->second, strTmp, line, col});
            } else {
                tokens.push_back(Token{TokenKind::TOKEN_IDENTIFIER, strTmp, line, col});
            }
            continue;
        }
        munmap((void*)source, size);
    }
    void entry(const std::vector<char*> inputfiles, Settings flag, std::vector<TokenFile>& tokens){
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",inputfiles.size()," Tasks<LEXER>");
        tokens.resize(inputfiles.size());
        for(unsigned int i = 0; i < static_cast<unsigned int>(inputfiles.size()); i++){
            tokens[i].filePath = inputfiles[i];
            pool.enqueue([i, &inputfiles, &tokens, flag]{
                unsigned int id = getID();
                if(flag.showStatus)
                    output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"the new Thread<LEXER>[",id,"] was created.");
                threadEntry(inputfiles[i], id, flag, tokens[i].tokens);
            });
        }
        pool.waitAll();
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<LEXER> are finished.");
    }
}