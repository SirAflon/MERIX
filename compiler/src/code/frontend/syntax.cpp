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
    void ParseScope(const std::vector<lexer::Token>& tokens,size_t& pos);
    
    std::string ParseHelperREG(const std::vector<lexer::Token>& tokens,size_t& pos){
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
    std::string ParseHelperCallVREG(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        std::string iden;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_GT:
                    con=false;
                    break;
                case TK::TOKEN_LT:
                    break;
                case TK::TOKEN_IDENTIFIER:
                    iden = tokens[pos].lexeme;
                    pos++;
                    break;
                default:
                    pos++;
            }
        }
        return iden;
    }
    void ParseREG(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        metaMove me;
        while(con&&pos<tokens.size()){
            switch(tokens[pos].kind){
                case TK::TOKEN_ARROW_RIGHT:
                    pos++;
                    break;
                case TK::TOKEN_ARROW_LEFT:
                    me.isLeft = true;
                    pos++;
                    break;
                case TK::TOKEN_KEYWORD_REG:
                    pos++;
                    if(me.one.empty()){
                        me.one = ParseHelperREG(tokens,pos);
                        me.ofOne = regType::REG;
                    }else if(me.two.empty()){
                        me.two = ParseHelperREG(tokens,pos);
                        me.ofTwo = regType::REG;
                    }
                    break;
                case TK::TOKEN_KEYWORD_VREG:
                    pos++;
                    if(me.one.empty()){
                        me.one = ParseHelperCallVREG(tokens,pos);
                        me.ofOne = regType::VREG;
                    }else if(me.two.empty()){
                        me.two = ParseHelperCallVREG(tokens,pos);
                        me.ofTwo = regType::VREG;
                    }
                    break;
                case TK::TOKEN_SEMICOLON:
                    pos++;
                    con=false;
                    break;
                default:
                    break;
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
                    break;
                case TK::TOKEN_KEYWORD_REG:
                    pos++;
                    ParseREG(tokens,pos);
                    break;
                case TK::TOKEN_KEYWORD_VREG:
                    pos++;
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