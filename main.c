#include "lexer.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

// const char *token_type_to_str(t_token_type type)
// {
//     switch (type)
//     {
//         case Word:         return "Word";
//         case Pipe:         return "Pipe";
//         case Redirect_In:  return "Redirect_In";
//         case Redirect_Out: return "Redirect_Out";
//         case Append:       return "Append";
//         case Here_doc:     return "Here_doc";
//         case End_of_file:  return "EOF";
//         default:           return "Unknown";
//     }
// }

// const char *quote_type_to_str(t_quote_type quote)
// {
//     switch (quote)
//     {
//         case No_quotes:      return "No_quotes";
//         case Single_quotes:  return "Single_quotes";
//         case Double_quotes:  return "Double_quotes";
//         default:             return "Unknown";
//     }
// }

// const char *expendable_to_str(t_expendable ex)
// {
//     switch (ex)
//     {
//         case Expendable:     return "Expendable";
//         case Not_expendable: return "Not_expendable";
//         default:             return "Unknown";
//     }
// }


// int main(void)
// {
//     char *input = NULL;
//     t_lexer *lexer = NULL;

//     // while (1)
//     // {
//     //     input = readline("minishell> ");
//     //     if (!input)
//     //         break;
//     //     if (*input)
//     //         add_history(input);

//     //     lexer = init_lexer(input);
//     //     if (!lexer)
//     //     {
//     //         fprintf(stderr, "Lexer init failed\n");
//     //         free(input);
//     //         return 1;
//     //     }

//     //     printf("Input: %s\n\n", input);
//     //     while (*(lexer->offset))
//     //     {
//     //         char c = *(lexer->offset);
//     //         set_context(lexer, c);
//     //         set_state(lexer);
//     //         printf("Char: '%c' (%d) | State: %-20s | Context: %-10s\n",
//     //                c, c, state_to_str(lexer->state), context_to_str(lexer->context));
//     //         lexer->offset++;
//     //     }

//     //     // Print final EOF
//     //     set_context(lexer, '\0');
//     //     set_state(lexer);
//     //     printf("Char: '\\0'        | State: %-20s | Context: %-10s\n",
//     //            state_to_str(lexer->state), context_to_str(lexer->context));

//     //     free_lexer(lexer);
//     //     free(input);
//     // }

//     // lexer->offset = lexer->input;
//     // resolve_tokens(lexer);

//     // printf("\nResolved Tokens:\n");
//     // t_list *cur = lexer->tokens;
//     // while (cur)
//     // {
//     //     t_token *tok = (t_token *)cur->content;
//     //     printf("Token: %-15s | Type: %-12s | Expandable: %s\n",
//     //         tok->value,
//     //         token_type_to_str(tok->type),
//     //         expendable_to_str(tok->expendable));
//     //     cur = cur->next;
//     // }

//     // return 0;
//     while (1)
// {
//     input = readline("minishell> ");
//     if (!input)
//         break;
//     if (*input)
//         add_history(input);

//     lexer = init_lexer(input);
//     if (!lexer)
//     {
//         fprintf(stderr, "Lexer init failed\n");
//         free(input);
//         return 1;
//     }

//     printf("Input: %s\n\n", input);
    
//     // First pass - print char by char analysis
//     while (*(lexer->offset))
//     {
//         char c = *(lexer->offset);
//         set_context(lexer, c);
//         set_state(lexer);
//         printf("Char: '%c' (%d) | State: %-20s | Context: %-10s\n",
//                c, c, state_to_str(lexer->state), context_to_str(lexer->context));
//         lexer->offset++;
//     }

//     // Print final EOF
//     set_context(lexer, '\0');
//     set_state(lexer);
//     printf("Char: '\\0'        | State: %-20s | Context: %-10s\n",
//            state_to_str(lexer->state), context_to_str(lexer->context));

//     // Reset offset and resolve tokens
//     lexer->offset = lexer->input;
//     resolve_tokens(lexer);

//     // Print resolved tokens
//     printf("\nResolved Tokens:\n");
//     t_list *cur = lexer->tokens;
//     while (cur)
//     {
//         t_token *tok = (t_token *)cur->content;
//         printf("Token: %-15s | Type: %-12s | Expandable: %s\n",
//             tok->value,
//             token_type_to_str(tok->type),
//             expendable_to_str(tok->expendable));
//         cur = cur->next;
//     }

//     free_lexer(lexer);
//     free(input);
// }
//     return 0;
// }
#include "lexer.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

// Function declarations (put these at the top)
void print_binary(unsigned long mask, int bits);
void print_token_with_mask(t_token *token);
void test_input(const char *input);

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

