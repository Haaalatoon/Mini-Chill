#include "lexer.h"

t_lexer *set_state(t_lexer *lexer)
{
	char current_char;

	current_char = *(lexer->offset);
	if (current_char == '\0')
		lexer->state = E_OF;
	else if (lexer->state == heredoc)
	{
		if (lexer->context == Separator)
		{
			if (is_whitespace(current_char))
				lexer->state = space;
			else if (current_char == '|')
				lexer->state = pi_pe;
			else if (current_char == '<')
				handle_redirect_in(lexer);
			else if (current_char == '>')
				handle_redirect_out(lexer);
		}
	}
	else if (current_char == '\'' && lexer->context != Double_quoted)
		lexer->state = single_quote;
	else if (current_char == '"' && lexer->context != Quoted)
		lexer->state = double_quote;
	else if (current_char == '$' && lexer->context != Quoted && is_valid_param_start(lexer->offset + 1))
		lexer->state = param_here;
	else if (lexer->context != Quoted && lexer->context != Double_quoted)
	{
		if (is_whitespace(current_char))
			lexer->state = space;
		else if (current_char == '<')
			handle_redirect_in(lexer);
		else if (current_char == '>')
			handle_redirect_out(lexer);
		else if (current_char == '|')
			lexer->state = pi_pe;
		else
			lexer->state = literal;
	}
	else 
		lexer->state = literal;
	return (lexer);
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
	else if (lexer->context == Separator)
	{
		if (c == '\'')
			lexer->context = Quoted;
		else if (c == '"')
			lexer->context = Double_quoted;
		else if (!is_seperator(c))
			lexer->context = Unquoted;
	}
	else if (lexer->context == Unquoted && is_seperator(c))
		lexer->context = Separator;
}

const char *state_to_str(t_state state)
{

	switch (state)
	{
		case space: return "space";
		case literal: return "literal";
		case pi_pe: return "In_pipe";
		case redirect_in: return "redirect_in";
		case redirect_out: return "In_redirect_out";
		case append: return "append";
		case param_here: return "param_here";
		case single_quote: return "single_quote";
		case double_quote: return "double_quote";
		case heredoc: return "heredoc";
		case E_OF: return "E_OF";
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

