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
        std::vector<Token> tokens((source.size()*3));
        unsigned int line =1,col = 1;
        size_t pos=0;
        while(pos<source.size()){
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
            if(ch=='/'&& pos+1 < source.size() &&source[pos+1]=='/'){
                pos += 2;
                col += 2;
                while(pos<source.size() && source[pos]!='\n'){
                    if(source[pos]=='\n')
                        break;
                    col++;                        
                    pos++;
                }
                continue;
            }
            if(ch=='/'&& pos+1 < source.size() &&source[pos+1]=='*'){
                pos += 2;
                col += 2;
                while(pos +1 <source.size() && !(source[pos]=='*'&&source[pos+1]=='/')){
                    if(source[pos]=='\n')
                        line++;
                    col++;
                    pos++;
                }
                pos += 2;
                col += 2;
                continue;
            }
            if(ch=='<'){
                if(source[pos+1]=='<'){
                    if(source[pos+2]=='='){
                        tokens.push_back(Token{TokenKind::TOKEN_SHIFT_LEFT_EQ,std::string("<<="),line,col});
                        pos +=3;
                        col +=3;
                        continue;
                    }
                    tokens.push_back(Token{TokenKind::TOKEN_SHIFT_LEFT,std::string("<<"),line,col});
                    pos++;
                    col++;
                    continue;
                }
                if(source[pos+1]=='-'){
                    tokens.push_back(Token{TokenKind::TOKEN_ARROW,std::string("<-"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_LT_EQ,std::string("<="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_LT,std::string("<"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='>'){
                if(source[pos+1]=='>'){
                    if(source[pos+2]=='='){
                        tokens.push_back(Token{TokenKind::TOKEN_SHIFT_RIGHT_EQ,std::string(">>="),line,col});
                        pos +=3;
                        col +=3;
                        continue;
                    }
                    tokens.push_back(Token{TokenKind::TOKEN_SHIFT_RIGHT,std::string(">>"),line,col});
                    pos++;
                    col++;
                    continue;
                }
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_GT_EQ,std::string(">="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_GT,std::string(">"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='-'){
                if(source[pos+1]=='>'){
                    tokens.push_back(Token{TokenKind::TOKEN_ARROW,std::string("->"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_MINUS_EQ,std::string("-="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                if(source[pos+1]=='-'){
                    tokens.push_back(Token{TokenKind::TOKEN_DEC,std::string("--"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_MINUS,std::string("-"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='+'){
                if(source[pos+1]=='+'){
                    tokens.push_back(Token{TokenKind::TOKEN_INC,std::string("++"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_PLUS_EQ,std::string("+="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_PLUS,std::string("+"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='|'){
                if(source[pos+1]=='|'){
                    tokens.push_back(Token{TokenKind::TOKEN_PIPE_PIPE,std::string("||"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_PIPE_EQ,std::string("|="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_PIPE,std::string("|"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='&'){
                if(source[pos+1]=='&'){
                    tokens.push_back(Token{TokenKind::TOKEN_AND_AND,std::string("&&"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_AMP_EQ,std::string("&="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_AMP,std::string("&"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='!'){
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_NOT_EQ,std::string("!="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_EXCLAM,std::string("!"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='='){
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_EQ_EQ,std::string("=="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_ASSIGN,std::string("="),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='^'){
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_CARET_EQ,std::string("^="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_CARET,std::string("^"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='%'){
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_PERCENT_EQ,std::string("%="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_PERCENT,std::string("%"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='/'){
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_SLASH_EQ,std::string("/="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_SLASH,std::string("/"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='*'){
                if(source[pos+1]=='='){
                    tokens.push_back(Token{TokenKind::TOKEN_STAR_EQ,std::string("*="),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_STAR,std::string("*"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch==':'){
                if(source[pos+1]==':'){
                    tokens.push_back(Token{TokenKind::TOKEN_SCOPE,std::string("::"),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_COLON,std::string(":"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='.'){
                if(source[pos+1]=='.'){
                    tokens.push_back(Token{TokenKind::TOKEN_DOTDOT,std::string(".."),line,col});
                    pos +=2;
                    col +=2;
                    continue;
                }
                tokens.push_back(Token{TokenKind::TOKEN_DOT,std::string("."),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='~'){
                tokens.push_back(Token{TokenKind::TOKEN_TILDE,std::string("~"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch==';'){
                tokens.push_back(Token{TokenKind::TOKEN_SEMICOLON,std::string(";"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch==','){
                tokens.push_back(Token{TokenKind::TOKEN_COMMA,std::string(","),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='['){
                tokens.push_back(Token{TokenKind::TOKEN_LBRACKET,std::string("["),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch==']'){
                tokens.push_back(Token{TokenKind::TOKEN_RBRACKET,std::string("]"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='['){
                tokens.push_back(Token{TokenKind::TOKEN_LBRACKET,std::string("["),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='{'){
                tokens.push_back(Token{TokenKind::TOKEN_LBRACE,std::string("{"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='}'){
                tokens.push_back(Token{TokenKind::TOKEN_RBRACE,std::string("}"),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch=='('){
                tokens.push_back(Token{TokenKind::TOKEN_LPAREN,std::string("("),line,col});
                pos ++;
                col ++;
                continue;
            }
            if(ch==')'){
                tokens.push_back(Token{TokenKind::TOKEN_RPAREN,std::string(")"),line,col});
                pos ++;
                col ++;
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