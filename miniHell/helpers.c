#include "minishell.h"

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}