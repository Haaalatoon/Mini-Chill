#include "lexer.h"

t_lexer *init_lexer(const char *input)
{
    t_lexer *lexer;

    lexer = (t_lexer *)malloc(sizeof(t_lexer));
    if (!lexer)
        return NULL;

    lexer->input = ft_strdup(input);
    if (!lexer->input)
    {
        free(lexer);
        return NULL;
    }
    lexer->offset = lexer->input;
    lexer->state = space;
    lexer->context = Unquoted;
    lexer->tokens = NULL;
    lexer->in_heredoc_delim = 0;

    return lexer;
}

void free_token(void *ptr)
{
    t_token *token = (t_token *)ptr;
    if (!token)
        return;
    if (token->value)
        free(token->value);
    free(token);
}

void free_lexer(t_lexer *lexer)
{
    if (!lexer)
        return;

    if (lexer->input)
        free(lexer->input);

    if (lexer->tokens)
    ft_lstclear(&lexer->tokens, free_token);

    free(lexer);
}
