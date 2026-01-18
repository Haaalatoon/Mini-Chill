#include "lexer.h"

void lexer_run(t_lexer *lexer)
{
    while (*(lexer->offset) && lexer->state != In_EOF)
    {
        if (lexer->state == In_space)
                handle_in_space(lexer);
        else if (lexer->state == In_literal)
                handle_in_literal(lexer);
        else if (lexer->state == In_pipe)
                handle_in_pipe(lexer);
        else if (lexer->state == In_redirect)
                handle_in_redirect(lexer);
        else if (lexer->state == In_append)
                handle_in_append(lexer);
        else if (lexer->state == In_heredoc)
                handle_in_heredoc(lexer);
        else if (lexer->state == In_quote)
                handle_in_quote(lexer);
        else if (lexer->state == In_double_quote)
                handle_in_double_quote(lexer);
        else if (lexer->state == In_param)
                handle_in_param(lexer);
        else
            lexer->state = In_EOF;
    }
}

void handle_in_space(t_lexer *lexer)
{
    char c;

    c = *(lexer->offset);
    if (c == ' ' || c == '\t' || c == '\n')
    {
        lexer->offset++;
        return;
    }
    else if (c == '|')
    {
        append_token(lexer, create_token("|", Pipe, No_quotes, Not_expendable));
        lexer->offset++;
        return;
    }
    if (c == '<')
    {
        if (*(lexer->offset + 1) == '<')
        {
            append_token(lexer, create_token_ncpy(lexer->offset, 2, Heredoc, lexer));

            lexer->offset += 2;
        }
        else
        {
            append_token(lexer, create_token_ncpy(lexer->offset, 1, Redirect_Out, lexer));
            lexer->offset++;
        }
        return;
    }
    if (c == '>')
    {
        if (*(lexer->offset + 1) == '>')
        {
            append_token(lexer, create_token_ncpy(lexer->offset, 2, Append, lexer));
            lexer->offset += 2;
        }
        else
        {
            append_token(lexer, create_token(">", Redirect_Out, No_quotes, Not_expendable));
            append_token(lexer, create_token_ncpy(lexer->offset, 1, Heredoc, lexer));

            lexer->offset++;
        }
        return;
    }
    if (c == '\'')
    {
        lexer->state = In_quote;
        lexer->context = S_Quoted;
        lexer->offset++;
        return;
    }
    if (c == '\"')
    {
        lexer->state = In_double_quote;
        lexer->context = D_Quoted;
        lexer->offset++;
        return;
    }
    if (c == '$')
    {
        lexer->state = In_param;
        // lexer->offset++;
        return;
    }
    lexer->state = In_literal;
}

void handle_in_literal(t_lexer *lexer)
{
    char *start = lexer->offset;
    char *end;

    while (*(lexer->offset) && *(lexer->offset) != ' ' && *(lexer->offset) != '\t' &&
           *(lexer->offset) != '\n' && *(lexer->offset) != '|' && *(lexer->offset) != '<' &&
           *(lexer->offset) != '>' && *(lexer->offset) != '\'' && *(lexer->offset) != '\"' &&
           *(lexer->offset) != '$')
        lexer->offset++;
    end = lexer->offset;

    append_token(lexer, create_token_ncpy(start, end - start, Word, lexer));
    lexer->state = In_space;
}

void handle_in_quote(t_lexer *lexer)
{
    char *start = lexer->offset + 1; // Skip the opening quote
    char *end;

    while (*(lexer->offset) && *(lexer->offset) != '\'')
        lexer->offset++;
    if (*(lexer->offset) == '\'') // If we found the closing quote
    {
        end = lexer->offset;
        lexer->offset++; // Move past the closing quote
        append_token(lexer, create_token_ncpy(start, end - start, Word, lexer));
        lexer->state = In_space;
        lexer->context = Unquoted; // Reset context after closing quote
    }
}

void handle_in_double_quote(t_lexer *lexer)
{
    char *start = lexer->offset + 1; // Skip the opening quote
    char *end;

    while (*(lexer->offset) && *(lexer->offset) != '\"')
        lexer->offset++;
    if (*(lexer->offset) == '\"') // If we found the closing quote
    {
        end = lexer->offset;
        lexer->offset++; // Move past the closing quote
        append_token(lexer, create_token_ncpy(start, end - start, Word, lexer));
        lexer->state = In_space;
        lexer->context = Unquoted; // Reset context after closing quote
    }
}

void handle_in_param(t_lexer *lexer)
{
    char *start = lexer->offset;
    char *end;

    lexer->offset++; // Move past the '$'
    while (is_valid_param_char(*(lexer->offset)))
        lexer->offset++;
    end = lexer->offset;
    append_token(lexer, create_token_ncpy(start - 1, end - start + 1, Word, lexer));
    lexer->state = In_space;
}

