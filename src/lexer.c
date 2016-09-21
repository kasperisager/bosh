#include "lexer.h"

/**
 * Lex a name given an input pointer.
 *
 * @param p The input pointer.
 * @return The resulting name.
 */
char *lex_name(char **p) {
  char *q = *p;

  while (*q && isname(*q)) {
    q++;
  }

  // Compute the number of characters to copy as the difference
  // between the pointer q and the string pointed to by the
  // input pointer. This can be illustrated as follows:
  //
  //   /foo/bar.txt
  //   ^           ^
  //  *p = 0       q = 12
  //
  // In this case, n will equal 12 as q - *p = 12.
  long n = q - *p;

  // Copy n characters from the string pointed to by the input
  // pointer to a freshly allocated block of memory.
  char *s = strncpy(malloc(n), *p, n);

  // Forward the string pointed to by the input pointer by the n
  // characters that have just been lexed.
  *p += n;

  return s;
}

/**
 * Lex an input string to a list of tokens.
 *
 * @param input The input string to lex.
 * @return A pointer to the list of tokens.
 */
struct token_list *lex(char *p) {
  struct token_list *s = NULL;
  struct token_list *c = NULL;

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
      t->token.value.str = lex_name(&p);
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
