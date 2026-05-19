#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>

typedef enum {
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_LT,
        TOKEN_GT,
        TOKEN_COMMA,
        TOKEN_SEMICOLON,
        TOKEN_COLON,
        TOKEN_DOT,
        TOKEN_DOTDOT,
        TOKEN_ASSIGN,
        TOKEN_QUESTIONMARK,

        TOKEN_PLUS_EQ,
        TOKEN_MINUS_EQ,
        TOKEN_STAR_EQ,
        TOKEN_SLASH_EQ,
        TOKEN_PERCENT_EQ,
        TOKEN_AMP_EQ,
        TOKEN_PIPE_EQ,
        TOKEN_CARET_EQ,
        TOKEN_SHIFT_LEFT_EQ,
        TOKEN_SHIFT_RIGHT_EQ,
        TOKEN_EQ_EQ,
        TOKEN_NOT_EQ,
        TOKEN_LT_EQ,
        TOKEN_GT_EQ,
        TOKEN_AND_AND,
        TOKEN_PIPE_PIPE,
        TOKEN_SHIFT_LEFT,
        TOKEN_SHIFT_RIGHT,
        TOKEN_INC,
        TOKEN_DEC,
        TOKEN_ARROW,

        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_STAR,
        TOKEN_SLASH,
        TOKEN_PERCENT,
        TOKEN_AMP,
        TOKEN_PIPE,
        TOKEN_CARET,
        TOKEN_TILDE,
        TOKEN_EXCLAM,
        TOKEN_SCOPE,

        TOKEN_INTEGER,//
        TOKEN_FLOAT,//
        TOKEN_CHAR,//
        TOKEN_STRING,//
        TOKEN_ERROR,//
        TOKEN_UNKNOWN,//

        TOKEN_IDENTIFIER,
        TOKEN_COMMENT,//

        TOKEN_KEYWORD_MEM,
        TOKEN_KEYWORD_REG,
        TOKEN_KEYWORD_VREG,
        TOKEN_KEYWORD_STRUCT,
        TOKEN_KEYWORD_PUBLIC,//
        TOKEN_KEYWORD_PRIVATE,//
        TOKEN_KEYWORD_INCLUDE,//
        TOKEN_KEYWORD_TYP,
        TOKEN_KEYWORD_VAL,
        TOKEN_KEYWORD_OVERRIDE,
        TOKEN_KEYWORD_DEFINE,//
        TOKEN_KEYWORD_MACRO,//
        TOKEN_KEYWORD_OPERATION,//
        TOKEN_KEYWORD_TRY,//
        TOKEN_KEYWORD_CATCH,//
        TOKEN_KEYWORD_THROW,//
        TOKEN_KEYWORD_FINALLY,//
        TOKEN_KEYWORD_IF,
        TOKEN_KEYWORD_ELSE,//
        TOKEN_KEYWORD_FOR,
        TOKEN_KEYWORD_WHILE,
        TOKEN_KEYWORD_DO,
        TOKEN_KEYWORD_SWITCH,
        TOKEN_KEYWORD_CASE,//
        TOKEN_KEYWORD_DEFAULT,//
        TOKEN_KEYWORD_BREAK,
        TOKEN_KEYWORD_RETURN,
        TOKEN_KEYWORD_CONTINUE,
        TOKEN_KEYWORD_ERR,//
        TOKEN_KEYWORD_META,
}TokenKind;

static const int MAX_TOKEN_LEN = 3;

