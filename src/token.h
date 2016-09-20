#ifndef TOKEN
#define TOKEN

#include <stdbool.h>
#include <ctype.h>

struct token {
  enum {
    // Operators
    PIPE,
    BG,
    RDIR,
    LDIR,

    // Values
    NAME
  } type;

  union {
    const char *str;
  } value;
};

/**
 * Check if a character is valid in a name.
 *
 * @param c The character to check.
 * @return true if the character is valid in a name.
 */
bool isname(char c);

#endif
