#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_VAR,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_NEWLINE,
    TOKEN_EOF
}   t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type     type;
    int              expandable;
    struct s_token  *next;
}   t_token;

typedef enum e_state
{
    STATE_START,
    STATE_WORD,
    STATE_SINGLE_QUOTE,
    STATE_DOUBLE_QUOTE,
    STATE_VARIABLE
} t_state;

// /* Tokenizer context structure */
// typedef struct s_tokenizer
// {
//     const char  *input;     /* Input string being tokenized */
//     int         index;      /* Current position in input */
//     t_state     state;      /* Current state of the tokenizer */
//     char        buffer[BUFF_SIZE]; /* Buffer for building token values */
//     int         buf_len;    /* Current length of data in buffer */
//     int         var_start;  /* Start index of variable name */
//     t_token     *tokens;    /* List of tokens being built */
// } t_tokenizer;
typedef struct s_tokenizer
{
    const char  *input;
    int         index;
    t_state     state;
    char        buffer[BUFF_SIZE];
    int         buf_len;
    int         var_start;
    t_token     *tokens;
    int         error;        // Add error flag
    char        *error_msg;   // Add error message
} t_tokenizer;

// Command structure
typedef struct s_command
{
    char            **args;    // Command and its arguments
    char            *input;    // Input redirection file
    char            *output;   // Output redirection file
    int             append;    // 1 if output is in append mode
    char            *heredoc;  // Heredoc delimiter
    struct s_command *next;    // Next command in pipeline
} t_command;

t_token *tokenize(const char *input);
void free_tokens(t_token *tokens);
void print_tokens(t_token *tokens);
int add_token(t_token **head, t_token_type type, const char *value, int expand);
void free_tokens(t_token *tokens);
void print_tokens(t_token *tokens);
void init_tokenizer(t_tokenizer *t, const char *input);
int is_end_of_input(t_tokenizer *t);
t_token *tokenize(const char *input);
void handle_start_state(t_tokenizer *t);
void handle_word_state(t_tokenizer *t);
void handle_single_quote_state(t_tokenizer *t);
void handle_double_quote_state(t_tokenizer *t);
void handle_variable_state(t_tokenizer *t);
int is_whitespace(char c);
int is_special_char(char c);
int is_quote(char c);
int handle_redirection(const char *input, int index, t_token **tokens);
int is_end_of_input(t_tokenizer *t);


#endif