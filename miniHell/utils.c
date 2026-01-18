#include "minishell.h"

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t');  // Don't include \n here since it's a token
}

// Add this function for better variable validation
int is_valid_var_start(char c)
{
    return (ft_isalpha(c) || c == '_' || c == '?');
}

int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '$');
}

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

int is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_' || c == '?');
}
