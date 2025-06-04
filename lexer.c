#include "lexer.h"

char *next_token(char *input, const char *delim, char **offset)
{
    char    *ptr;
    char    *start;

    if (input != NULL)
        ptr = input;
    else
        ptr = *offset;
    if (ptr == NULL) // If no more data, return NULL
        return NULL;
    while (*ptr && ft_strchr(delim, *ptr) != NULL) // Skip leading delimiters
        ptr++;
    if (*ptr == '\0') // If we reached the end, return NULL
        return (*offset = NULL, NULL);
    start = ptr; // Mark the start of the token
    while (*ptr && ft_strchr(delim, *ptr) == NULL) // Advance until next delimiter or end of string
        ptr++;
    if (*ptr)
    {
        *ptr = '\0'; // Replace delimiter with null terminator
        *offset = ptr + 1; // Update offset to point to the next character
    }
    else
        *offset = NULL; // If we hit end of string, set offset to NULL
    return (start); // Return the isolated token
}


#include <stdio.h>

int main(int argc, char *argv[]) {

    char    *saveptr = NULL; // Pointer to track the state

    // Get the first token
    char *token = next_token(argv[1], " ", &saveptr);

    // Walk through other tokens
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = next_token(NULL, " ", &saveptr);
    }

    return 0;
}