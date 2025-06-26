#include "lexer.h"

t_token_type	get_token_type(t_state state)
{
	if (state == pi_pe)
		return (Pipe);
	if (state == redirect_in)
		return (Redirect_In);
	if (state == redirect_out)
		return (Redirect_Out);
	if (state == append)
		return (Append);
	if (state == heredoc)
		return (Here_doc);
	if (state == E_OF)
		return (End_of_file);
	return (Word);
}

t_quote_type	get_quote_type(t_context context)
{
	if (context == Quoted)
		return (Single_quotes);
	if (context == Double_quoted)
		return (Double_quotes);
	return (No_quotes);
}

int	is_operator_state(t_state state)
{
	return (state == pi_pe || state == redirect_in || state == redirect_out
		|| state == append || state == heredoc);
}

int	is_content_state(t_state state)
{
	return (state == literal || state == param_here || state == single_quote
		|| state == double_quote);
}

void	create_operator_token(t_lexer *lexer)
{
	if (lexer->state == heredoc)
	{
		append_token(lexer, create_token("<<", Here_doc, Not_expendable));
		lexer->offset += 2;
		lexer->in_heredoc_delim = 1;
	}
	else if (lexer->state == append)
	{
		append_token(lexer, create_token(">>", Append, Not_expendable));
		lexer->offset += 2;
	}
	else if (lexer->state == redirect_in)
	{
		append_token(lexer, create_token("<", Redirect_In, Not_expendable));
		lexer->offset++;
	}
	else if (lexer->state == redirect_out)
	{
		append_token(lexer, create_token(">", Redirect_Out, Not_expendable));
		lexer->offset++;
	}
	else if (lexer->state == pi_pe)
	{
		append_token(lexer, create_token("|", Pipe, Not_expendable));
		lexer->offset++;
	}
}

static void finalize_current_token(t_lexer *lexer, char **start, t_expendable *expend)
{
    char *chunk;
    
    if (!*start)
        return;
    chunk = ft_substr(*start, 0, lexer->offset - *start);
    if (!chunk)
        return; 
    append_token(lexer, create_token(chunk, Word, *expend));
    free(chunk);
    *start = NULL;
    *expend = Not_expendable;
}

void resolve_tokens(t_lexer *lexer)
{
    t_expendable expend = Not_expendable;
    char *start = NULL;

    while (*(lexer->offset))
    {
        set_context(lexer, *(lexer->offset));
        set_state(lexer);
		if (lexer->state == space && !start)
        {
            lexer->offset++;
            continue;
        }
		if (lexer->state == param_here)
            expend = Expendable;
        if (is_operator_state(lexer->state))
        {
            finalize_current_token(lexer, &start, &expend);
            create_operator_token(lexer);
            continue;
        }
		if (!start)
            start = lexer->offset;
        if (lexer->context == Separator && lexer->state == space)
            finalize_current_token(lexer, &start, &expend);
        lexer->offset++;
    }
    finalize_current_token(lexer, &start, &expend);
    append_token(lexer, create_token("", End_of_file, Not_expendable));
}
