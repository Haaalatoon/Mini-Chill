#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


/*
 * next_token:
 *   A reentrant, in‐place tokenizer. 
 *   Overwrites delimiters in the input buffer with '\0'.
 */
char *next_token(char *str, const char *delims, char **saveptr)
{
    char *p;
    char *start;

    if (str != NULL) {
        p = str;
    } else {
        p = *saveptr;
    }

    /* If there’s no more data, bail out. */
    if (p == NULL)
        return NULL;

    /* 1) Skip leading delimiters */
    while (*p && strchr(delims, *p) != NULL)
        p++;

    /* If we reached the end, no more tokens. */
    if (*p == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    /* 2) Mark this as the beginning of the token */
    start = p;

    /* 3) Advance until next delimiter or end‐of‐string */
    while (*p && strchr(delims, *p) == NULL)
        p++;

    /* 4) If we found a delimiter, replace it with '\0' and set saveptr */
    if (*p) {
        *p = '\0';
        *saveptr = p + 1;
    } else {
        /* We hit end‐of‐string. No more tokens after this. */
        *saveptr = NULL;
    }

    /* 5) Return the token we just isolated (in‐place) */
    return start;
}

int main() {
    char str[] = "Hello, | |World,from|C";
    const char delimiters[] = ", |";
    char *saveptr = NULL; // Pointer to track the state

    // Get the first token
    char *token = next_token(str, delimiters, &saveptr);

    // Walk through other tokens
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = next_token(NULL, delimiters, &saveptr);
    }

    return 0;
}