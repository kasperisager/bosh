#ifndef TERMINAL
#define TERMINAL

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Display a shell prompt and return the user input.
 *
 * @return The string inputted in the prompt.
 */
char *prompt();

#endif
