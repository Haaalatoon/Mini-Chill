#include "lexer.h"

t_token *create_token(char *value, t_token_type type, t_expendable expendable)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return NULL;
    if (value)
    {
        token->value = ft_strdup(value);
        if (!token->value)
        {
            free(token);
            return NULL;
        }
    }
    else
        token->value = NULL;
    token->type = type;
    token->expendable = expendable;
    // token->expansion_mask = 0; // Initialize expansion mask to 0
    clear_mask(token->expansion_mask); // Initialize expansion mask to 0
    return (token);
}


void append_token(t_lexer *lexer, t_token *token)
{
    t_list *new_node;

    new_node = ft_lstnew(token);
    if (!new_node)
    {
        free_token(token);
        return;
    }
    ft_lstadd_back(&lexer->tokens, new_node);
}
