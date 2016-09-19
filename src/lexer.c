#include "lexer.h"

/**
 * Lex a name given an input pointer.
 *
 * @param p The input pointer.
 * @return The resulting name.
 */
const char *name(const char **p) {
  const char *q = *p;

  while (*q && isname(*q)) {
    q++;
  }

  long n = q - *p;
  char *s = strncpy(malloc(n), *p, n);

  *p += n;

  return s;
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
    t->value = NULL;

    if (c) c->next = t;
    else s = t;

    switch (*p) {
      case '|': p++; t->token = PIPE; break;
      case '&': p++; t->token = BG;   break;
      case '>': p++; t->token = RDIR; break;
      case '<': p++; t->token = LDIR; break;
      default:       t->token = NAME;
                     t->value = name(&p);
    }

    c = t;
  }

  return s;
}
