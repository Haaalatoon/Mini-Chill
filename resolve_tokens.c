#include "lexer.h"

#include "lexer.h" // or your central header that defines enums

t_token_type get_token_type(t_state state)
{
    if (state == pi_pe)
        return Pipe;
    if (state == redirect_in)
        return Redirect_In;
    if (state == redirect_out)
        return Redirect_Out;
    if (state == append)
        return Append;
    if (state == heredoc)
        return Here_doc;
    if (state == E_OF)
        return End_of_file;
    if (state == Delim)
        return Delim;
    return Word; // default
}

t_quote_type get_quote_type(t_context context)
{
    if (context == Quoted)
        return Single_quotes;
    if (context == Double_quoted)
        return Double_quotes;
    return No_quotes;
}

// Helper function to check if state represents an operator
int is_operator_state(t_state state)
{
    return (state == pi_pe || state == redirect_in || 
            state == redirect_out || state == append || 
            state == heredoc);
}

// Helper function to check if state represents content
int is_content_state(t_state state)
{
    return (state == literal || state == param_here || 
            state == single_quote || state == double_quote);
}

// Helper function to create operator tokens
void create_operator_token(t_lexer *lexer)
{
    if (lexer->state == heredoc)
    {
        append_token(lexer, create_token("<<", Here_doc, Not_expendable));
        lexer->offset++;
    }
    else if (lexer->state == append)
    {
        append_token(lexer, create_token(">>", Append, Not_expendable));
        lexer->offset++; // Skip second character (main loop will skip first)
    }
    else if (lexer->state == redirect_in)
    {
        append_token(lexer, create_token("<", Redirect_In, Not_expendable));
        lexer->offset++; // Skip next character if any
    }
    else if (lexer->state == redirect_out)
    {
        append_token(lexer, create_token(">", Redirect_Out, Not_expendable));
        lexer->offset++; // Skip next character if any
    }
    else if (lexer->state == pi_pe)
    {
        append_token(lexer, create_token("|", Pipe, Not_expendable));
        lexer->offset++; // Skip next character if any
    }
}

// Helper function to add character to string
char *add_char_to_string(char *str, char c)
{
    int len = str ? ft_strlen(str) : 0;
    char *new_str = malloc(len + 2);
    if (!new_str)
        return str;
    
    if (str)
    {
        ft_strlcpy(new_str, str, len + 1);
        new_str[len] = c;
        new_str[len + 1] = '\0';
    }
    else
    {
        new_str[0] = c;
        new_str[1] = '\0';
    }
    
    return new_str;
}

void resolve_tokens(t_lexer *lexer)
{
    lexer->offset = lexer->input;
    char *token_content = NULL;
    t_expendable expend = Not_expendable;
    unsigned long current_mask = 0;
    size_t pos = 0; // position in the token
    
    while (*(lexer->offset))
    {
        set_context(lexer, *(lexer->offset));
        set_state(lexer);
        
        // Skip whitespace that separates tokens
        if (lexer->state == space && lexer->context == Separator)
        {
            // // Finalize current token if we have one
            // if (token_content && *token_content)
            // {
            //     append_token(lexer, create_token(token_content, Word, expend));
            //     free(token_content);
            //     token_content = NULL;
            //     expend = Not_expendable;
            // }
            // lexer->offset++;
            // continue;
            if (token_content && *token_content)
            {
                t_token *token = create_token(token_content, Word, expend);
                token->expansion_mask = current_mask;  // Add this line
                append_token(lexer, token);
                free(token_content);
                token_content = NULL;
                expend = Not_expendable;
                current_mask = 0;  // Add this line
            }
            lexer->offset++;
            continue;

        }
        
        // Handle operators (they are always separate tokens)
        if (is_operator_state(lexer->state))
        {
            // Finalize current word token first
            if (token_content && *token_content)
            {
                if (lexer->in_heredoc_delim)
                    expend = Not_expendable;
                // append_token(lexer, create_token(token_content, Word, expend));
                
                // free(token_content);
                // token_content = NULL;
                // expend = Not_expendable;
                t_token *token = create_token(token_content, Word, expend);
                token->expansion_mask = current_mask;  // Add this line
                append_token(lexer, token);
                free(token_content);
                token_content = NULL;
                expend = Not_expendable;
                current_mask = 0;  // Add this line
            }
            
            // Create operator token
            create_operator_token(lexer);
            continue;
        }
        
        // Handle content that becomes part of a word token
        if (is_content_state(lexer->state))
        {
            // Initialize token if needed
            if (!token_content)
                token_content = ft_strdup("");
            
            // Process quotes (they don't appear in final token)
            if (lexer->state == single_quote || lexer->state == double_quote)
            {
                lexer->offset++; // Skip quote character
                continue;
            }
            
            // Add content to current token
            if (lexer->state == literal || lexer->state == param_here)
            {
                // if (lexer->state == param_here)
                //     expend = Expendable;
                
                // char *new_content = add_char_to_string(token_content, *(lexer->offset));
                // free(token_content);
                // token_content = new_content;
                if (lexer->state == param_here)
                {
                    // Set bit for this $ position
                    int pos = token_content ? ft_strlen(token_content) : 0;  // Add this line
                    if (pos < 64)  // Add this line
                        current_mask |= (1UL << pos);  // Add this line
                    expend = Expendable;
                }
                
                char *new_content = add_char_to_string(token_content, *(lexer->offset));
                free(token_content);
                token_content = new_content;
            }
        }
        lexer->offset++;
        pos++;
    }
    if (token_content && *token_content)
    {
        // if (lexer->in_heredoc_delim)
        //     expend = Not_expendable;
        // append_token(lexer, create_token(token_content, Word, expend));
        // free(token_content);
        if (lexer->in_heredoc_delim)
            expend = Not_expendable;
        t_token *token = create_token(token_content, Word, expend);
        token->expansion_mask = current_mask;  // Add this line
        append_token(lexer, token);
        free(token_content);
    }
    
    // Add EOF token
    append_token(lexer, create_token("", End_of_file, Not_expendable));
}

