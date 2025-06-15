#include "lexer.h"

void set_state(t_lexer *lexer)
{
    char current_char = *lexer->offset;

    if (current_char == '\0')
    {
        lexer->state = In_EOF;
        return;
    }
    if (lexer->state == In_param)
    {
        if (is_valid_param_char(current_char))
            return;
        else
        {
            if (lexer->context == Quoted)
                lexer->state = In_double_quote;
            else
                lexer->state = In_literal;
        }
    }
    if (lexer->context == Quoted)
    {
        if (lexer->state == In_quote)
        {
            if (current_char == '\'')
                return;
            else
                lexer->state = In_quote;
            return;
        }
        if (lexer->state == In_double_quote)
        {
            if (current_char == '"')
                return;
            else if (current_char == '$' && is_valid_param_start(lexer->offset + 1))
            {
                lexer->state = In_param;
                return;
            }
            else
            {
                lexer->state = In_double_quote;
                return; 
            }
        }
    }
    if (is_whitespace(current_char))
        lexer->state = In_space;
    else if (current_char == '|')
        lexer->state = In_pipe;
    else if (current_char == '<')
    {
        if (*(lexer->offset + 1) == '<')
        {
            lexer->state = In_heredoc;
            lexer->offset++;
        }
        else
            lexer->state = In_redirect;
    }
    else if (current_char == '>')
    {
        if (*(lexer->offset + 1) == '>')
        {
            lexer->state = In_append;
            lexer->offset++;
        }
        else
            lexer->state = In_redirect;
    }
    else if (current_char == '\'')
        lexer->state = In_quote;
    else if (current_char == '"')
        lexer->state = In_double_quote;
    else if (current_char == '$' && is_valid_param_start(lexer->offset + 1))
            lexer->state = In_param;
    else
        lexer->state = In_literal;
}

void set_context(t_lexer *lexer, char c)
{
    if (c == '\'' && lexer->state == In_quote && lexer->context != Quoted)
        lexer->context = Quoted;
    else if (c == '\'' && lexer->context == Quoted)
        lexer->context = Unquoted;

    else if (c == '"' && lexer->state == In_double_quote && lexer->context != Quoted)
        lexer->context = Quoted;
    else if (c == '"' && lexer->context == Quoted)
        lexer->context = Unquoted;

    else if (lexer->context != Quoted
             && (is_whitespace(c) || c == '|' || c == '<' || c == '>' || c == '\0'))
        lexer->context = Separator;

    else if (lexer->context == Separator && !is_seperator(c))
        lexer->context = Unquoted;
}

// const char *state_to_str(t_state state)
// {
//     switch (state)
//     {
//         case In_space: return "In_space";
//         case In_literal: return "In_literal";
//         case In_pipe: return "In_pipe";
//         case In_redirect: return "In_redirect";
//         case In_append: return "In_append";
//         case In_heredoc: return "In_heredoc";
//         case In_quote: return "In_quote";
//         case In_double_quote: return "In_double_quote";
//         case In_param: return "In_param";
//         case In_EOF: return "In_EOF";
//         default: return "Unknown";
//     }
// }

// const char *context_to_str(t_context context)
// {
//     switch (context)
//     {
//         case Unquoted: return "Unquoted";
//         case Quoted: return "Quoted";
//         case Separator: return "Separator";
//         default: return "Unknown";
//     }
// }
