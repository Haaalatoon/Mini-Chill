#include "minishell.h"

// void handle_start_state(t_tokenizer *t)
// {
//     char c = t->input[t->index];
    
//     if (is_whitespace(c))
//     {
//         t->index++;
//         return;
//     }
    
//     if (c == '\'')
//     {
//         t->state = STATE_SINGLE_QUOTE;
//         t->buf_len = 0;
//         t->index++;
//     }
//     else if (c == '"')
//     {
//         t->state = STATE_DOUBLE_QUOTE;
//         t->buf_len = 0;
//         t->index++;
//     }
//     else if (c == '$')
//     {
//         t->state = STATE_VARIABLE;
//         t->var_start = t->index + 1;
//         t->index++;
//     }
//     else if (c == '|')
//     {
//         add_token(&t->tokens, TOKEN_PIPE, "|", 0);
//         t->index++;
//     }
//     else if (c == '<' || c == '>')
//     {
//         t->index = handle_redirection(t->input, t->index, &t->tokens) + 1;
//     }
//     else
//     {
//         t->state = STATE_WORD;
//         t->buf_len = 0;
//     }
// }
void handle_start_state(t_tokenizer *t)
{
    char c = t->input[t->index];
    
    if (is_whitespace(c))
    {
        t->index++;
        return;
    }
    
    if (c == '\n')  // Add newline handling
    {
        add_token(&t->tokens, TOKEN_NEWLINE, "\\n", 0);
        t->index++;
    }
    else if (c == '\'')
    {
        t->state = STATE_SINGLE_QUOTE;
        t->buf_len = 0;
        t->index++;
    }
    else if (c == '"')
    {
        t->state = STATE_DOUBLE_QUOTE;
        t->buf_len = 0;
        t->index++;
    }
    else if (c == '$')
    {
        t->state = STATE_VARIABLE;
        t->var_start = t->index + 1;
        t->index++;
    }
    else if (c == '|')
    {
        add_token(&t->tokens, TOKEN_PIPE, "|", 0);
        t->index++;
    }
    else if (c == '<' || c == '>')
    {
        t->index = handle_redirection(t->input, t->index, &t->tokens) + 1;
    }
    else
    {
        t->state = STATE_WORD;
        t->buf_len = 0;
    }
}

// void handle_word_state(t_tokenizer *t)
// {
//     char c = t->input[t->index];
    
//     if (c == '\0' || is_whitespace(c) || is_special_char(c) || is_quote(c))
//     {
//         t->buffer[t->buf_len] = '\0';
//         add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
//         t->state = STATE_START;
//     }
//     else
//     {
//         t->buffer[t->buf_len++] = c;
//         t->index++;
//     }
// }

void handle_word_state(t_tokenizer *t)
{
    char c = t->input[t->index];
    
    // Check for end of word conditions
    if (c == '\0' || is_whitespace(c) || c == '|' || c == '<' || c == '>' || c == '\n')
    {
        // Complete the current word
        t->buffer[t->buf_len] = '\0';
        if (t->buf_len > 0)  // Only add non-empty word
            add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        t->state = STATE_START;
        // Don't increment index - let START state handle the character
    }
    else if (c == '\'' || c == '"')
    {
        // Quote within a word - transition to quote state
        // Keep current buffer content
        if (c == '\'')
            t->state = STATE_SINGLE_QUOTE;
        else
            t->state = STATE_DOUBLE_QUOTE;
        t->index++;
    }
    else if (c == '$')
    {
        // Variable within a word - finish current word first if buffer has content
        if (t->buf_len > 0)
        {
            t->buffer[t->buf_len] = '\0';
            add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
            t->buf_len = 0;
        }
        t->state = STATE_VARIABLE;
        t->var_start = t->index + 1;
        t->index++;
    }
    else
    {
        // Regular character
        t->buffer[t->buf_len++] = c;
        t->index++;
    }
}

void handle_single_quote_state(t_tokenizer *t)
{
    char c = t->input[t->index];

    if (c == '\0')
    {
        /* Error: unclosed single quote */
        free_tokens(t->tokens);
        t->tokens = NULL;
        t->state = STATE_START;
        // t->error = 1;
        // t->error_msg = "Unclosed single quote";
        return;
    }
    else if (c == '\'')
    {
        // t->buffer[t->buf_len] = '\0';
        // if (t->buf_len > 0)
        //     add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        // t->state = STATE_START;
        // t->index++;
        // End of single quote - add token if we have content
        if (t->buf_len > 0)
        {
            t->buffer[t->buf_len] = '\0';
            add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        }
        t->state = STATE_WORD;  // Return to WORD state, not START
        t->buf_len = 0;         // Reset buffer
        t->index++;
    }
    else
    {
        // // Check buffer overflow
        // if (t->buf_len >= BUFF_SIZE - 1)
        // {
        //     t->error = 1;
        //     t->error_msg = "Quoted string too long";
        //     return;
        // }
        t->buffer[t->buf_len++] = c;
        t->index++;
    }
}

