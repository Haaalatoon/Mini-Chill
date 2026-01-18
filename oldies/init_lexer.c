#include "lexer.h"

t_lexer *init_lexer(char *input)
{
    t_lexer *lexer;

    lexer = (t_lexer *)malloc(sizeof(t_lexer));
    if (!lexer)
        return NULL;
    lexer->input = ft_strdup(input);  // Store copy of input
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
    return (lexer);
}

void append_token(t_lexer *lexer, t_token *new_token)
{
    ft_lstadd_back(&lexer->tokens, ft_lstnew(new_token));
}

char *create_token(char *start, size_t len)
{
    char *dest;

    dest = malloc(len + 1);
    if (!dest)
        return (NULL);
    ft_strlcpy(dest, start, len + 1);
    return (dest);
}
