#pragma once
#include "createSyntaxData.h"
#include "inputParser.h"
#include "syntaxTree.h"
#include "lexerTokens.h"

namespace syntax{
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
}