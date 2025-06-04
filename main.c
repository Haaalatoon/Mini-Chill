#include "lexer.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    const char delimiters[] = ", |";
    char *saveptr = NULL; // Pointer to track the state

    // Get the first token
    char *token = next_token(argv[0], delimiters, &saveptr);

    // Walk through other tokens
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = next_token(NULL, delimiters, &saveptr);
    }

    return 0;
}