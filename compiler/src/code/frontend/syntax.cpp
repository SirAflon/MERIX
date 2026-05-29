#include "../../header/frontend/syntax.h"
#include "../../lib/header/contextTokens.h"
#include "../../lib/header/output.h"
#include <thread>
#include "../../lib/header/createSyntax.h"

namespace syntax {
    static ThreadPool pool(std::max(1u, std::thread::hardware_concurrency()));
    std::atomic<unsigned int> freeID = 0;
    unsigned int getID(){
        return freeID.fetch_add(1);
    }
    void ParseREG(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseMEM(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseVREG(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseScope(const std::vector<lexer::Token>& tokens,size_t& pos);
    
    void ParseREG(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        bool regValid=false;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_GT:
                    con=false;
                    break;
                case TK::TOKEN_LT:
                    break;
                case TK::TOKEN_IDENTIFIER:
                    if(regs.find(tokens[pos].lexeme)!=regs.end())
                        regValid=true;
                    pos++;
                    break;
                default:
                    pos++;
            }
        }
    }
    void ParseMEM(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        std::vector<Node*> nodes;
        std::vector<TokenLists> list;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                default:
                    pos++;
            }
        }
    }
    void ParseVREG(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        std::string structName;
        std::vector<std::string> num;
        bool isRef=false;
        bool isPointer=false;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_GT:
                    con=false;
                    break;
                case TK::TOKEN_LT:
                    break;
                case TK::TOKEN_IDENTIFIER:
                    if(structName.empty())
                        structName = tokens[pos].lexeme;
                    pos++;
                    break;
                case TK::TOKEN_INTEGER:
                    num.push_back(tokens[pos].lexeme);
                    pos++;
                    break;
                case TK::TOKEN_KEYWORD_MEM:
                    pos++;
                    ParseMEM(tokens,pos);
                    break;
                case TK::TOKEN_KEYWORD_REG:
                    pos++;
                    ParseREG(tokens,pos);
                case TK::TOKEN_STAR:
                    pos++;
                    isPointer=true;
                    break;
                case TK::TOKEN_AMP:
                    pos++;
                    isRef=true;
                    break;
                case TK::TOKEN_COMMA:
                    pos++;
                    break;
                default:
                    pos++;
            }
        }
    }
    void ParseRuntime(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con = true;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_IDENTIFIER:
                    break;
                case TK::TOKEN_CHAR:
                    break;
                case TK::TOKEN_STRING:
                    break;
                case TK::TOKEN_EQ_EQ:
                    break;
                case TK::TOKEN_NOT_EQ:
                    break;
                case TK::TOKEN_LT_EQ:
                    break;
                case TK::TOKEN_GT_EQ:
                    break;
                case TK::TOKEN_AND_AND:
                    break;
                case TK::TOKEN_PIPE_PIPE:
                    break;
                case TK::TOKEN_PLUS:
                    break;
                case TK::TOKEN_MINUS:
                    break;
                case TK::TOKEN_STAR:
                    break;
                case TK::TOKEN_SLASH:
                    break;
                case TK::TOKEN_PERCENT:
                    break;
                case TK::TOKEN_EXCLAM:
                    break;
                case TK::TOKEN_COMMA:
                    pos++;
                    break;
                case TK::TOKEN_RPAREN:
                    con=false;
                    pos++;
                    break;
                default:
                    pos++;
            }
        }
    }
    void ParseScope(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_KEYWORD_MEM:
                    pos++;
                    ParseMEM(tokens,pos);
                    break;
                case TK::TOKEN_KEYWORD_REG:
                    pos++;
                    ParseREG(tokens,pos);
                    break;
                case TK::TOKEN_KEYWORD_VREG:
                    pos++;
                    ParseVREG(tokens,pos);
                    break;
                case TK::TOKEN_KEYWORD_STRUCT:
                    break;
                case TK::TOKEN_KEYWORD_OVERRIDE:
                    break;
                case TK::TOKEN_KEYWORD_IF:
                    break;
                case TK::TOKEN_KEYWORD_WHILE:
                    break;
                case TK::TOKEN_KEYWORD_DO:
                    break;
                case TK::TOKEN_KEYWORD_SWITCH:
                    break;
                case TK::TOKEN_KEYWORD_RETURN:
                    break;
                case TK::TOKEN_KEYWORD_BREAK:
                    break;
                case TK::TOKEN_KEYWORD_CONTINUE:
                    break;
                case TK::TOKEN_IDENTIFIER:
                    break;
                case TK::TOKEN_LBRACE:
                    pos++;
                    ParseScope(tokens,pos);
                    break;
                case TK::TOKEN_RBRACE:
                    con = false;
                    break;
                case TK::TOKEN_GT:
                    pos++;
                    break;
                default:
                    pos++;
            }
        }
    }
    void threadEntry(lexer::TokenFile tokens,unsigned int id,Settings flags,Node* ast){
        size_t pos=0;
        ParseScope(tokens.tokens,pos);
    }
    void entry(std::vector<lexer::TokenFile> tokens,Settings flag,std::vector<Syntax>& syn){
        Node* ast;
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"creating ",tokens.size()," Tasks<SYNTAX>");
        syn.resize(tokens.size());
        for(unsigned int i = 0; i < static_cast<unsigned int>(tokens.size()); i++){
            syn[i].filePath = tokens[i].filePath;
            pool.enqueue([i, &tokens, &syn, flag]{
                unsigned int id = getID();
                if(flag.showStatus)
                    output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"the new Thread<SYNTAX>[",id,"] was created.");
                threadEntry(tokens[i], id, flag, syn[i].tree.get());
            });
        }
        pool.waitAll();
        if(flag.showStatus)
            output::println(output::UseColor(output::FG_GREEN),"REPORT<MAIN>: ",output::UseColor(output::FG_DEFAULT),"all Threads<SYNTAX> are finished.");
    }
}