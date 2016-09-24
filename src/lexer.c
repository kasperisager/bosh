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
  char *s = strncpy(malloc((n + 1) * sizeof(char)), *p, n);

  // Terminate the copied string.
  s[n] = '\0';

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
  // Keep track of the start node (s) of the token list and the
  // current node (c). The start node will be returned to the
  // caller and the current node will be used for appending new
  // nodes to the list.
  struct token_list *s = NULL;
  struct token_list *c = NULL;

  while (*p) {
    if (isspace(*p)) {
      p++; continue;
    }

    // Dynamically allocate a new node in the token list and
    // initialize its next pointer to the null pointer.
    struct token_list *t = malloc(sizeof(struct token_list));
    t->next = NULL;

    // Append the new node to the current node, increasing the
    // size of the list, or make the new node the start node
    // if its the very first node.
    if (c) c->next = t;
    else s = t;

    // Move the current node pointer to the new node.
    c = t;

    if (isname(*p)) {
      t->token.type = NAME;
      t->token.value.str = lex_name(&p);
      continue;
    }

    switch (*p) {
      case '|': t->token.type = PIPE; break;
      case '&': t->token.type = BG;   break;
      case '>': t->token.type = RDIR; break;
      case '<': t->token.type = LDIR; break;
      case ';': t->token.type = EOS;  break;
      // If an invalid token is found, i.e. one not recognized
      // by the lexer, return the null pointer to indicate an
      // error during lexing.
      default:
        fprintf(stderr, "bosh: unexpected character \"%c\"\n", *p);
        return NULL;
    }

    p++;
  }

  return s;
}
