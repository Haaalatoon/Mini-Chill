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

t_expendable get_expandable(t_context context)
{
    if (context == Double_quoted || context == Unquoted)
        return Expendable;
    return Not_expendable;
}


void resolve_tokens(t_lexer *lexer)
{
	char *start = lexer->offset;

	while (*(lexer->offset))
	{
		set_context(lexer, *lexer->offset);
		set_state(lexer);

		if (lexer->state == space)
		{
			lexer->offset++;
			continue;
		}

		start = lexer->offset;
        t_context start_context = lexer->context;
        t_state start_state = lexer->state;

        while (*(lexer->offset))
        {
            char c = *(lexer->offset);
            set_context(lexer, c);
            set_state(lexer);

            if (lexer->state == space || lexer->state != start_state)
                break;

            lexer->offset++;
        }

        // Extract and create token
        int len = lexer->offset - start;
        if (len > 0)
        {
            char *value = ft_substr(start, 0, len);
            t_token *token = create_token(
                value,
                get_token_type(start_state),
                get_quote_type(start_context),
                get_expandable(start_context)
            );
            append_token(lexer, token);
            free(value);
        }
    }
}