static const std::unordered_map<std::string_view, TokenKind> tokenMapSymbols = {
    {"(", TOKEN_LPAREN},
    {")", TOKEN_RPAREN},
    {"{", TOKEN_LBRACE},
    {"}", TOKEN_RBRACE},
    {"[", TOKEN_LBRACKET},
    {"]", TOKEN_RBRACKET},
    {"<", TOKEN_LT},
    {">", TOKEN_GT},
    {",", TOKEN_COMMA},
    {";", TOKEN_SEMICOLON},
    {":", TOKEN_COLON},
    {".", TOKEN_DOT},
    {"=", TOKEN_ASSIGN},
    {"+=", TOKEN_PLUS_EQ},
    {"-=", TOKEN_MINUS_EQ},
    {"*=", TOKEN_STAR_EQ},
    {"/=", TOKEN_SLASH_EQ},
    {"%=", TOKEN_PERCENT_EQ},
    {"&=", TOKEN_AMP_EQ},
    {"|=", TOKEN_PIPE_EQ},
    {"^=", TOKEN_CARET_EQ},
    {"<<=", TOKEN_SHIFT_LEFT_EQ},
    {">>=", TOKEN_SHIFT_RIGHT_EQ},
    {"==", TOKEN_EQ_EQ},
    {"!=", TOKEN_NOT_EQ},
    {"<=", TOKEN_LT_EQ},
    {">=", TOKEN_GT_EQ},
    {"&&", TOKEN_AND_AND},
    {"||", TOKEN_PIPE_PIPE},
    {"<<", TOKEN_SHIFT_LEFT},
    {">>", TOKEN_SHIFT_RIGHT},
    {"++", TOKEN_INC},
    {"--", TOKEN_DEC},
    {"->", TOKEN_ARROW},
    {"//", TOKEN_COMMENT},
    {"+", TOKEN_PLUS},
    {"-", TOKEN_MINUS},
    {"*", TOKEN_STAR},
    {"/", TOKEN_SLASH},
    {"%", TOKEN_PERCENT},
    {"&", TOKEN_AMP},
    {"|", TOKEN_PIPE},
    {"^", TOKEN_CARET},
    {"~", TOKEN_TILDE},
    {"!", TOKEN_EXCLAM},
    {"?", TOKEN_QUESTIONMARK},
    {"::", TOKEN_SCOPE},
    {"..", TOKEN_DOTDOT},
};

static const std::unordered_map<std::string_view, TokenKind> tokenMapKeywords{
    {"mem", TOKEN_KEYWORD_MEM},
    {"reg", TOKEN_KEYWORD_REG},
    {"vreg", TOKEN_KEYWORD_VREG},
    {"struct", TOKEN_KEYWORD_STRUCT},
    {"public", TOKEN_KEYWORD_PUBLIC},
    {"private", TOKEN_KEYWORD_PRIVATE},
    {"include", TOKEN_KEYWORD_INCLUDE},
    {"typ", TOKEN_KEYWORD_TYP},
    {"val", TOKEN_KEYWORD_VAL},
    {"override", TOKEN_KEYWORD_OVERRIDE},
    {"define", TOKEN_KEYWORD_DEFINE},
    {"macro", TOKEN_KEYWORD_MACRO},
    {"operation", TOKEN_KEYWORD_OPERATION},
    {"try", TOKEN_KEYWORD_TRY},
    {"catch", TOKEN_KEYWORD_CATCH},
    {"throw", TOKEN_KEYWORD_THROW},
    {"finally", TOKEN_KEYWORD_FINALLY},
    {"if", TOKEN_KEYWORD_IF},
    {"else", TOKEN_KEYWORD_ELSE},
    {"for", TOKEN_KEYWORD_FOR},
    {"while", TOKEN_KEYWORD_WHILE},
    {"do", TOKEN_KEYWORD_DO},
    {"switch", TOKEN_KEYWORD_SWITCH},
    {"case", TOKEN_KEYWORD_CASE},
    {"default", TOKEN_KEYWORD_DEFAULT},
    {"break", TOKEN_KEYWORD_BREAK},
    {"return", TOKEN_KEYWORD_RETURN},
    {"continue", TOKEN_KEYWORD_CONTINUE},
    {"err", TOKEN_KEYWORD_ERR},
    {"meta", TOKEN_KEYWORD_META},
};

static const std::unordered_set<char> tokenBreakChar{
    '(',')','[',']','{','}','<','>',
    ';',':','!','&','=','|',',','.',
    '+','-','*','/','%','^','~',
    '"','\'',' '
};