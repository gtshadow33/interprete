#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOK_NUM,
    TOK_ID,
    TOK_ASSIGN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    double value;
    char name[32];
} Token;

#endif
