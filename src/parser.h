#ifndef PARSER
#define PARSER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "redirect.h"
#include "command.h"

/**
 * Parse a list of tokens to a list of commands.
 *
 * @param tokens The tokens to parse.
 * @return A pointer to the list of commands.
 */
struct command_list *parse(struct token_list *tokens);

#endif
