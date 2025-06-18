#include "lexer.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *input = NULL;
    t_lexer *lexer = NULL;

    while (1)
    {    input = readline("minishell> ");
    if (!input)  // EOF (Ctrl‑D)
        break;

    // Optionally add non-empty lines to history
    if (*input)
        add_history(input);

    // Initialize lexer on the user input
    lexer = init_lexer(input);
    if (!lexer)
    {
        fprintf(stderr, "Lexer init failed\n");
        free(input);
        return 1;
    }

    printf("Input: %s\n\n", input);

    // Walk through each character
    while (*(lexer->offset))
    {
        char c = *(lexer->offset);
        set_context(lexer, c);
        set_state(lexer);
        printf("Char: '%c' (%d) | State: %-20s | Context: %-10s\n",
                c, c, state_to_str(lexer->state), context_to_str(lexer->context));

        lexer->offset++;
    }

    // Finally handle EOF marker
    set_context(lexer, '\0');
    set_state(lexer);
    printf("Char: '\\0'        | State: %-20s | Context: %-10s\n",
            state_to_str(lexer->state), context_to_str(lexer->context));
    if (lexer->context == Quoted)
    {
        printf("Warning: Unmatched quote detected!\n");
        // Think of a way to handle this 
    }

    free_lexer(lexer);
    free(input);
    }
    return 0;
}
