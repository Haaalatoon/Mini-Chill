#ifndef LEXER_H
#define LEXER_H

#include "libft/libft.h"

enum Tokens // This is for the token type
{
    Word,
    Pipe,
    Redirect_In,
    Redirect_Out,
    Append,
    Heredoc,
    End_of_file,
    Undefined
};

enum QuoteType // This is for the quote type in the token
{
    No_quotes,
    Single_quotes,
    Double_quotes
};

enum Expendable // This is for the expendable type in the token
{
    Not_expendable,
    Expendable
};

enum Context
{
    Unquoted,
    S_Quoted,
    D_Quoted,
};

enum State
{
    In_space,
    In_literal,
    In_pipe,
    In_redirect,
    In_append,
    In_heredoc,
    In_quote,
    In_double_quote,
    In_undefined,
    In_param,
    In_EOF
};

typedef struct s_token
{
    char *value;
    int type;
    int quote;
    int expendable;
} t_token;

typedef struct s_lexer
{
    char *offset;
    t_list *tokens; // This should be the head of a linked list of tokens
    int state;
    int context;
} t_lexer;


#endif