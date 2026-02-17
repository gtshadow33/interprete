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

    // -------- NÚMEROS --------
    if (isdigit(c) || c == '.') {
        char buf[64];
        int i = 0;

        // -------- HEXADECIMAL --------
        if (src[pos] == '0' && (src[pos+1] == 'x' || src[pos+1] == 'X')) {
            buf[i++] = src[pos++]; // 0
            buf[i++] = src[pos++]; // x

            while (isxdigit(src[pos])) {
                if (i >= 63) break;
                buf[i++] = src[pos++];
            }

            buf[i] = '\0';

            tok.type = TOK_NUM;
            tok.value = (double)strtol(buf, NULL, 0);
            return tok;
        }
        // -------- BINARIO --------
        if (src[pos] == '0' && (src[pos+1] == 'b' || src[pos+1] == 'B')) {
            buf[i++] = src[pos++]; // 0
            buf[i++] = src[pos++]; // b

            while (src[pos] == '0' || src[pos] == '1') {
                if (i >= 63) break;
                buf[i++] = src[pos++];
            }

            buf[i] = '\0';

            // strtol NO acepta 0b, así que usamos base 2 manual
            tok.type = TOK_NUM;
            tok.value = (double)strtol(buf + 2, NULL, 2);
            return tok;
        }


        // -------- DECIMAL / FLOTANTE / NOTACIÓN CIENTÍFICA --------

        // Parte entera
        while (isdigit(src[pos])) {
            if (i >= 63) break;
            buf[i++] = src[pos++];
        }

        // Parte decimal
        if (src[pos] == '.') {
            if (i < 63) buf[i++] = src[pos++];
            
            while (isdigit(src[pos])) {
                if (i >= 63) break;
                buf[i++] = src[pos++];
            }
        }

        // Parte exponencial
        if (src[pos] == 'e' || src[pos] == 'E') {
            if (i < 63) buf[i++] = src[pos++];

            // signo opcional SOLO después de e
            if (src[pos] == '+' || src[pos] == '-') {
                if (i < 63) buf[i++] = src[pos++];
            }

            // Debe haber al menos un dígito en el exponente
            if (!isdigit(src[pos])) {
                printf("Error: exponente inválido\n");
            }

            while (isdigit(src[pos])) {
                if (i >= 63) break;
                buf[i++] = src[pos++];
            }
        }

        buf[i] = '\0';

        tok.type = TOK_NUM;
        tok.value = strtod(buf, NULL);
        return tok;
    }

    // -------- IDENTIFICADORES --------
    if (isalpha(c)) {
        int i = 0;

        while (isalnum(src[pos])) {
            if (i >= 63) break;
            tok.name[i++] = src[pos++];
        }

        tok.name[i] = '\0';
        tok.type = TOK_ID;
        return tok;
    }

    // -------- OPERADORES --------
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
