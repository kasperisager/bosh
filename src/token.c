#include "token.h"

/**
 * Check if a character is valid in a name.
 *
 * @param c The character to check.
 * @return true if the character is valid in a name.
 */
bool isname(char c) {
  return !isspace(c) && (isalnum(c)
    || c == '/'
    || c == '.'
    || c == '-'
    || c == '~'
  );
}
