#include "token.h"

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
