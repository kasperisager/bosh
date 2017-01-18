#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "terminal.h"
#include "lexer.h"
#include "expand.h"
#include "parser.h"
#include "execute.h"

int main() {
  signal(SIGINT, terminate);

  while (true) {
    char *input = prompt();

    if (!input) {
      continue;
    }

    struct token_list *ts = lex(input);

    expand(ts);

    struct command_list *cs = parse(ts);

    execute(cs);

    free(input);

    while (ts) {
      struct token t = ts->token;

      switch (t.type) {
        case NAME:
          free(t.value.str);
          break;
        default:
          break;
      }

      struct token_list *tc = ts;
      ts = ts->next;
      free(tc);
    }

    while (cs) {
      struct command c = cs->command;

      if (c.arguments) free(c.arguments);
      if (c.in) free(c.in);
      if (c.out) free(c.out);

      struct command_list *cc = cs;
      cs = cs->next;
      free(cc);
    }
  }

  return 0;
}
