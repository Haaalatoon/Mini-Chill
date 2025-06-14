#include "lexer.h"

void set_state(t_lexer *lexer, char current_char)
{
    // If we're inside quotes, state is determined by quote type
    if (lexer->context == Quoted)
    {
        if (lexer->state == In_quote)
        {
            if (current_char == '\'')
                lexer->state = In_literal;
            else
                lexer->state = In_quote; // Continue in quote state
            return;
        }
        if (lexer->state == In_double_quote)
        {
            if (current_char == '"')
                lexer->state = In_literal;
            else if (current_char == '$' && is_valid_param_start(lexer->offset + 1))
                lexer->state = In_param;
            else
                lexer->state = In_double_quote;
            return;
        }
    }
    if (is_whitespace(current_char))
        lexer->state = In_space;
    else if (current_char == '|')
        lexer->state = In_pipe;
    else if (current_char == '<')
    {
        if (*(lexer->offset + 1) == '<')
            lexer->state = In_heredoc;
        else
            lexer->state = In_redirect;
    }
    else if (current_char == '>')
    {
        if (*(lexer->offset + 1) == '>')
            lexer->state = In_append;
        else
            lexer->state = In_redirect;
    }
    else if (current_char == '\'')
        lexer->state = In_quote;
    else if (current_char == '"')
        lexer->state = In_double_quote;
    else if (current_char == '$')
    {
        if (is_valid_param_start(lexer->offset + 1))
            lexer->state = In_param;
        else
            lexer->state = In_literal;
    }
    else if (current_char == '\0')
    {
        lexer->state = In_EOF;
    }
    else
        lexer->state = In_literal;
}


void set_context(t_lexer *lexer, char current_char)
{
    // if (current_char == '\'')
    // {
    //     if (lexer->context == Unquoted)
    //         lexer->context = Quoted;
    //     else if (lexer->context == Quoted && lexer->state == In_quote)
    //         lexer->context = Unquoted;
    // }
    // else if (current_char == '"')
    // {
    //     if (lexer->context == Unquoted)
    //         lexer->context = Quoted;
    //     else if (lexer->context == Quoted && lexer->state == In_double_quote)
    //         lexer->context = Unquoted;
    // }
    // Handle transitions for single quotes
    if (current_char == '\'' && lexer->state != In_double_quote)
    {
        if (lexer->context == Unquoted)
        {
            lexer->context = Quoted;
        }
        else if (lexer->context == Quoted && lexer->state == In_quote)
        {
            lexer->context = Unquoted;
        }
    }
    // Handle transitions for double quotes
    else if (current_char == '"' && lexer->state != In_quote)
    {
        if (lexer->context == Unquoted)
        {
            lexer->context = Quoted;
        }
        else if (lexer->context == Quoted && lexer->state == In_double_quote)
        {
            lexer->context = Unquoted;
        }
    }
    else if (is_whitespace(current_char) || current_char == '|' || current_char == '<' || current_char == '>' || current_char == '\0')
    {
        if (lexer->context != Quoted)
            lexer->context = Separator;
    }
    else
    {
        if (lexer->context == Separator)
            lexer->context = Unquoted;
    }
}

int main(int argc, char **argv)
{

    t_lexer *lexer = init_lexer(argv[1]);
    if (!lexer)
    {
        printf("Failed to initialize lexer\n");
        return 1;
    }

    char *input = lexer->input;
    while (*input)
    {
        set_state(lexer, *input);
        set_context(lexer, *input);
        printf("Current char: '%c', State: %d, Context: %d\n", *input, lexer->state, lexer->context);
        input++;
    }

    free_lexer(lexer);
    return 0;
}