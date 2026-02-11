#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include "lexer.h"

static const char *src;
static int pos;

void init_lexer(const char *input) {
    src = input;
    pos = 0;
}

Token get_next_token(void) {
    Token tok;

    while (isspace(src[pos])) pos++;
    char c = src[pos];

    if (isdigit(c) || c == '.') {
        char buf[64];
        int i = 0;
        while (isdigit(src[pos]) || src[pos] == '.') {
            buf[i++] = src[pos++];
        }
        buf[i] = '\0';
        tok.type = TOK_NUM;
        tok.value = atof(buf);
        return tok;
    }

    if (isalpha(c)) {
        int i = 0;
        while (isalnum(src[pos])) {
            tok.name[i++] = src[pos++];
        }
        tok.name[i] = '\0';
        tok.type = TOK_ID;
        return tok;
    }

    pos++;

    switch (c) {
        case '+': tok.type = TOK_PLUS; break;
        case '-': tok.type = TOK_MINUS; break;
        case '*': tok.type = TOK_MUL; break;
        case '/': tok.type = TOK_DIV; break;
        case '^': tok.type = TOK_EXP; break;
        case '=': tok.type = TOK_ASSIGN; break;
        case '(': tok.type = TOK_LPAREN; break;
        case ')': tok.type = TOK_RPAREN; break;
        case '\0': tok.type = TOK_EOF; break;
        default:
            printf("Carácter inválido: %c\n", c);
            tok.type = TOK_EOF;
    }

    return tok;
}
