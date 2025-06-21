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


// void resolve_tokens(t_lexer *lexer)
// {
//     lexer->offset = lexer->input;

//     while (*(lexer->offset))
//     {
//         set_context(lexer, *(lexer->offset));
//         set_state(lexer);

//         // Skip whitespace
//         if (lexer->state == space)
//         {
//             lexer->offset++;
//             continue;
//         }

//         // Handle special separator tokens (|, <, >, etc.)
//         if (lexer->state == pi_pe || lexer->state == redirect_in || lexer->state == redirect_out || lexer->state == append || lexer->state == heredoc)
//         {
//             t_token_type type = get_token_type(lexer->state);
//             int len = (lexer->state == append || lexer->state == heredoc) ? 2 : 1;
//             char *op = ft_substr(lexer->offset, 0, len);
//             append_token(lexer, create_token(op, type, Not_expendable));
//             free(op);
//             lexer->offset += len;
//             continue;
//         }
		
//         // Collect full literal token (quoted/unquoted) until space or separator
//         char *merged = ft_strdup("");
//         t_expendable expend = Not_expendable;

//         while (*(lexer->offset) && !is_seperator(*(lexer->offset)))
//         {
//             if (is_quote(*(lexer->offset)))
//             {
//                 char quote = *(lexer->offset++);
//                 char *start = lexer->offset;
//                 int part_expend = Not_expendable;

//                 while (*(lexer->offset) && *(lexer->offset) != quote)
//                 {
// 					set_context(lexer, *(lexer->offset));
//         			set_state(lexer);
// 					if (lexer->state == param_here)
//                         part_expend = Expendable;
//                     lexer->offset++;
//                 }

//                 char *chunk = ft_substr(start, 0, lexer->offset - start);
//                 char *tmp = merged;
//                 merged = ft_strjoin(tmp, chunk);
//                 free(chunk);
//                 free(tmp);

//                 if (part_expend == Expendable)
//                     expend = Expendable;

//                 if (*(lexer->offset) == quote)
//                     lexer->offset++; // skip closing quote
//             }
//             else if (lexer->state == param_here)
//             {
//                 char *start = lexer->offset;
//                 lexer->offset++; // skip $
//                 while (is_valid_param_char(*(lexer->offset)))
//                     lexer->offset++;
//                 int len = lexer->offset - start;

//                 char *chunk = ft_substr(start, 0, len);
//                 char *tmp = merged;
//                 merged = ft_strjoin(tmp, chunk);
//                 free(chunk);
//                 free(tmp);
//                 expend = Expendable;
//             }
//             else
//             {
//                 char *chunk = ft_substr(lexer->offset, 0, 1);
//                 char *tmp = merged;
//                 merged = ft_strjoin(tmp, chunk);
//                 free(chunk);
//                 free(tmp);
//                 lexer->offset++;
//             }
// 			if (*(lexer->offset))
//             {
//                 set_context(lexer, *(lexer->offset));
//                 set_state(lexer);
//             }
//         }
//         if (*merged)
//             append_token(lexer, create_token(merged, Word, expend));
//             // No need to free `merged` here, already passed to create_token
// 		free(merged);
//     }
// }

// void resolve_tokens(t_lexer *lexer)
// {
//     lexer->offset = lexer->input;
//     t_expendable expend = Not_expendable;
//     char *start = NULL;
// 	char *chunk;

//     while (*(lexer->offset))
//     {
//         set_context(lexer, *(lexer->offset));
//         set_state(lexer);

//         if (lexer->state == space && !start)
//         {
//             lexer->offset++;
//             continue;
//         }
//         // Start new token if we don't have one
//         if (!start)
//             start = lexer->offset;

//         if (lexer->state == param_here)
//             expend = Expendable;

//         // Create token when we hit space or EOF
//         if ((lexer->state == space || !*(lexer->offset)) && start)
//         {
//             chunk = ft_substr(start, 0, lexer->offset - start);
//             append_token(lexer, create_token(chunk, Word, expend));
//             free(chunk);
//             start = NULL;
//             expend = Not_expendable;
//         }
//         lexer->offset++;
//     }

