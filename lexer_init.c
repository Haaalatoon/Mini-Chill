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
    lexer->state = In_space;
    lexer->context = Unquoted;
    lexer->tokens = NULL;

    return lexer;
}

void free_lexer(t_lexer *lexer)
{
    if (!lexer)
        return;

    if (lexer->input)
        free(lexer->input);

    if (lexer->tokens)
        ft_lstclear(&lexer->tokens, (void (*)(void *))free); //To double check PLEASE

    free(lexer);
}
