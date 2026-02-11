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
static AST *power(void);
static AST *factor(void);
static AST *expr_from_node(AST *node);

AST *parse(void) {
    current = get_next_token();

    if (current.type == TOK_ID) {
        Token id = current;
        current = get_next_token();
        if (current.type == TOK_ASSIGN) {
            eat(TOK_ASSIGN);
            return new_assign(id.name, expr());
        }
        // Si no es asignación, volvemos atrás creando el nodo de variable
        // y continuamos con expr_from_node que procesa el resto
        AST *left = new_var(id.name);
        return expr_from_node(left);
    }

    return expr();
}

// Función auxiliar: continúa procesando + - * / desde un nodo existente
static AST *expr_from_node(AST *node) {
    // Primero procesa * y / (mayor precedencia)
    while (current.type == TOK_MUL || current.type == TOK_DIV) {
        char op = (current.type == TOK_MUL) ? '*' : '/';
        eat(current.type);
        node = new_binop(op, node, factor());
    }
    
    // Luego procesa + y - (menor precedencia)
    while (current.type == TOK_PLUS || current.type == TOK_MINUS) {
        char op = (current.type == TOK_PLUS) ? '+' : '-';
        eat(current.type);
        AST *right = factor();
        
        // Procesa * y / en el lado derecho
        while (current.type == TOK_MUL || current.type == TOK_DIV) {
            char op2 = (current.type == TOK_MUL) ? '*' : '/';
            eat(current.type);
            right = new_binop(op2, right, factor());
        }
        
        node = new_binop(op, node, right);
    }
    return node;
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
    AST *node = power();

    while (current.type == TOK_MUL || current.type == TOK_DIV) {
        char op = (current.type == TOK_MUL) ? '*' : '/';
        eat(current.type);
        node = new_binop(op, node, power());
    }

    return node;
}

static AST *power(void) {
    AST *node = factor();

    if (current.type == TOK_EXP) {
        eat(TOK_EXP);
        // Recursión a la derecha para asociatividad derecha: 2^3^2 = 2^(3^2)
        node = new_binop('^', node, power());
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