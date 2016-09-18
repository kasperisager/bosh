#ifndef TOKEN
#define TOKEN

#include <stdbool.h>

enum token {
  // Symbols
  PIPE = '|',
  BG   = '&',
  RDIR = '>',
  LDIR = '<'
};

/**
 * Check if a character is a reserved symbol.
 *
 * @param c The character to check.
 * @return true if the character is a reserved symbol.
 */
bool issymbol(char c);

#endif
