#include "lexer.h"

/**
 * Lex a name given an input pointer.
 *
 * @param p The input pointer.
 * @return The resulting name.
 */
char *name(char **p) {
  char *q = *p;

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
struct token_list *lex(char *input) {
  struct token_list *s = NULL;
  struct token_list *c = NULL;

  char *p = input;

  while (*p) {
    if (isspace(*p)) {
      p++; continue;
    }

    struct token_list *t = malloc(sizeof(struct token_list));
    t->next = NULL;

    if (c) c->next = t;
    else s = t;

    c = t;

    if (isname(*p)) {
      t->token.type = NAME;
      t->token.value.str = name(&p);
      continue;
    }

    switch (*p) {
      case '|': p++; t->token.type = PIPE; break;
      case '&': p++; t->token.type = BG;   break;
      case '>': p++; t->token.type = RDIR; break;
      case '<': p++; t->token.type = LDIR; break;
      case ';': p++; t->token.type = EOS;  break;
      default: return NULL;
    }
  }

  return s;
}
