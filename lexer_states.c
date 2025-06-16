#include "lexer.h"

void set_state(t_lexer *lexer)
{
    char current_char = *(lexer->offset);

    if (current_char == '\0')
        return((void)(lexer->state = In_EOF));
    if (lexer->state == In_param)
    {
        if (is_valid_param_char(current_char))
            return;
        else
            return((void)(lexer->state = In_literal));
    }
    if (current_char == '$' && lexer->context != Quoted && is_valid_param_start(lexer->offset + 1))
        return((void)(lexer->state = In_param));
    if (is_whitespace(current_char))
        return((void)(lexer->state = In_space));
    if (current_char == '|')
        return((void)(lexer->state = In_pipe));
    if (current_char == '<')
    {
        if (*(lexer->offset + 1) == '<')
        {
            lexer->state = In_heredoc;
            lexer->offset++;
        }
        else
            lexer->state = In_redirect;
        return;
    }
    if (current_char == '>')
    {
        if (*(lexer->offset + 1) == '>')
        {
            lexer->state = In_append;
            lexer->offset++;
        }
        else
            lexer->state = In_redirect;
        return;
    }
    lexer->state = In_literal;
    
}

void set_context(t_lexer *lexer, char c)
{
    if (c == '\'' && lexer->context == Unquoted)
        lexer->context = Quoted;
    else if (c == '\'' && lexer->context == Quoted)
        lexer->context = Unquoted;
    else if (c == '"' && lexer->context == Unquoted)
        lexer->context = Double_quoted;
    else if (c == '"' && lexer->context == Double_quoted)
        lexer->context = Unquoted;

    else if (lexer->context != Quoted && lexer->context != Double_quoted
        && (is_seperator(c)))
        lexer->context = Separator;

    else if (lexer->context == Separator)
    {
        if (c == '\'')
            lexer->context = Quoted;
        else if (c == '"')
            lexer->context = Double_quoted;
        else
            lexer->context = Unquoted;
    }
}


const char *state_to_str(t_state state)
{
    switch (state)
    {
        case In_space: return "In_space";
        case In_literal: return "In_literal";
        case In_pipe: return "In_pipe";
        case In_redirect: return "In_redirect";
        case In_append: return "In_append";
        case In_heredoc: return "In_heredoc";
        case In_param: return "In_param";
        case In_EOF: return "In_EOF";
        default: return "Unknown";
    }
}

const char *context_to_str(t_context context)
{
    switch (context)
    {
        case Unquoted: return "Unquoted";
        case Quoted: return "Quoted";
        case Double_quoted: return "Double_quoted";
        case Separator: return "Separator";
        default: return "Unknown";
    }
}

