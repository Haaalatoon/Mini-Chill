#include "lexer.h"

#include "lexer.h" // or your central header that defines enums

t_token_type get_token_type(t_state state)
{
    if (state == pi_pe)
        return Pipe;
    if (state == redirect_in)
        return Redirect_In;
    if (state == redirect_out)
        return Redirect_Out;
    if (state == append)
        return Append;
    if (state == heredoc)
        return Here_doc;
    if (state == E_OF)
        return End_of_file;
    return Word; // default
}

t_quote_type get_quote_type(t_context context)
{
    if (context == Quoted)
        return Single_quotes;
    if (context == Double_quoted)
        return Double_quotes;
    return No_quotes;
}

t_expendable get_expandable(t_state state)
{
    if (state ==  param_here)
        return Expendable; // param_here is not expendable
    else
         return Not_expendable; // default for other states
}


void resolve_tokens(t_lexer *lexer)
{
    lexer->offset = lexer->input;
    char *token_start = NULL;

    while (*(lexer->offset))
    {
        char c = *(lexer->offset);
        set_context(lexer, c);
        set_state(lexer);

        // Quote handling
        if (lexer->state == single_quote)
        {
            lexer->offset++; // skip opening '
            token_start = lexer->offset;
            while (*(lexer->offset) && *(lexer->offset) != '\'')
                lexer->offset++;
            char *chunk = ft_substr(token_start, 0, lexer->offset - token_start);
            t_token *token = create_token(chunk, Word, Not_expendable);
            append_token(lexer, token);
            free(chunk);
            lexer->offset++; // skip closing '
            continue;
        }
        else if (lexer->state == double_quote)
        {
            lexer->offset++; // skip opening "
            token_start = lexer->offset;
            while (*(lexer->offset) && *(lexer->offset) != '"')
                lexer->offset++;
            char *chunk = ft_substr(token_start, 0, lexer->offset - token_start);
            t_token *token = create_token(chunk, Word, Expendable);
            append_token(lexer, token);
            free(chunk);
            lexer->offset++; // skip closing "
            continue;
        }

        // Space = token boundary
        if (lexer->state == space)
        {
            lexer->offset++;
            continue;
        }

        // Redirection, pipe
        if (lexer->state == pi_pe || lexer->state == redirect_in || lexer->state == redirect_out || lexer->state == append || lexer->state == heredoc)
        {
            t_token_type type = get_token_type(lexer->state);
            char *op = ft_substr(lexer->offset, 0, 1);
            t_token *token = create_token(op, type, Not_expendable);
            append_token(lexer, token);
            free(op);
            lexer->offset++;
            continue;
        }

        // Default literal/param/word
        token_start = lexer->offset;
        while (*(lexer->offset) &&
               !is_seperator(*(lexer->offset)) &&
               !is_quote(*(lexer->offset)))
        {
            set_context(lexer, *(lexer->offset));
            set_state(lexer);
            lexer->offset++;
        }

        if (lexer->offset > token_start)
        {
            char *chunk = ft_substr(token_start, 0, lexer->offset - token_start);
            t_token *token;
            if (get_expandable(lexer->state) == Expendable)
            {
                token = create_token(chunk, Word, Expendable);
            }
            else
            {
                token = create_token(chunk, Word, Not_expendable);
            }
            append_token(lexer, token);
            free(chunk);
        }
    }
}