#ifndef LEXER
#define LEXER

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

enum token {
  // Symbols
  PIPE = '|',
  BG   = '&',
  RDIR = '>',
  LDIR = '<'
};

/**
 * A linked list of tokens.
 */
struct tokens {
  /**
   * The value of the current token.
   */
  const char *token;

  /**
   * A pointer to the next token.
   */
  struct tokens *next;
};

/**
 * Lex an input string to a list of tokens.
 *
 * @param input The input string to lex.
 * @return A pointer to the list of tokens.
 */
struct tokens *lex(const char *input);

#endif
