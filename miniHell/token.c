#include "minishell.h"

int add_token(t_token **head, t_token_type type, const char *value, int expand)
{
    t_token *new;
    t_token *current;

    if (!head || !value)
        return (1);
    new = malloc(sizeof(t_token));
    if (!new)
        return (1);
    new->type = type;
    new->expandable = expand;
    new->next = NULL;
    new->value = ft_strdup(value);
    if (!new->value)
    {
        free(new);
        return (1);
    }
    if (!*head)
        *head = new;
    else
    {
        current = *head;
        while (current->next)
            current = current->next;
        current->next = new;
    }
    return (0);
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next;
        if (tokens->value)
            free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}
// For debugging purposes, this function prints the tokens in a readable format
void print_tokens(t_token *tokens)
{
    static const char *types[] = {
        "WORD", "VAR", "PIPE", "REDIR_IN",
        "REDIR_OUT", "APPEND", "HEREDOC", "NEWLINE", "EOF"
    };
    t_token *cur;
    
    if (!tokens)
    {
        printf("No tokens found.\n");
        return;
    }
    
    printf("Tokens:\n");
    for (cur = tokens; cur; cur = cur->next)
    {
        printf("Type: %-9s  Value: '%s'  Expand: %s\n",
            types[cur->type],
            cur->value ? cur->value : "(null)",
            cur->expandable ? "Yes" : "No");
    }
    printf("\n");
}