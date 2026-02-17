#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUM,
    AST_VAR,
    AST_BINOP,
    AST_ASSIGN
} ASTType;

typedef struct AST {
    ASTType type;
    double value;
    char name[32];
    char op;
    struct AST *left;
    struct AST *right;
} AST;

AST *new_num(double v);
AST *new_var(const char *name);
AST *new_binop(char op, AST *l, AST *r);
AST *new_assign(const char *name, AST *value);

double eval(AST *node);

#endif
