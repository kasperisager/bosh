#include "expand.h"

/**
 * Expand a list of tokens in-place.
 *
 * @param tokens The tokens to expand.
 */
void expand(struct token_list *ts) {
  for (; ts; ts = ts->next) {
    if (ts->token.type != NAME) continue;

    char *s = ts->token.value.str;

    int n = strlen(s);

    switch (*s) {
      case '~': {
        if (*(s + 1) != '/') break;

        char e[n];

        strncpy(e, s + 1, n);

        char *h = getpwuid(getuid())->pw_dir;

        free(s);

        n = strlen(h) + n - 1;
        s = calloc(n + 1, sizeof(char));

        strcat(s, h);
        strcat(s, e);
      } break;
    }

    ts->token.value.str = s;
  }
}
