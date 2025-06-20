#include "lexer.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

// int main(void)
// {
//     char *input = NULL;
//     t_lexer *lexer = NULL;

//     while (1)
//     {    input = readline("minishell> ");
//     if (!input)  // EOF (Ctrl‑D)
//         break;

//     // Optionally add non-empty lines to history
//     if (*input)
//         add_history(input);

//     // Initialize lexer on the user input
//     lexer = init_lexer(input);
//     if (!lexer)
//     {
//         fprintf(stderr, "Lexer init failed\n");
//         free(input);
//         return 1;
//     }

//     printf("Input: %s\n\n", input);

//     // Walk through each character
//     while (*(lexer->offset))
//     {
//         char c = *(lexer->offset);
        
        
//         set_context(lexer, c);
//         set_state(lexer);


//         printf("Char: '%c' (%d) | State: %-20s | Context: %-10s\n",
//                 c, c, state_to_str(lexer->state), context_to_str(lexer->context));

//         lexer->offset++;
//     }

//     // Finally handle EOF marker

    
//     set_context(lexer, '\0');
//     set_state(lexer);
//     printf("Char: '\\0'        | State: %-20s | Context: %-10s\n",
//             state_to_str(lexer->state), context_to_str(lexer->context));

//     free_lexer(lexer);
//     free(input);
//     }
//     resolve_tokens(lexer);

//     // Print tokens
//     t_list *cur = lexer->tokens;
//     while (cur)
//     {
//         t_token *tok = (t_token *)cur->content;
//         printf("Token: %-10s | Type: %d | Quote: %d | Expandable: %d\n",
//             tok->value, tok->type, tok->quote, tok->expendable);
//         cur = cur->next;
//     }

//     return 0;
// }
const char *token_type_to_str(t_token_type type)
{
    switch (type)
    {
        case Word:         return "Word";
        case Pipe:         return "Pipe";
        case Redirect_In:  return "Redirect_In";
        case Redirect_Out: return "Redirect_Out";
        case Append:       return "Append";
        case Here_doc:     return "Here_doc";
        case End_of_file:  return "EOF";
        default:           return "Unknown";
    }
}

const char *quote_type_to_str(t_quote_type quote)
{
    switch (quote)
    {
        case No_quotes:      return "No_quotes";
        case Single_quotes:  return "Single_quotes";
        case Double_quotes:  return "Double_quotes";
        default:             return "Unknown";
    }
}

const char *expendable_to_str(t_expendable ex)
{
    switch (ex)
    {
        case Expendable:     return "Expendable";
        case Not_expendable: return "Not_expendable";
        default:             return "Unknown";
    }
}


int main(void)
{
    char *input = NULL;
    t_lexer *lexer = NULL;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        if (*input)
            add_history(input);

        lexer = init_lexer(input);
        if (!lexer)
        {
            fprintf(stderr, "Lexer init failed\n");
            free(input);
            return 1;
        }

        printf("Input: %s\n\n", input);
        while (*(lexer->offset))
        {
            char c = *(lexer->offset);
            set_context(lexer, c);
            set_state(lexer); // if you added char param
            printf("Char: '%c' (%d) | State: %-20s | Context: %-10s\n",
                   c, c, state_to_str(lexer->state), context_to_str(lexer->context));
            lexer->offset++;
        }

        // Print final EOF
        set_context(lexer, '\0');
        set_state(lexer);
        printf("Char: '\\0'        | State: %-20s | Context: %-10s\n",
               state_to_str(lexer->state), context_to_str(lexer->context));

        // Reset offset and resolve
        lexer->offset = lexer->input;
        resolve_tokens(lexer);

        // // Show tokens
        // printf("\nResolved Tokens:\n");
        // t_list *cur = lexer->tokens;
        // while (cur)
        // {
        //     t_token *tok = (t_token *)cur->content;
        //     printf("Token: %-10s | Type: %-12s | Quote: %-15s | Expandable: %s\n",
        //         tok->value,
        //      token_type_to_str(tok->type),
        //      quote_type_to_str(tok->quote),
        //     expendable_to_str(tok->expendable));

        //     cur = cur->next;
        // }

        free_lexer(lexer);
        free(input);
    }
    return 0;
}
