#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast.h"
#include "../symtable.h"
#include <math.h>

#define MAX_VARS 100

static struct {
    char name[32];
    double value;
} vars[MAX_VARS];

static int var_count = 0;

double get_var(const char *name) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;

    printf("Variable no definida: %s\n", name);
    return 0;
}

void set_var(const char *name, double value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    strcpy(vars[var_count].name, name);
    vars[var_count].value = value;
    var_count++;
}

AST *new_num(double v) {
    AST *n = malloc(sizeof(AST));
    n->type = AST_NUM;
    n->value = v;
    return n;
}

AST *new_var(const char *name) {
    AST *n = malloc(sizeof(AST));
    n->type = AST_VAR;
    strcpy(n->name, name);
    return n;
}

AST *new_binop(char op, AST *l, AST *r) {
    AST *n = malloc(sizeof(AST));
    n->type = AST_BINOP;
    n->op = op;
    n->left = l;
    n->right = r;
    return n;
}

AST *new_assign(const char *name, AST *value) {
    AST *n = malloc(sizeof(AST));
    n->type = AST_ASSIGN;
    strcpy(n->name, name);
    n->right = value;
    return n;
}

double eval(AST *n) {
    switch (n->type) {
        case AST_NUM: return n->value;
        case AST_VAR: return get_var(n->name);
        case AST_ASSIGN: {
            double v = eval(n->right);
            set_var(n->name, v);
            return v;
        }
        case AST_BINOP: {
            double a = eval(n->left);
            double b = eval(n->right);
            switch (n->op) {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return a / b;
                case '^': return pow(a,b);
            }
        }
    }
    return 0;
}