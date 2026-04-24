#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>

typedef enum {
        // === Single‑character tokens (brackets, punctuation) ===
        TOKEN_LPAREN,          // (
        TOKEN_RPAREN,          // )
        TOKEN_LBRACE,          // {
        TOKEN_RBRACE,          // }
        TOKEN_LBRACKET,        // [
        TOKEN_RBRACKET,        // ]
        TOKEN_LT,              // <
        TOKEN_GT,              // >
        TOKEN_COMMA,           // ,
        TOKEN_SEMICOLON,       // ;
        TOKEN_COLON,           // :
        TOKEN_DOT,             // .
        TOKEN_DOTDOT,          // ..
        TOKEN_ASSIGN,          // =

        // === Two‑character operators ===
        TOKEN_PLUS_EQ,         // +=
        TOKEN_MINUS_EQ,        // -=
        TOKEN_STAR_EQ,         // *=
        TOKEN_SLASH_EQ,        // /=
        TOKEN_PERCENT_EQ,      // %=
        TOKEN_AMP_EQ,          // &=
        TOKEN_PIPE_EQ,         // |=
        TOKEN_CARET_EQ,        // ^=
        TOKEN_SHIFT_LEFT_EQ,   // <<=
        TOKEN_SHIFT_RIGHT_EQ,  // >>=
        TOKEN_EQ_EQ,           // ==
        TOKEN_NOT_EQ,          // !=
        TOKEN_LT_EQ,           // <=
        TOKEN_GT_EQ,           // >=
        TOKEN_AND_AND,         // &&
        TOKEN_PIPE_PIPE,       // ||
        TOKEN_SHIFT_LEFT,      // <<
        TOKEN_SHIFT_RIGHT,     // >>
        TOKEN_INC,             // ++
        TOKEN_DEC,             // --
        TOKEN_ARROW,           // ->

        // === Single‑character operators (not covered above) ===
        TOKEN_PLUS,            // +
        TOKEN_MINUS,           // -
        TOKEN_STAR,            // *
        TOKEN_SLASH,           // /
        TOKEN_PERCENT,         // %
        TOKEN_AMP,             // &
        TOKEN_PIPE,            // |
        TOKEN_CARET,           // ^
        TOKEN_TILDE,           // ~
        TOKEN_EXCLAM,          // !
        TOKEN_SCOPE,           // ::

        // === Literals ===
        TOKEN_INTEGER,         // 123, 0
        TOKEN_CHAR,            // 'a', '\n'
        TOKEN_STRING,          // "hello"

        // === Identifiers & keywords (keywords are a subset of identifiers) ===
        TOKEN_IDENTIFIER,

        // Keywords – listed explicitly for easy checking
        TOKEN_KEYWORD_MEM,
        TOKEN_KEYWORD_STRUCT,
        TOKEN_KEYWORD_SIGNED,
        TOKEN_KEYWORD_UNSIGNED,
        TOKEN_KEYWORD_CONST,
        TOKEN_KEYWORD_PUBLIC,
        TOKEN_KEYWORD_PRIVATE,
        TOKEN_KEYWORD_INCLUDE,
        TOKEN_KEYWORD_AS,
        TOKEN_KEYWORD_TYP,
        TOKEN_KEYWORD_VAL,
        TOKEN_KEYWORD_OVERRIDE,
        TOKEN_KEYWORD_DEFINE,
        TOKEN_KEYWORD_MACRO,
        TOKEN_KEYWORD_OPERATION,
        TOKEN_KEYWORD_TRY,
        TOKEN_KEYWORD_CATCH,
        TOKEN_KEYWORD_THROW,
        TOKEN_KEYWORD_FINALLY,
        TOKEN_KEYWORD_IF,
        TOKEN_KEYWORD_ELSE,
        TOKEN_KEYWORD_FOR,
        TOKEN_KEYWORD_WHILE,
        TOKEN_KEYWORD_DO,
        TOKEN_KEYWORD_SWITCH,
        TOKEN_KEYWORD_CASE,
        TOKEN_KEYWORD_DEFAULT,
        TOKEN_KEYWORD_BREAK,
        TOKEN_KEYWORD_RETURN,
        TOKEN_KEYWORD_CONTINUE,
} TokenKind;

static const int MAX_TOKEN_LEN = 3;
static const std::unordered_map<std::string, TokenKind> tokenMapSymbols = {
    // Single‑character tokens (brackets, punctuation)
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

    // Two‑character and three‑character operators
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

    // Remaining single‑character operators
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

    // Multi‑character punctuation
    {"::", TOKEN_SCOPE},
    {"..", TOKEN_DOTDOT},
};
static const std::unordered_map<std::string, TokenKind> tokenMapKeywords{
    // Keywords (all fixed strings)
    {"mem", TOKEN_KEYWORD_MEM},
    {"struct", TOKEN_KEYWORD_STRUCT},
    {"signed", TOKEN_KEYWORD_SIGNED},
    {"unsigned", TOKEN_KEYWORD_UNSIGNED},
    {"const", TOKEN_KEYWORD_CONST},
    {"public", TOKEN_KEYWORD_PUBLIC},
    {"private", TOKEN_KEYWORD_PRIVATE},
    {"include", TOKEN_KEYWORD_INCLUDE},
    {"as", TOKEN_KEYWORD_AS},
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
};