#include "minishell.h"

int handle_redirection(const char *input, int index, t_token **tokens)
{
    t_token_type type;
    char value[3];
    
    value[0] = input[index];
    value[1] = '\0';
    value[2] = '\0';
    
    if (input[index] == '<')
    {
        if (input[index + 1] && input[index + 1] == '<')
        {
            type = TOKEN_HEREDOC;
            value[1] = '<';
            index++;
        }
        else
            type = TOKEN_REDIR_IN;
    }
    else /* Must be '>' */
    {
        if (input[index + 1] && input[index + 1] == '>')
        {
            type = TOKEN_APPEND;
            value[1] = '>';
            index++;
        }
        else
            type = TOKEN_REDIR_OUT;
    }
    
    add_token(tokens, type, value, 0);
    return (index);
}
