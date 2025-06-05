#include "minishell.h"


t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->next = NULL;
    return (token);
}


void add_token(t_token **tokens, t_token *new_token)
{
    t_token *current;

    if (!*tokens)
        *tokens = new_token;
    else
    {
        current = *tokens;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}


int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}


int handle_quotes(char *input, int i, char **value)
{
    int start;
    char quote_type;

    quote_type = input[i];
    start = i + 1;
    i++;
    
    // Find the closing quote
    while (input[i] && input[i] != quote_type)
        i++;
    
    // If we didn't find a closing quote, return an error
    if (!input[i])
        return (-1);
    
    // Extract the quoted text (without the quotes)
    *value = ft_substr(input, start, i - start);
    
    return (i);
}

int extract_word(char *input, int i, char **value)
{
    int start;

    start = i;
    
    // Continue until we hit a special character or whitespace
    while (input[i] && !is_whitespace(input[i]) && !is_special_char(input[i]))
    {
        // Handle quotes within a word
        if (input[i] == '\'' || input[i] == '\"')
        {
            char quote_type = input[i];
            i++;
            while (input[i] && input[i] != quote_type)
                i++;
            if (!input[i])  // Unclosed quote
                return (-1);
        }
        i++;
    }
    
    *value = ft_substr(input, start, i - start);
    i--; // Move back one to handle the next character properly
    
    return (i);
}


int handle_redirection(char *input, int i, t_token **tokens)
{
    t_token_type type;
    char *value;

    if (input[i] == '<')
    {
        if (input[i + 1] && input[i + 1] == '<')
        {
            type = TOKEN_HEREDOC;
            value = ft_strdup("<<");
            i++;
        }
        else
        {
            type = TOKEN_REDIR_IN;
            value = ft_strdup("<");
        }
    }
    else // input[i] == '>'
    {
        if (input[i + 1] && input[i + 1] == '>')
        {
            type = TOKEN_APPEND;
            value = ft_strdup(">>");
            i++;
        }
        else
        {
            type = TOKEN_REDIR_OUT;
            value = ft_strdup(">");
        }
    }
    
    add_token(tokens, create_token(type, value));
    free(value);
    
    return (i);
}

t_token *tokenize(const char *input)
{
    t_tokenizer t;
    
    if (!input)
        return (NULL);
        
    init_tokenizer(&t, input);
    
    while (t.input[t.index] != '\0' && !t.error)
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
    
    // Handle errors
    if (t.error)
    {
        if (t.error_msg)
            printf("Error: %s\n", t.error_msg);
        free_tokens(t.tokens);
        return (NULL);
    }
    
    // Handle final state
    if (t.state == STATE_WORD && t.buf_len > 0)
    {
        t.buffer[t.buf_len] = '\0';
        add_token(&t.tokens, TOKEN_WORD, t.buffer, 0);
    }
    else if (t.state == STATE_SINGLE_QUOTE || t.state == STATE_DOUBLE_QUOTE)
    {
        printf("Error: tokenizer: unbalanced quotes!\n");
        free_tokens(t.tokens);
        return (NULL);
    }
    
    // Add EOF token
    add_token(&t.tokens, TOKEN_EOF, "", 0);
    return (t.tokens);
}

/*
 * Debug function to print all tokens
 */
void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    const char *type_names[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC", "EOF"
    };

    printf("Tokens:\n");
    while (current)
    {
        printf("Type: %s, Value: '%s'\n", type_names[current->type], current->value);
        current = current->next;
    }
    printf("\n");
}

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char *input;
    t_token *tokens;

    printf("Minishell Tokenizer Tester\n");
    printf("Enter commands to see how they're tokenized. Press Ctrl+D to exit.\n");

    while (1)
    {
        // Display prompt and read input
        input = readline("minishell> ");

        // Exit on EOF (Ctrl+D)
        if (!input)
        {
            printf("\n");
            break;
        }

        // Tokenize the input
        tokens = tokenize(input);

        // Print the tokens
        if (tokens)
        {
            print_tokens(tokens);
            free_tokens(tokens);
        }
        else
        {
            printf("Error in tokenization\n");
        }

        // Free the input and add to history
        free(input);
    }

    return (0);
}
