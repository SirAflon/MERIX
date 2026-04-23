#pragma once
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
        TOKEN_KEYWORD_CLASS,
        TOKEN_KEYWORD_SIGNED,
        TOKEN_KEYWORD_UNSIGNED,
        TOKEN_KEYWORD_CONST,
        TOKEN_KEYWORD_PUBLIC,
        TOKEN_KEYWORD_PRIVATE,
        TOKEN_KEYWORD_INCLUDE,
        TOKEN_KEYWORD_AS,
        TOKEN_KEYWORD_TYP,
        TOKEN_KEYWORD_VAL,
        TOKEN_KEYWORD_REQUIRES,
        TOKEN_KEYWORD_OVERRIDE,
        TOKEN_KEYWORD_TRUE,
        TOKEN_KEYWORD_FALSE,
        TOKEN_KEYWORD_NULLPTR,
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