//     if (start)
//     {
//         chunk = ft_substr(start, 0, lexer->offset - start);
//         append_token(lexer, create_token(chunk, Word, expend));
//         free(chunk);
//     }
// }


// void resolve_tokens(t_lexer *lexer)
// {
//     lexer->offset = lexer->input;
//     t_expendable expend = Not_expendable;
//     char *chunk;
//     char *start;
//     char *tmp;
//     char *merged;

//     while (*(lexer->offset))
//     {
//         set_context(lexer, *(lexer->offset));
//         set_state(lexer);

//         if (lexer->state == space)
//         {
//             lexer->offset++;
//             continue;
//         }
        
//         // handle redirect later

//         while (*(lexer->offset) && !is_seperator(*(lexer->offset)))
//         {
//             set_context(lexer, *(lexer->offset));
//             set_state(lexer);
//             if (lexer->state == single_quote || lexer->state == double_quote)
//             {
//                 start = ++lexer->offset; // skip opening quote
//                 while (lexer->state == literal || lexer->state == param_here)
//                 {
//                     set_context(lexer, *(lexer->offset));
//                     set_state(lexer);
//                     if (lexer->state == param_here)
//                         expend = Expendable;
//                     lexer->offset++;
//                 }
//                 chunk = ft_substr(start, 0, lexer->offset - start - 1); // skip closing quote
//                 tmp = merged;
//                 merged = ft_strjoin(tmp, chunk);
//                 free(chunk);
//                 free(tmp);
//             }
//             else
//             {
//                 chunk = ft_substr(lexer->offset, 0, 1);
//                 tmp = merged;
//                 merged = ft_strjoin(tmp, chunk);
//                 free(chunk);
//                 free(tmp);
//                 lexer->offset++;
//             }
//         }
//     }
//     if (*(lexer->offset))
//     {
//         set_context(lexer, *(lexer->offset));
//         set_state(lexer);
//     }
//     if (merged)
//     {
//         append_token(lexer, create_token(merged, Word, expend));
//         // free(merged);
//         merged = NULL;
//     }
// }

void resolve_tokens(t_lexer *lexer)
{
    lexer->offset = lexer->input;
    char *token = NULL;
    t_expendable expend = Not_expendable;
    int after_operator = 0; 

    while (*(lexer->offset))
    {
        set_context(lexer, *(lexer->offset));
        set_state(lexer);

        // Skip spaces between tokens
        if (lexer->state == space && (!token || after_operator))
        {
            lexer->offset++;
            continue;
        }

        // Start new token
        if (!token)
            token = ft_strdup("");

        // Handle operators
        if (lexer->state == pi_pe || lexer->state == redirect_in || 
            lexer->state == redirect_out || lexer->state == append || 
            lexer->state == heredoc)
        {
            // Handle previous token
            if (*token)
            {
                append_token(lexer, create_token(token, Word, expend));
                free(token);
                token = NULL;
                expend = Not_expendable;
            }

            // Create operator token
            int len = (lexer->state == append || lexer->state == heredoc) ? 2 : 1;
            char *op = ft_substr(lexer->offset, 0, len);
            append_token(lexer, create_token(op, get_token_type(lexer->state), Not_expendable));
            free(op);
            lexer->offset+= len;
            after_operator = 1;
            continue;
        }

        if (lexer->state == literal || lexer->state == param_here)
        {
            char *tmp = token;
            char c[2] = {*(lexer->offset), '\0'};
            token = ft_strjoin(tmp, c);
            free(tmp);
            if (lexer->state == param_here)
                expend = Expendable;
        }        

        // Create token on space
        if (lexer->state == space && token)
        {
            append_token(lexer, create_token(token, Word, expend));
            free(token);
            token = NULL;
            expend = Not_expendable;
        }

        lexer->offset++;
    }
    if (token)
    {
        append_token(lexer, create_token(token, Word, expend));
        free(token);
    }
}