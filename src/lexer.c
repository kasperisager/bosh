#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

/**
 * Check if a character is a reserved symbol.
 *
 * @param c The character to check.
 * @return true if the character is a reserved symbol.
 */
bool issymbol(char c) {
  return c == PIPE
      || c == BG
      || c == RDIR
      || c == LDIR;
}

/**
 * Construct an identifier given an input pointer.
 *
 * @param p The input pointer.
 * @return The resulting identifier.
 */
const char *identifier(const char **p) {
  const char *q = *p;

  while (*q && !isspace(*q) && !issymbol(*q)) {
    q++;
  }

  long n = q - *p;
  char *s = strncpy(malloc(n), *p, n);

  *p += n;

  return s;
}

/**
 * Produce a token value given an input pointer.
 *
 * @param p The input pointer.
 * @return The resulting production.
 */
const char *production(const char **p) {
  switch (**p) {
    case PIPE: (*p)++; return "|";
    case BG:   (*p)++; return "&";
    case RDIR: (*p)++; return ">";
    case LDIR: (*p)++; return "<";
    default:           return identifier(&*p);
  }
}

/**
 * Lex an input string to a list of tokens.
 *
 * @param input The input string to lex.
 * @return A pointer to the list of tokens.
 */
struct tokens *lex(const char *input) {
  struct tokens *s = NULL;
  struct tokens *c = NULL;

  const char *p = input;

  while (*p) {
    if (isspace(*p)) {
      p++; continue;
    }

    struct tokens *t = malloc(sizeof(struct tokens));
    t->next  = NULL;
    t->token = production(&p);

    if (c) c->next = t;
    else s = t;

    c = t;
  }

  return s;
}
