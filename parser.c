#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "token.h"

// Token actual que estamos analizando
static Token current;

/*
 * eat(t)
 * Verifica que el token actual sea del tipo esperado.
 * Si coincide, avanza al siguiente token.
 * Si no coincide, lanza error de sintaxis.
 */
static void eat(TokenType t) {
    if (current.type == t)
        current = get_next_token();  // Avanza al siguiente token
    else {
        printf("Error de sintaxis\n");
        exit(1);
    }
}

// Declaraciones adelantadas
static AST *expr(void);
static AST *term(void);
static AST *power(void);
static AST *factor(void);
static AST *expr_from_node(AST *node);

/*
 * parse()
 * Punto de entrada del parser.
 * Devuelve el AST completo de la expresión.
 */
AST *parse(void) {
    current = get_next_token();  // Obtiene el primer token

    // Si empieza con identificador puede ser asignación
    if (current.type == TOK_ID) {
        Token id = current;
        current = get_next_token();

        // Caso: x = algo
        if (current.type == TOK_ASSIGN) {
            eat(TOK_ASSIGN);
            return new_assign(id.name, expr());
        }

        // Caso: empieza con variable pero no es asignación
        AST *left = new_var(id.name);
        return expr_from_node(left);
    }

    // Si no es ID, procesamos como expresión normal
    return expr();
}

/*
 * expr_from_node()
 * Continúa procesando operadores cuando ya tenemos
 * un nodo inicial (ej: variable).
 */
static AST *expr_from_node(AST *node) {

    // Primero procesa * y /
    while (current.type == TOK_MUL || current.type == TOK_DIV) {
        char op = (current.type == TOK_MUL) ? '*' : '/';
        eat(current.type);
        node = new_binop(op, node, factor());
    }
    
    // Luego procesa + y -
    while (current.type == TOK_PLUS || current.type == TOK_MINUS) {
        char op = (current.type == TOK_PLUS) ? '+' : '-';
        eat(current.type);

        AST *right = factor();

        // Maneja multiplicación o división del lado derecho
        while (current.type == TOK_MUL || current.type == TOK_DIV) {
            char op2 = (current.type == TOK_MUL) ? '*' : '/';
            eat(current.type);
            right = new_binop(op2, right, factor());
        }
        
        node = new_binop(op, node, right);
    }

    return node;
}

/*
 * expr()
 * Maneja suma y resta.
 * Nivel más bajo de precedencia.
 */
static AST *expr(void) {
    AST *node = term();  // Primero procesa * y /

    while (current.type == TOK_PLUS || current.type == TOK_MINUS) {
        char op = (current.type == TOK_PLUS) ? '+' : '-';
        eat(current.type);
        node = new_binop(op, node, term());
    }

    return node;
}

/*
 * term()
 * Maneja multiplicación y división.
 */
static AST *term(void) {
    AST *node = power();  // Primero procesa potencia

    while (current.type == TOK_MUL || current.type == TOK_DIV) {
        char op = (current.type == TOK_MUL) ? '*' : '/';
        eat(current.type);
        node = new_binop(op, node, power());
    }

    return node;
}

/*
 * power()
 * Maneja operador ^
 * Asociatividad derecha.
 */
static AST *power(void) {
    AST *node = factor();

    if (current.type == TOK_EXP) {
        eat(TOK_EXP);
        node = new_binop('^', node, power());
    }

    return node;
}

/*
 * factor()
 * Nivel base del parser.
 * Maneja:
 *   - números
 *   - variables
 *   - expresiones entre paréntesis
 */
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
