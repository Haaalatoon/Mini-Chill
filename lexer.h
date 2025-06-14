#ifndef LEXER_H
#define LEXER_H

#include "libft/libft.h"
#include <stdio.h>

// Token types
typedef enum e_token_type
{
    Word,
    Pipe,
    Redirect_In,
    Redirect_Out,
    Append,
    Heredoc,
    End_of_file,
    // Undefined to check for later
} t_token_type;

// Quote types
typedef enum e_quote_type
{
    No_quotes,
    Single_quotes,
    Double_quotes
} t_quote_type;

// Expendable status
typedef enum e_expendable
{
    Not_expendable,
    Expendable
} t_expendable;

// Lexer context
typedef enum e_context
{
    Unquoted,
    Quoted,
    Separator
} t_context;

// Lexer states
typedef enum e_state
{
    In_space,
    In_literal,
    In_pipe,
    In_redirect,
    In_append,
    In_heredoc,
    In_quote,
    In_double_quote,
    In_param,
    In_EOF
} t_state;

// Token structure
typedef struct s_token
{
    char *value;
    t_token_type type;
    t_quote_type quote;
    t_expendable expendable;
} t_token;

typedef struct s_lexer
{
    char *offset;
    char *input; // This should be the input string to be lexed
    t_list *tokens; // This should be the head of a linked list of tokens
    t_state state;
    t_context context;
} t_lexer;

// Lexer helpers
int is_whitespace(char c);
int is_quote(char c);
int is_redirect(char c);
int is_seperator(char c);
int is_valid_param_start(char *str);
int is_valid_param_char(char c);
int contains_parameter(char *value);

// Lexer initialization and cleanup
t_lexer *init_lexer(const char *input);
void free_lexer(t_lexer *lexer);

// Token management
t_token *create_token(char *value, t_token_type type, t_quote_type quote, t_expendable expendable);
void free_token(t_token *token);
void append_token(t_lexer *lexer, t_token *token);

// Lexer state management
void set_state(t_lexer *lexer, char current_char);
void set_context(t_lexer *lexer, char current_char);

// Lexer processing
#endif