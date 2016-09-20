#ifndef LEXER
#define LEXER

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

/**
 * Lex an input string to a list of tokens.
 *
 * @param input The input string to lex.
 * @return A pointer to the list of tokens.
 */
struct token_list *lex(const char *input);

#endif
