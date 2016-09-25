#ifndef TERMINAL
#define TERMINAL

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include "../lib/linenoise.h"

/**
 * Display a shell prompt and return the user input.
 *
 * @return The string inputted in the prompt.
 */
char *prompt();

#endif
