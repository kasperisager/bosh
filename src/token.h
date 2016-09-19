#ifndef TOKEN
#define TOKEN

#include <stdbool.h>
#include <ctype.h>

enum token {
  // Operators
  PIPE,
  BG,
  RDIR,
  LDIR,

  // Values
  NAME,

  // Status
  ILLEGAL
};

/**
 * Check if a character is valid in a name.
 *
 * @param c The character to check.
 * @return true if the character is valid in a name.
 */
bool isname(char c);

#endif
