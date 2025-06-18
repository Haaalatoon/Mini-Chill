#include "lexer.h"

void set_state(t_lexer *lexer)
{
	char current_char = *(lexer->offset);

	if (current_char == '\0')
		return((void)(lexer->state = E_OF));
	if (current_char == '\'' && lexer->context != Double_quoted)
	{
		lexer->state = single_quote;
		return;
	}
	if (current_char == '"' && lexer->context != Quoted)
	{
		lexer->state = double_quote;
		return;
	}
	if (current_char == '$' && lexer->context != Quoted && lexer->context != Heredoc && is_valid_param_start(lexer->offset + 1))
		return((void)(lexer->state = param_here));
	if (is_whitespace(current_char) && lexer->context != Quoted && lexer->context != Double_quoted && lexer->context != Heredoc)
		return((void)(lexer->state = space));
	if (current_char == '|')
		return((void)(lexer->state = pi_pe));
	if (current_char == '<')
	{
		if (*(lexer->offset + 1) == '<')
		{
			lexer->state = heredoc;
			lexer->context = Heredoc;
			if (*(lexer->offset + 2) == ' ')
				lexer->offset += 2;
			else
				lexer->offset++;
		}
		else
			lexer->state = redirect_in;
		return;
	}
	if (current_char == '>')
	{
		if (*(lexer->offset + 1) == '>')
		{
			lexer->state = append;
			lexer->offset++;
		}
		else
			lexer->state = redirect_out;
		return;
	}
	lexer->state = literal;
}		

void set_context(t_lexer *lexer, char c)
{
	if (lexer->context == Heredoc)
	{
		if(is_seperator(c))
			lexer->context = Separator;
		else
		 return;
	}
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
		case Heredoc: return "Heredoc";
		default: return "Unknown";
	}
}

