// #include "minishell.h"

// int is_whitespace(char c)
// {
//     return (c == ' ' || c == '\t' || c == '\n');
// }

// int is_special_char(char c)
// {
//     return (c == '|' || c == '<' || c == '>' || c == '$');
// }

// t_token *create_token(t_token_type type, char *value)
// {
//     t_token *token;

//     token = (t_token *)malloc(sizeof(t_token));
//     if (!token)
//         return (NULL);
//     token->type = type;
//     token->value = ft_strdup(value);
//     if (!token->value)
//     {
//         free(token);
//         return (NULL);
//     }
//     token->next = NULL;
//     return (token);
// }

// void add_token(t_token **tokens, t_token *new_token)
// {
//     t_token *current;

//     if (!*tokens)
//         *tokens = new_token;
//     else
//     {
//         current = *tokens;
//         while (current->next)
//             current = current->next;
//         current->next = new_token;
//     }
// }

// // New helper function - efficient way to create and add a token in one step
// int add_new_token(t_token **tokens, t_token_type type, char *value)
// {
//     t_token *new_token = create_token(type, value);
//     if (!new_token)
//         return (0);
//     add_token(tokens, new_token);
//     return (1);
// }

// void	free_tokens(t_token *tokens)
// {
// 	t_token	*tmp;

// 	while (tokens)
// 	{
// 		tmp = tokens;
// 		tokens = tokens->next;
// 		free(tmp->value);
// 		free(tmp);
// 	}
// }

// int handle_dollar(char *input, int index, t_token **tokens)
// {
// 	int start;
// 	char *value;

// 	if (input[index + 1] == '?')
// 	{
// 		add_token(tokens, create_token(TOKEN_EXIT_STATUS, "$?"));
// 		return (index + 1);
// 	}
// 	else if (input[index + 1] && (ft_isalpha(input[index + 1]) || input[index + 1] == '_'))
// 	{
// 		start = index + 1;
// 		index++;
// 		while (input[index] && (ft_isalnum(input[index]) || input[index] == '_'))
// 			index++;
// 		value = ft_substr(input, start, index - start);
// 		if (!value)
// 			return (-1);
// 		add_token(tokens, create_token(TOKEN_ENV_VAR, value));
// 		free(value);
// 		return (index - 1);
// 	}
// 	else
// 	{
// 		add_token(tokens, create_token(TOKEN_WORD, "$"));
// 		return (index);
// 	}
// }

// int	handle_quotes(char *input, int index, t_token **tokens)
// {
// 	int		start;
// 	char	quote_type;
//     char	*value;
//     t_token_type	type;

//     quote_type = input[index];
//     if (quote_type == '\'')
//         type = TOKEN_SGL_QUOTED_STRING;
//     else
//         type = TOKEN_DBL_QUOTED_STRING;
//     start = index + 1;
// 	index++;
// 	while (input[index] && input[index] != quote_type)
// 		index++;
// 	if (!input[index])
// 		return (-1);
// 	value = ft_substr(input, start, index - start);
//     if (!value)
//         return (-1);
//     add_token(tokens, create_token(type, value));
//     free(value);
// 	return (index);
// }

// int	extract_word(char *input, int index, t_token **tokens)
// {
// 	int		start;
// 	char *value;

// 	start = index;
// 	while (input[index] && !is_whitespace(input[index])
// 		&& !is_special_char(input[index]))
//         index++;
// 	value = ft_substr(input, start, index - start);
//     if (!value)
//         return (-1);
//     add_token(tokens, create_token(TOKEN_WORD, value));
//     free(value);
// 	return (index - 1);
// }

// int	handle_redirection(char *input, int index, t_token **tokens)
// {
// 	t_token_type	type;
// 	char			*value;

// 	if (input[index] == '<')
// 	{
// 		if (input[index + 1] && input[index + 1] == '<')
// 		{
// 			type = TOKEN_HEREDOC;
// 			value = ft_strdup("<<");
// 			index++;
// 		}
// 		else
// 		{
// 			type = TOKEN_REDIR_IN;
// 			value = ft_strdup("<");
// 		}
// 	}
// 	else
// 	{
// 		if (input[index + 1] && input[index + 1] == '>')
// 		{
// 			type = TOKEN_APPEND;
// 			value = ft_strdup(">>");
// 			index++;
// 		}
// 		else
// 		{
// 			type = TOKEN_REDIR_OUT;
// 			value = ft_strdup(">");
// 		}
// 	}
// 	add_token(tokens, create_token(type, value));
// 	return (free(value), index);
// }

// t_token	*tokenize(char *input)
// {
// 	t_token	*tokens;
// 	int		i;

// 	i = 0;
// 	tokens = NULL;
// 	while (input[i])
// 	{
// 		if (is_whitespace(input[i]))
// 		{
//             i++;
//             continue;
//         }
// 		else if (input[i] == '|')
// 			add_token(&tokens, create_token(TOKEN_PIPE, "|"));
// 		else if (input[i] == '<' || input[i] == '>')
// 			i = handle_redirection(input, i, &tokens);
// 		else if (input[i] == '\'' || input[i] == '\"')
// 		{
// 			i = handle_quotes(input, i, &tokens);
// 			if (i == -1)
// 				return (free_tokens(tokens), NULL);
// 		}
// 		else if (input[i] == '$')
// 		{
// 			i = handle_dollar(input, i, &tokens);
// 			if (i == -1)
// 				return (free_tokens(tokens), NULL);
// 		}
// 		else
// 		{
// 			i = extract_word(input, i, &tokens);
// 			if (i == -1)
// 				return (free_tokens(tokens), NULL);
// 		}
//         i++;
// 	}
//     return (tokens);
// }

// void print_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     const char *type_names[] = {
//         "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC", 
//         "SGL_QUOTED_STRING", "DBL_QUOTED_STRING", "ENV_VAR", "EXIT_STATUS", "EOF"
//         // Make sure this matches your enum order
//     };

//     printf("Tokens:\n");
//     while (current)
//     {
//         printf("Type: %s, Value: '%s'\n", type_names[current->type], current->value);
//         current = current->next;
//     }
//     printf("\n");
// }

// int main(void)
// {
//     char *input;
//     t_token *tokens;

//     printf("Minishell Tokenizer Tester\n");
//     printf("Enter commands to see how they're tokenized. Press Ctrl+D to exit.\n");

//     while (1)
//     {
//         // Display prompt and read input
//         input = readline("minishell> ");

//         // Exit on EOF (Ctrl+D)
//         if (!input)
//         {
//             printf("\n");
//             break;
//         }

//         // Tokenize the input
//         tokens = tokenize(input);

//         // Print the tokens
//         if (tokens)
//         {
//             print_tokens(tokens);
//             free_tokens(tokens);
//         }
//         else
//         {
//             printf("Error in tokenization\n");
//         }

//         // Free the input and add to history
//         add_history(input);
//         free(input);
//     }

//     return (0);
// }