void print_binary(unsigned long mask, int bits)
{
    printf("0b");
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (int)((mask >> i) & 1));  // Cast to int to fix format warning
    }
}

// Helper function to print token with expansion info
void print_token_with_mask(t_token *token)
{
    printf("Token: \"%s\" | Type: %d | Expandable: %s | Mask: ",
           token->value,
           token->type,
           token->expendable == Expendable ? "YES" : "NO");
    
    if (token->expansion_mask == 0) {
        printf("0 (no expansion)");
    } else {
        print_binary(token->expansion_mask, 8);
        printf(" (%lu)", token->expansion_mask);
    }
    printf("\n");
    
    // Show which characters would be expanded
    if (token->expansion_mask > 0 && token->value) {
        printf("  Expansion map: ");
        for (int i = 0; token->value[i]; i++) {
            if (token->expansion_mask & (1UL << i)) {
                printf("[%c]", token->value[i]);  // Expandable
            } else {
                printf(" %c ", token->value[i]);   // Literal
            }
        }
        printf("\n");
    }
}

// Test function to process input and show results
void test_input(const char *input)
{
    printf("\n=== Testing: \"%s\" ===\n", input);
    
    t_lexer *lexer = init_lexer(input);
    if (!lexer) {
        printf("Failed to initialize lexer\n");
        return;
    }
    
    resolve_tokens(lexer);
    
    // Print all tokens
    t_list *current = lexer->tokens;
    int token_num = 1;
    
    while (current) {
        t_token *token = (t_token *)current->content;
        printf("Token %d: ", token_num++);
        print_token_with_mask(token);
        current = current->next;
    }
    
    free_lexer(lexer);
}

void run_bitmask_tests(void)
{
    printf("\nTesting Bitmask Implementation for Parameter Expansion\n");
    printf("=====================================================\n");
    
    // Single quotes - should NOT expand
    test_input("'$HOME'");
    test_input("'$USER'");
    
    // Double quotes - should expand
    test_input("\"$HOME\"");
    test_input("\"$USER\"");
    
    // Unquoted - should expand
    test_input("$HOME");
    test_input("$USER");
    
    // Mixed quoting
    test_input("'$HOME'$USER");
    test_input("\"$HOME\"'$USER'");
    test_input("$HOME\"$USER\"");
    
    // Multiple variables in one token
    test_input("\"$HOME/$USER\"");
    test_input("$HOME$USER");
    
    // Heredoc delimiter (should not expand)
    test_input("<< $EOF");
    test_input("<< \"$DELIM\"");
    
    // Complex cases
    test_input("echo $HOME");
    test_input("echo \"Hello $USER\"");
    test_input("echo '$HOME is home'");
    test_input("cat << $EOF");
    
    // Edge cases
    test_input("$");  // Just $ without valid param
    test_input("$$");  // Special parameter
    test_input("$?");  // Exit status
}
void print_expandable_tokens(t_lexer *lexer)
{
    t_list *cur = lexer->tokens;
    int i = 1;
    while (cur)
    {
        t_token *tok = (t_token *)cur->content;
        if (tok->expendable == Expendable)
            printf("Expandable Token %d: %s\n", i, tok->value);
        cur = cur->next;
        i++;
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
            
        // Check for special test command
        if (strcmp(input, "test") == 0) {
            run_bitmask_tests();
            free(input);
            continue;
        }
        
        lexer = init_lexer(input);
        if (!lexer)
        {
            fprintf(stderr, "Lexer init failed\n");
            free(input);
            return 1;
        }
        printf("Input: %s\n\n", input);
        
        // First pass - print char by char analysis
        while (*(lexer->offset))
        {
            char c = *(lexer->offset);
            set_context(lexer, c);
            set_state(lexer);
            printf("Char: '%c' (%d) | State: %-20s | Context: %-10s\n",
                   c, c, state_to_str(lexer->state), context_to_str(lexer->context));
            lexer->offset++;
        }
        // Print final EOF
        set_context(lexer, '\0');
        set_state(lexer);
        printf("Char: '\\0'        | State: %-20s | Context: %-10s\n",
               state_to_str(lexer->state), context_to_str(lexer->context));
        
        // Reset offset and resolve tokens
        lexer->offset = lexer->input;
        resolve_tokens(lexer);
        
        // Print resolved tokens with bitmask info
        printf("\nResolved Tokens:\n");
        t_list *cur = lexer->tokens;
        int token_num = 1;
        while (cur)
        {
            t_token *tok = (t_token *)cur->content;
            printf("Token %d: ", token_num++);
            print_token_with_mask(tok);
            cur = cur->next;
        }
        print_expandable_tokens(lexer);
        
        free_lexer(lexer);
        free(input);
    }
    
    return 0;
}