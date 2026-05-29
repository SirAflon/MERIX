#include "../../header/frontend/syntax.h"
#include "../../lib/header/contextTokens.h"
#include "../../lib/header/output.h"
#include <thread>

namespace syntax {
    struct metaDataType{
        std::vector<std::string> parentStructs;
        std::string intBits;
        std::string floatBits;
        std::string reg="";
        int arrayLength=0;
        bool isArray=false;
        bool isVoid = true;
        bool isRef = false;
        bool isPointer = false;
    };
    struct metaVReg{
        std::vector<std::string> para;
        std::string name;
        std::string reg="";
        bool isCall=false;
    };
    struct metaArray{
        std::vector<lexer::Token> para;
        std::string name;
        bool isCall=true;
        bool isRange=false;
    };
    static ThreadPool pool(std::max(1u, std::thread::hardware_concurrency()));
    std::atomic<unsigned int> freeID = 0;
    unsigned int getID(){
        return freeID.fetch_add(1);
    }
    void ParseIndexBracked(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseIdentefier(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseRuntime(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseREG(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseMEM(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseVREG(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseMeta(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseIF(const std::vector<lexer::Token>& tokens,size_t& pos);
    void ParseScope(const std::vector<lexer::Token>& tokens,size_t& pos);
    std::unique_ptr<Node> createReg(const std::string& name){
        auto node = std::make_unique<RegPinnedType>();
        node ->register_name = name;
        return node;
    }
    std::unique_ptr<Node> createDataType(const metaDataType& info){
        if(info.isVoid){
            auto node = std::make_unique<MemType>();
            return node;
        }
        if(info.isPointer){
            auto node = std::make_unique<PointerType>();
            auto typeNode = info;
            typeNode.isPointer = false;
            node -> pointee_type = createDataType(typeNode);
            return node;
        }
        if(info.isRef){
            auto node = std::make_unique<ReferenceType>();
            auto typeNode = info;
            typeNode.isRef = false;
            node -> referenced_type = createDataType(typeNode);
            return node;
        }
        if(info.isArray){
            auto node = std::make_unique<ArrayType>();
            auto typeNode = info;
            typeNode.isArray = false;
            node -> element_type = createDataType(typeNode);
            node -> size = info.arrayLength;
            return node;
        }
        if(info.parentStructs.size() > 0){
            auto node = std::make_unique<StructType>();
            for(const std::string& item : info.parentStructs)
                node ->struct_name.push_back(item);
            return node;
        }
        auto node = std::make_unique<MemType>();
        node ->integer_bits = std::stoll(info.intBits);
        node ->fraction_bits = std::stoll(info.floatBits);
        if(info.reg.size() > 0)
            node ->reg_pin = createReg(info.reg);
        return node;
    }
    std::unique_ptr<Node> createChar(const char& chr){
        auto node = std::make_unique<CharLiteral>();
        node ->value = chr;
        return node;
    }
    std::unique_ptr<Node> createString(const lexer::Token& token){
        auto node = std::make_unique<InitializerListExpr>();
        for(size_t i = 0; i<token.lexeme.size();i++)
            node ->elements.push_back(createChar(token.lexeme[i]));
        return node;
    }
    std::unique_ptr<Node> createInt(const std::string& num){
        auto node = std::make_unique<IntegerLiteral>();
        node ->value = std::stoll(num);
        return node;
    }
    std::unique_ptr<Node> createFloat(const std::string& num){
        auto node = std::make_unique<FloatLiteral>();
        node ->value = std::stoll(num);
        return node;
    }
    std::unique_ptr<Node> createCallVReg(const metaVReg& info){
        auto node = std::make_unique<VRegRefExpr>();
        node ->variable_name = info.name;
        return node;
    }
    std::unique_ptr<Node> createVarVReg(const metaVReg& info){
        auto node = std::make_unique<VRegType>();
        if(info.reg.size() > 0){
            node ->parameters.push_back(createReg(info.reg));
            return node;
        }
        for(size_t i=0;i < info.para.size();i++)
            node ->parameters.push_back(createInt(info.para[i]));
        return node;
    }
    std::unique_ptr<Node> createVReg(const metaVReg& info){
        if(info.isCall)
            return createCallVReg(info);
        return createVarVReg(info);
    }
    std::unique_ptr<Node> createIdentefier(const std::string& name){
        auto node = std::make_unique<IdentifierExpr>();
        node ->name = name;
        return node;
    }
    std::unique_ptr<Node> createCallArray(const metaArray& info){
        auto node = std::make_unique<ArrayIndexExpr>();
        node ->array = createIdentefier(info.name);
        node ->index = createInt(info.para[0].lexeme);
        return node;
    }
    std::unique_ptr<Node> createLit(const lexer::Token& para){
        using TK = TokenKind;
        switch(para.kind){
            case TK::TOKEN_INTEGER:
                return createInt(para.lexeme);
            case TK::TOKEN_FLOAT:
                return createFloat(para.lexeme);
            case TK::TOKEN_CHAR:
                return createChar(*para.lexeme.c_str());
            case TK::TOKEN_STRING:
                return createString(para);
        }
    }
    std::unique_ptr<Node> createNewArray(const metaArray& info){
        auto node = std::make_unique<InitializerListExpr>();
        for(size_t i=0;i<info.para.size();i++)
            node ->elements.push_back(createLit(info.para[i]));
        return node;
    }
    std::unique_ptr<Node> createArray(const metaArray& info){
        if(info.isCall)
            return createCallArray(info);
        else
            return createNewArray(info);
    }
    void ParseIndexBracked(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con = true;
        std::vector<lexer::Token> in;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_IDENTIFIER:
                    ParseIdentefier(tokens,pos);
                    break;
                case TK::TOKEN_INTEGER:
                case TK::TOKEN_FLOAT:
                case TK::TOKEN_CHAR:
                case TK::TOKEN_STRING:
                    in.push_back(tokens[pos]);
                    pos++;
                    break;
                case TK::TOKEN_LBRACKET:
                    ParseIndexBracked(tokens,pos);
                    break;
                case TK::TOKEN_RBRACKET:
                    con=false;
                    break;
                case TK::TOKEN_LPAREN:
                    pos++;
                    ParseRuntime(tokens,pos);
                    break;
                case TK::TOKEN_COMMA:
                    pos++;
                    break;
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
                default:
                    pos++;
            }
        }
    }
    void ParseIdentefier(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con = true;
        size_t start = pos;
        pos++;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_DOT:
                    pos++;
                    ParseIdentefier(tokens,pos);
                    break;
                case TK::TOKEN_INC:
                    break;
                case TK::TOKEN_DEC:
                    break;
                case TK::TOKEN_LPAREN:
                    ParseRuntime(tokens,pos);
                    break;
                case TK::TOKEN_LBRACKET:
                    pos++;
                    ParseIndexBracked(tokens,pos);
                    break;
                default:
                    con=false;
            }
        }
    }
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
    void ParseMeta(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_GT:
                    con=false;
                    break;
                case TK::TOKEN_LT:
                    break;
                case TK::TOKEN_KEYWORD_META:
                    break;
                case TK::TOKEN_COMMA:
                    pos++;
                    break;
                case TK::TOKEN_IDENTIFIER:
                    break;
                case TK::TOKEN_KEYWORD_VAL:
                    break;
                case TK::TOKEN_KEYWORD_TYP:
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
                    ParseIdentefier(tokens,pos);
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
    void ParseIF(const std::vector<lexer::Token>& tokens,size_t& pos){
        using TK = TokenKind;
        bool con=true;
        while(pos < tokens.size()&&con){
            switch(tokens[pos].kind){
                case TK::TOKEN_LPAREN:
                    pos++;
                    ParseRuntime(tokens,pos);
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
                    ParseIF(tokens,pos);
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
                    ParseMeta(tokens,pos);
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