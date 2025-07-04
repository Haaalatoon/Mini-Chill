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
	if (state == Delim)
		return (Delim);
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
		lexer->offset++;
	}
	else if (lexer->state == append)
	{
		append_token(lexer, create_token(">>", Append, Not_expendable));
		lexer->offset++;
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

char	*add_char_to_string(char *str, char c)
{
	int		len;
	char	*new_str;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (str);
	if (str)
	{
		len = ft_strlen(str);
		ft_strlcpy(new_str, str, len + 1);
		new_str[len] = c;
		new_str[len + 1] = '\0';
	}
	else
	{
		new_str[0] = c;
		new_str[1] = '\0';
	}
	return (new_str);
}

int	is_mask_bit_set(unsigned long mask[], size_t pos)
{
	size_t	array_index;
	size_t	bit_index;

	array_index = pos / 64;
	bit_index = pos % 64;
	if (array_index >= MSK_ARR_SZ)
		return (0);
	return ((mask[array_index] & (1UL << bit_index)) != 0);
}

void	set_mask_bit(unsigned long mask[], size_t pos)
{
	size_t	array_index;
	size_t	bit_index;

	array_index = pos / 64;
	bit_index = pos % 64;
	if (array_index < MSK_ARR_SZ)
		mask[array_index] |= (1UL << bit_index);
}

void	clear_mask(unsigned long mask[])
{
	ft_memset(mask, 0, sizeof(unsigned long) * MSK_ARR_SZ);
}

void	copy_mask(unsigned long dest[], unsigned long src[])
{
	ft_memcpy(dest, src, sizeof(unsigned long) * MSK_ARR_SZ);
}

// static void	finalize_word_token(t_lexer *lexer, char **token_content,
// 		t_expendable *expend, unsigned long *current_mask)
static void	finalize_word_token(t_lexer *lexer, char **token_content,
		t_expendable *expend, unsigned long curr_mask[])
{
	t_token	*token;

	if (*token_content && **token_content)
	{
		if (lexer->in_heredoc_delim)
			*expend = Not_expendable;
		token = create_token(*token_content, Word, *expend);
		// token->expansion_mask = *current_mask;
		copy_mask(token->expansion_mask, curr_mask);
		append_token(lexer, token);
		free(*token_content);
		*token_content = NULL;
		*expend = Not_expendable;
		// *current_mask = 0;
		clear_mask(curr_mask);
	}
}

// static int	handle_content_states(t_lexer *lexer, char **token_content,
// 		t_expendable *expend, unsigned long *current_mask)
static int	handle_content_states(t_lexer *lexer, char **token_content,
		t_expendable *expend, unsigned long curr_mask[])
{
	int		pos;
	char	*new_content;

	pos = 0;
	if (is_content_state(lexer->state))
	{
		if (!*token_content)
			*token_content = ft_strdup("");
		if (lexer->state == single_quote || lexer->state == double_quote)
			return (lexer->offset++, 1);
		if (lexer->state == literal || lexer->state == param_here)
		{
			if (lexer->state == param_here)
			{
				if (*token_content)
					pos = ft_strlen(*token_content);
				if (pos < 64)
					// *current_mask |= (1UL << pos);
					set_mask_bit(curr_mask, pos);
				*expend = Expendable;
			}
			new_content = add_char_to_string(*token_content, *(lexer->offset));
			free(*token_content);
			*token_content = new_content;
		}
	}
	return (0);
}

// static int	whitespace_separation(t_lexer *lexer, char **token_content,
// 		t_expendable *expend, unsigned long *current_mask)
static int	whitespace_separation(t_lexer *lexer, char **token_content,
		t_expendable *expend, unsigned long curr_mask[])
{
	if (lexer->state == space && lexer->context == Separator)
	{
		finalize_word_token(lexer, token_content, expend, curr_mask);
		lexer->offset++;
		return (1);
	}
	return (0);
}
void	resolve_tokens(t_lexer *lexer)
{
	char			*token_content;
	t_expendable	expend;
	unsigned long	curr_mask[MSK_ARR_SZ];

	// unsigned long	curr_mask;
	token_content = NULL;
	// curr_mask = 0;
	clear_mask(curr_mask);
	while (*(lexer->offset))
	{
		set_context(lexer, *(lexer->offset));
		set_state(lexer);
		if (whitespace_separation(lexer, &token_content, &expend, curr_mask))
			continue ;
		if (is_operator_state(lexer->state))
		{
			finalize_word_token(lexer, &token_content, &expend, curr_mask);
			create_operator_token(lexer);
			continue ;
		}
		if (handle_content_states(lexer, &token_content, &expend, curr_mask))
			continue ;
		lexer->offset++;
	}
	if (token_content && *token_content)
		finalize_word_token(lexer, &token_content, &expend, curr_mask);
	append_token(lexer, create_token("", End_of_file, Not_expendable));
}
