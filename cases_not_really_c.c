// ============================================================================
//  * ALL POSSIBLE STATE AND CONTEXT COMBINATIONS (MANDATORY PART ONLY)
// ============================================================================


//  CONTEXT: Unquoted
//  =================
//  * State: In_space
//  * - Character: ' ', '\t', '\n'
//  * - Action: Skip whitespace, dont create token
//  * - Transition: Stay in In_space or move to other states
//  * 
//  * State: In_literal
//  * - Character: Regular characters (letters, digits, etc.)
//  * - Token Type: Word
//  * - Quote: No_quotes
//  * - Expendable: Check for '$' to determine
//  * 
//  * State: In_pipe
//  * - Character: '|'
//  * - Token Type: Pipe
//  * - Quote: No_quotes
//  * - Expendable: Not_expendable
//  * 
//  * State: In_redirect
//  * - Character: '<' or '>'
//  * - Token Type: Redirect_In or Redirect_Out
//  * - Quote: No_quotes
//  * - Expendable: Not_expendable
//  * 
//  * State: In_append
//  * - Character: '>>'
//  * - Token Type: Append
//  * - Quote: No_quotes
//  * - Expendable: Not_expendable
//  * 
//  * State: In_heredoc
//  * - Character: '<<'
//  * - Token Type: Heredoc
//  * - Quote: No_quotes
//  * - Expendable: Not_expendable
//  * 
//  * State: In_param
//  * - Character: '$' followed by valid parameter name
//  * - Token Type: Word
//  * - Quote: No_quotes
//  * - Expendable: Expendable
//  * 
//  * State: In_EOF
//  * - Character: '\0'
//  * - Token Type: End_of_file
//  * - Quote: No_quotes
//  * - Expendable: Not_expendable



//  CONTEXT: Quoted (Single quotes)
//  ===============================
//  * State: In_quote
//  * - Character: Everything between single quotes
//  * - Token Type: Word
//  * - Quote: Single_quotes
//  * - Expendable: Not_expendable (single quotes prevent expansion)
//  * - Special: No interpretation of any metacharacters including '$'



//  CONTEXT: Quoted (Double quotes)
//  ===============================
//  * State: In_double_quote
//  * - Character: Everything between double quotes
//  * - Token Type: Word
//  * - Quote: Double_quotes
//  * - Expendable: Check for '$' (only '$' expands in double quotes)
//  * - Special: Only '$' is interpreted, other metacharacters are literal



//  CONTEXT: Separator
//  ==================
//  * This context indicates we've hit a separator and should resolve the current token
//  * - Separators: ' ', '\t', '\n', '|', '<', '>', '\0'
//  * - Action: Generate token from accumulated characters, then process separator


// ============================================================================
//   DETAILED CASE ANALYSIS FOR EACH CHARACTER TYPE
// ============================================================================


//  * WHITESPACE (' ', '\t', '\n'):
//  * =============================
//  * Context: Unquoted -> Separator (if we have accumulated chars) or stay Unquoted
//  * Context: Quoted -> stays Quoted (whitespace is part of the token)
//  * State: In_space (if unquoted and no accumulated chars)



//  * PIPE ('|'):
//  * ===========
//  * Context: Unquoted -> Separator (resolve current token) then process pipe
//  * Context: Quoted -> stays Quoted (pipe is literal)
//  * State: In_pipe (if unquoted)



//  * REDIRECT ('<', '>'):
//  * ===================
//  * Context: Unquoted -> Separator (resolve current token) then:
//  *   - '<' -> In_redirect (Redirect_In)
//  *   - '>' -> In_redirect (Redirect_Out)
//  *   - '<<' -> In_heredoc (Heredoc)
//  *   - '>>' -> In_append (Append)
//  * Context: Quoted -> stays Quoted (redirect is literal)



//  * QUOTES ('\'', '"'):
//  * ==================
//  * Context: Unquoted -> Quoted
//  *   - '\'' -> In_quote (Single_quotes)
//  *   - '"' -> In_double_quote (Double_quotes)
//  * Context: Quoted -> Unquoted (if matching quote found)
//  * Context: Quoted -> stays Quoted (if non-matching quote)
 


//  * DOLLAR SIGN ('$'):
//  * ==================
//  * Context: Unquoted -> check next chars for parameter name
//  *   - Valid param name -> In_param, Expendable
//  *   - '$?' -> In_param, Expendable (exit status)
//  *   - Invalid -> treat as literal
//  * Context: Single_quotes -> literal '$'
//  * Context: Double_quotes -> check for parameter expansion



//  * REGULAR CHARACTERS (letters, digits, etc.):
//  * ===========================================
//  * Context: Unquoted -> In_literal
//  * Context: Quoted -> continue building quoted token
//  * State: In_literal



//  * NULL TERMINATOR ('\0'):
//  * =======================
//  * Context: Any -> Separator (resolve current token) then EOF
//  * State: In_EOF
 

//  ============================================================================
//   FUNCTION STRUCTURE RECOMMENDATIONS
//  ============================================================================


//  * Main lexing loop structure:
//  * 
//  * while (*lexer->offset)
//  * {
//  *     char current_char = *lexer->offset;
//  *     int old_context = lexer->context;
//  *     
//  *     // Update state based on current character
//  *     set_state(lexer, current_char);
//  *     
//  *     // Update context based on current character and state
//  *     set_context(lexer, current_char);
//  *     
//  *     // If context changed to Separator, resolve and generate token
//  *     if (lexer->context == Separator && old_context != Separator)
//  *     {
//  *         resolve_and_generate_token(lexer, token_start, lexer->offset);
//  *         // Reset for next token
//  *         token_start = lexer->offset;
//  *     }
//  *     
//  *     lexer_advance(lexer);
//  * }
//  * 
//  * // Handle final token if any
//  * if (token_start < lexer->offset)
//  *     resolve_and_generate_token(lexer, token_start, lexer->offset);
//  * 
//  * // Add EOF token
//  * add_token_to_list(lexer, create_token(NULL, End_of_file, No_quotes, Not_expendable));
 

// ============================================================================
//  CRITICAL EDGE CASES TO HANDLE
// ============================================================================


//  1. Unclosed quotes: "hello or 'world"
//     - Should not be interpreted (per subject requirements)

// 2. Empty quotes: "" or ''
//     - Should create empty Word token

// 3. Adjacent tokens: echo"hello"world
//     - Should be treated as single Word token
 
// 4. Multiple redirections: << >> < >
//     - Each should be separate token

// 5. Parameter expansion: $HOME, $?, $123, $invalid
//     - Mark as expendable only if valid

// 6. Mixed quotes: "hello'world'"
//     - Handle quote nesting properly

// 7. Whitespace in quotes: "hello   world"
//     - Preserve all whitespace

// 8. Special characters in quotes: "|<>" in quotes should be literal
 