#ifndef LEXER_H
# define LEXER_H

# include "libft/libft.h"
# include <stdio.h>

// # define MSK_ARR_SZ 100

# include <stdbool.h>

// To change later
typedef struct s_minishell
{
	char	**environ;
	char	*line;
	int		exit_s;
	bool	signint_child;
	bool	heredoc_sigint;
}	t_minishell;

extern t_minishell g_minishell;

// Token types
typedef enum e_token_type
{
	Word,
	Pipe,
	Redirect_In,
	Redirect_Out,
	Append,
	Here_doc,
	End_of_file,
}					t_token_type;

// Quote types
typedef enum e_quote_type
{
	No_quotes,
	Single_quotes,
	Double_quotes
}					t_quote_type;

// Expendable status
typedef enum e_expendable
{
	Not_expendable,
	Expendable
}					t_expendable;

// Lexer context
typedef enum e_context
{
	Unquoted,
	Quoted,
	Double_quoted,
	Separator,
}					t_context;

// Lexer states
typedef enum e_state
{
	space,
	literal,
	pi_pe,
	heredoc,
	redirect_in,
	redirect_out,
	append,
	param_here,
	single_quote,
	double_quote,
	E_OF
}					t_state;

// Token structure
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_expendable	expendable;
	// unsigned long	expansion_mask[MSK_ARR_SZ];
}					t_token;

typedef struct s_lexer
{
	char			*offset;
	char			*input;
	t_list			*tokens;
	t_state			state;
	t_context		context;
	int				in_heredoc_delim;
}					t_lexer;

// Lexer helpers
int					is_whitespace(char c);
int					is_quote(char c);
int					is_redirect(char c);
int					is_seperator(char c);
int					is_valid_param_start(char *str);
int					is_valid_param_char(char c);
int					contains_parameter(char *value);
void				handle_redirect_in(t_lexer *lexer);
void				handle_redirect_out(t_lexer *lexer);
void				skip_spaces(t_lexer *lexer);

// Lexer initialization and cleanup
t_lexer				*init_lexer(const char *input);
void				free_lexer(t_lexer *lexer);

// Token management
t_token				*create_token(char *value, t_token_type type,
						t_expendable expendable);
void				free_token(void *ptr);
void				append_token(t_lexer *lexer, t_token *token);

// Lexer state management
t_lexer				*set_state(t_lexer *lexer);
void				set_context(t_lexer *lexer, char c);

// Resolving tokens
void				resolve_tokens(t_lexer *lexer);
void				skip_space_sep(t_lexer *lexer);

// DEBUGGING
const char			*state_to_str(t_state state);
const char			*context_to_str(t_context context);
t_token_type		get_token_type(t_state state);
t_quote_type		get_quote_type(t_context context);

// Functions from signals.c
void	ft_init_signals(void);
void	ft_sigint_handler(int sig);
void	ft_reset_signals_in_child(void);

#endif