void handle_double_quote_state(t_tokenizer *t)
{
    char c = t->input[t->index];
    
    if (c == '\0')
    {
        /* Error: unclosed double quote */
        free_tokens(t->tokens);
        t->tokens = NULL;
        // return;  // Exit early to avoid further processing
        t->state = STATE_START;
    }
    else if (c == '"')
    {
        // // End of double quote - add token if we have content
        // if (t->buf_len > 0)
        // {
        //     t->buffer[t->buf_len] = '\0';
        //     add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        // }
        // t->state = STATE_WORD;  // Return to WORD state
        // t->buf_len = 0;         // Reset buffer
        // t->index++;
        t->buffer[t->buf_len] = '\0';
        if (t->buf_len > 0)
            add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        t->state = STATE_START;
        t->index++;
    }
    else if (c == '$')
    {
        // // Handle variable expansion inside double quotes
        // if (t->buf_len > 0)
        // {
        //     t->buffer[t->buf_len] = '\0';
        //     add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        //     t->buf_len = 0;
        // }
        
        // // Switch to variable state
        // t->state = STATE_VARIABLE;
        // t->var_start = t->index + 1;
        // t->index++;
        // Handle variable expansion inside double quotes
        t->buffer[t->buf_len] = '\0';
        if (t->buf_len > 0)
            add_token(&t->tokens, TOKEN_WORD, t->buffer, 0);
        
        /* Switch to variable state */
        t->state = STATE_VARIABLE;
        t->var_start = t->index + 1;
        t->buf_len = 0;  // Reset buffer length for variable
        t->index++;
    }
    else
    {
        t->buffer[t->buf_len++] = c;
        t->index++;
    }
}

// void handle_variable_state(t_tokenizer *t)
// {
//     char c = t->input[t->index];
//     char var_token[256];
    
//     /* If the next character is not a valid variable character */
//     if (c == '\0' || !(ft_isalnum(c) || c == '_' || c == '?'))
//     {
//         /* If we have a variable name, process it */
//         if (t->index > t->var_start)
//         {
//             /* Handle special case for $? */
//             if (t->input[t->var_start] == '?')
//             {
//                 strcpy(var_token, "$?");
//             }
//             else
//             {
//                 /* Create variable token with $ prefix */
//                 var_token[0] = '$';
//                 ft_strlcpy(var_token + 1, t->input + t->var_start, t->index - t->var_start + 1);
//             }
            
//             add_token(&t->tokens, TOKEN_VAR, var_token, 1);
//         }
//         else
//         {
//             /* Handle standalone $ as a WORD token, not a VAR token */
//             add_token(&t->tokens, TOKEN_WORD, "$", 0);
//         }
        
//         /* Handle returning to a quote state if needed */
//         if (c == '"' || c == '\'')
//             t->index++;
            
//         t->state = STATE_START;
//     }
//     else
//     {
//         t->index++;
//     }
// }
void handle_variable_state(t_tokenizer *t)
{
    char c = t->input[t->index];
    char var_token[256];
    int var_len = t->index - t->var_start;
    
    // Check if we've reached the end of a valid variable name
    if (c == '\0' || !(ft_isalnum(c) || c == '_' || c == '?'))
    {
        if (var_len > 0)
        {
            // Special case for $? 
            if (var_len == 1 && t->input[t->var_start] == '?')
            {
                strcpy(var_token, "$?");
            }
            else
            {
                // Regular variable: $VAR_NAME
                var_token[0] = '$';
                ft_strlcpy(var_token + 1, t->input + t->var_start, var_len + 1);
            }
            add_token(&t->tokens, TOKEN_VAR, var_token, 1);
        }
        else
        {
            // Standalone $ is treated as a word
            add_token(&t->tokens, TOKEN_WORD, "$", 0);
        }
        
        // Return to appropriate state
        if (c == '"')
        {
            t->state = STATE_DOUBLE_QUOTE;
            t->buf_len = 0;
            t->index++;
        }
        else
        {
            t->state = STATE_START;
        }
    }
    else
    {
        // Continue reading variable name
        t->index++;
    }
}