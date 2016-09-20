#ifndef TOKEN
#define TOKEN

#include <stdbool.h>
#include <ctype.h>

/**
 * A single lexical token.
 */
struct token {
  /**
   * The type of the token.
   */
  enum {
    // Operators
    PIPE,
    BG,
    RDIR,
    LDIR,

    // Values
    NAME
  } type;

  /**
   * The value of the token.
   */
  union {
    const char *str;
  } value;
};

/**
 * A linked list of tokens.
 */
struct token_list {
  /**
   * The current token.
   */
  struct token token;

  /**
   * A pointer to the next token.
   */
  struct token_list *next;
};

/**
 * Check if a character is valid in a name.
 *
 * @param c The character to check.
 * @return true if the character is valid in a name.
 */
bool isname(char c);

#endif
