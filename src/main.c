#include <stdio.h>
#include <string.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast.h"

int main() {
    char input[256];

    printf("Intérprete C con decimales y variables\n");

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin))
            break;

        if (strncmp(input, "exit", 4) == 0)
            break;

        init_lexer(input);
        AST *tree = parse();
        printf("= %g\n", eval(tree));
    }

    return 0;
}
