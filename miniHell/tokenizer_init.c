#include "minishell.h"

void init_tokenizer(t_tokenizer *t, const char *input)
{
    if (!t || !input)
        return;
        
    t->input = input;
    t->index = 0;
    t->state = STATE_START;
    t->buf_len = 0;
    t->var_start = 0;
    t->tokens = NULL;
    t->error = 0;        // Initialize error flag
    t->error_msg = NULL; // Initialize error message
}

int is_end_of_input(t_tokenizer *t)
{
    return (t->input[t->index] == '\0');
}

t_token *tokenize(const char *input)
{
    t_tokenizer t;
    
    if (!input)
        return (NULL);
        
    init_tokenizer(&t, input);
    
    while (t.input[t.index] != '\0')
    {
        if (t.state == STATE_START)
            handle_start_state(&t);
        else if (t.state == STATE_WORD)
            handle_word_state(&t);
        else if (t.state == STATE_SINGLE_QUOTE)
            handle_single_quote_state(&t);
        else if (t.state == STATE_DOUBLE_QUOTE)
            handle_double_quote_state(&t);
        else if (t.state == STATE_VARIABLE)
            handle_variable_state(&t);
    }

    /* Handle errors */
    if (t.error)
    {
        if (t.error_msg)
            printf("Error: %s\n", t.error_msg);
        free_tokens(t.tokens);
        return (NULL);
    }

    /* Handle EOF for current state */
    if (t.state == STATE_WORD && t.buf_len > 0)
    {
        t.buffer[t.buf_len] = '\0';
        add_token(&t.tokens, TOKEN_WORD, t.buffer, 0);
    }
    else if (t.state == STATE_SINGLE_QUOTE || t.state == STATE_DOUBLE_QUOTE)
    {
        printf("Error: tokenizer: unbalanced quotes!\n");
        /* Error: unclosed quote at EOF */
        free_tokens(t.tokens);
        return (NULL);
    }
    
    else if (t.state == STATE_VARIABLE)
    {
        // Handle unfinished variable at EOF
        int var_len = t.index - t.var_start;
        if (var_len > 0)
        {
            char var_token[256];
            if (var_len == 1 && t.input[t.var_start] == '?')
                strcpy(var_token, "$?");
            else
            {
                var_token[0] = '$';
                ft_strlcpy(var_token + 1, t.input + t.var_start, var_len + 1);
            }
            add_token(&t.tokens, TOKEN_VAR, var_token, 1);
        }
        else
            add_token(&t.tokens, TOKEN_WORD, "$", 0);
    }

    /* Add EOF token */
    add_token(&t.tokens, TOKEN_EOF, "", 0);
    return (t.tokens);
}