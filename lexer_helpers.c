#include "lexer.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_redirect(char c)
{
	return (c == '<' || c == '>');
}

int	is_seperator(char c)
{
	return (is_whitespace(c) || is_redirect(c) || c == '|' || c == '\0');
}

int	is_valid_param_start(char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '?')
		return (1);
	return (ft_isalpha(*str) || *str == '_');
}

int	is_valid_param_char(char c)
{
	if (c == '?')
		return (1);
	return (ft_isalnum(c) || c == '_');
}

int	contains_parameter(char *value)
{
	char	*dollar_sign;

	dollar_sign = ft_strchr(value, '$');
	while (dollar_sign)
	{
		if (is_valid_param_start(dollar_sign + 1))
			return (1);
		dollar_sign = ft_strchr(dollar_sign + 1, '$');
	}
	return (0);
}

void	handle_redirect_in(t_lexer *lexer)
{
	if (*(lexer->offset + 1) == '<')
		lexer->state = heredoc;
	else
		lexer->state = redirect_in;
}

void	handle_redirect_out(t_lexer *lexer)
{
	if (*(lexer->offset + 1) == '>')
		lexer->state = append;
	else
		lexer->state = redirect_out;
}

void	skip_space_sep(t_lexer *lexer)
{
	if (lexer->state == space)
		lexer->offset++;
}