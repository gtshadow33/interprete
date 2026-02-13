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
        current = get_next_token();
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

/*
 * parse()
 * Punto de entrada del parser.
 * Devuelve el AST completo de la expresión.
 */
AST *parse(void) {
    current = get_next_token();  // Primer token

    // Verifica si es asignación: ID = expr
    if (current.type == TOK_ID) {
        Token id = current;
        eat(TOK_ID);

        if (current.type == TOK_ASSIGN) {
            eat(TOK_ASSIGN);
            return new_assign(id.name, expr());
        }

        // Si no es asignación, reconstruimos como expresión normal
        // Simulamos que el ID es un factor ya leído
        AST *node = new_var(id.name);

        // Continuamos procesando con la jerarquía normal
        while (current.type == TOK_EXP) {
            eat(TOK_EXP);
            node = new_binop('^', node, power());
        }

        while (current.type == TOK_MUL || current.type == TOK_DIV) {
            char op = (current.type == TOK_MUL) ? '*' : '/';
            eat(current.type);
            node = new_binop(op, node, power());
        }

        while (current.type == TOK_PLUS || current.type == TOK_MINUS) {
            char op = (current.type == TOK_PLUS) ? '+' : '-';
            eat(current.type);
            node = new_binop(op, node, term());
        }

        return node;
    }

    return expr();
}

/*
 * expr()
 * Maneja suma y resta.
 */
static AST *expr(void) {
    AST *node = term();

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
    AST *node = power();

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
