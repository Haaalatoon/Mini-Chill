#include "minishell.h"

int main(void)
{
    char *line;
    t_token *tokens;

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("\n");
            break;
        }
        if (*line)
        {
            tokens = tokenize(line);
            if (tokens)
            {
                print_tokens(tokens);
                free_tokens(tokens);
            }
        }
        add_history(line);
        free(line);
    }
    rl_clear_history();
    return (0);
}