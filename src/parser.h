#ifndef PARSER
#define PARSER

#include <stdlib.h>
#include "token.h"
#include "command.h"

/**
 * Parse a list of tokens to a list of commands.
 *
 * @param tokens The tokens to parse.
 * @return A pointer to the list of commands.
 */
struct command_list *parse(struct token_list *tokens);

#endif
