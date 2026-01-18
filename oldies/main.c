#include "lexer.h"
#include <stdio.h>

// int main(int argc, char *argv[]) {

//     const char delimiters[] = ", |";
//     char *saveptr = NULL; // Pointer to track the state

//     // Get the first token
//     char *token = next_token(argv[0], delimiters, &saveptr);

//     // Walk through other tokens
//     while (token != NULL) {
//         printf("Token: %s\n", token);
//         token = next_token(NULL, delimiters, &saveptr);
//     }

//     return 0;
// }

#include "lexer.h"
#include <stdio.h>

void print_tokens(t_list *tokens)
{
    t_token *tk;

    while (tokens)
    {
        tk = (t_token *)tokens->content;
        printf("Token: %-10s | Type: %-10d | Quote: %-2d | Expand: %-2d\n",
            tk->value, tk->type, tk->quote, tk->expendable);
        tokens = tokens->next;
    }
}

int main(void)
{
    char *input = "echo \"hello $USER\" | grep hi >> out.txt";
    t_lexer *lexer = init_lexer(input);

    lexer_run(lexer);
    print_tokens(lexer->tokens);

    return (0);
}
