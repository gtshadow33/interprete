#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "token.h"

static Token current;

static void eat(TokenType t) {
    if (current.type == t)
        current = get_next_token();
    else {
        printf("Error de sintaxis\n");
        exit(1);
    }
}

static AST *expr(void);
static AST *term(void);
static AST *factor(void);

AST *parse(void) {
    current = get_next_token();

    if (current.type == TOK_ID) {
        Token id = current;
        current = get_next_token();
        if (current.type == TOK_ASSIGN) {
            eat(TOK_ASSIGN);
            return new_assign(id.name, expr());
        }
    }

    return expr();
}

static AST *expr(void) {
    AST *node = term();

    while (current.type == TOK_PLUS || current.type == TOK_MINUS) {
        char op = (current.type == TOK_PLUS) ? '+' : '-';
        eat(current.type);
        node = new_binop(op, node, term());
    }

    return node;
}

static AST *term(void) {
    AST *node = factor();

    while (current.type == TOK_MUL || current.type == TOK_DIV) {
        char op = (current.type == TOK_MUL) ? '*' : '/';
        eat(current.type);
        node = new_binop(op, node, factor());
    }

    return node;
}

static AST *factor(void) {
    Token t = current;

    if (t.type == TOK_NUM) {
        eat(TOK_NUM);
        return new_num(t.value);
    }

    if (t.type == TOK_ID) {
        eat(TOK_ID);
        return new_var(t.name);
    }

    if (t.type == TOK_LPAREN) {
        eat(TOK_LPAREN);
        AST *node = expr();
        eat(TOK_RPAREN);
        return node;
    }

    printf("Factor inválido\n");
    exit(1);
}
