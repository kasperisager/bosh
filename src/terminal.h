#ifndef TERMINAL
#define TERMINAL

#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Display a shell prompt and return the user input.
 *
 * @return The string inputted in the prompt.
 */
char *prompt();

#endif
