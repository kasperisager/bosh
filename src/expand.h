#ifndef EXPAND
#define EXPAND

#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include "token.h"

/**
 * Expand a list of tokens in-place.
 *
 * @param tokens The tokens to expand.
 */
void expand(struct token_list *tokens);

#endif
