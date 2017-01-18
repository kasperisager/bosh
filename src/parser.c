#include "parser.h"

/**
 * Parse command arguments given a token list.
 *
 * @param ts The token list
 * @return A pointer to the command arguments.
 */
static char **parse_arguments(struct token_list **ts) {
  struct token_list *t = *ts;

  // Keep track of the number of arguments that have been parsed.
  // The initial value is set at 2 in order to make room for two
  // required slots: The name of the program and a terminating
  // null pointer.
  int n = 2, i;

  while (t && t->token.type == NAME) {
    n++; t = t->next;
  }

  // Dynamically allocate the array of arguments.
  char **a = calloc(n, sizeof(char *));

  for (i = 1; i < n - 1; i++) {
    // Add the next argument to the array and grab the next
    // token in the list.
    a[i] = (*ts)->token.value.str;
    *ts = (*ts)->next;
  }

  // Make the final item of the array the null pointer to
  // indicate that the array has come to an end.
  a[n - 1] = NULL;

  return a;
}

/**
 * Parse a list of tokens to a list of commands.
 *
 * @param tokens The tokens to parse.
 * @return A pointer to the list of commands.
 */
struct command_list *parse(struct token_list *ts) {
  // Keep track of the start node (s) of the command list and the
  // current node (c). The start node will be returned to the
  // caller and the current node will be used for appending new
  // nodes to the list.
  struct command_list *s = NULL;
  struct command_list *c = NULL;

  // Keep track of the default input for a given parsed command.
  // This will be set whenever pipes are encountered.
  struct redirect *in = NULL;

  while (ts) {
    // Each command must start with a name token in order for the
    // command to make sense. If this is not the case, return the
    // null pointer to indicate an error during parsing.
    if (ts->token.type != NAME) {
      fprintf(stderr, "bosh: expected command name\n");
      return NULL;
    }

    // Dynamically allocate a new node in the command list and
    // initialize its next pointer to the null pointer.
    struct command_list *n = calloc(1, sizeof(struct command_list));
    n->next = NULL;

    // Append the new node to the current node, increasing the
    // size of the list, or make the new node the start node
    // if its the very first node.
    if (c) c->next = n;
    else s = n;

    // Move the current node pointer to the new node.
    c = n;

    // Set the program to run as the value of the current token
    // and flag that the program should run in the foreground by
    // default.
    n->command.program = ts->token.value.str;
    n->command.background = false;
    n->command.arguments = NULL;

    // Initialize the command I/O redirects. The input will the
    // output of a previous process if piped, other the null
    // pointer.
    n->command.out = NULL;
    if (in) {
      n->command.in = calloc(1, sizeof(struct redirect));

      // Copy the previous redirect to the newly allocated memory.
      // This ensures that each command can be freed independently
      // from each other. For instance, if two commands share a
      // redirect and this redirect was freed from the first
      // command, the second command wouldn't known whether or not
      // it would be safe to free its own redirect.
      memcpy(n->command.in, in, sizeof(struct redirect));
    } else {
      n->command.in = NULL;
    }

    in = NULL;
    ts = ts->next;

    // Parse the arguments to pass to the program and set the
    // first argument to the program that should be run. This is
    // a convention used by the exec family of system calls.
    n->command.arguments = parse_arguments(&ts);
    n->command.arguments[0] = n->command.program;

    // If there are no tokens left to parse, bail out.
    if (!ts) break;

    // Before any other cases are covered, check for input
    // redirection.
    if (ts->token.type == LDIR) {
      if (n->command.in) {
        fprintf(stderr, "bosh: input already redirected\n");
        return NULL;
      }

      if (!ts->next || ts->next->token.type != NAME) {
        fprintf(stderr, "bosh: expected redirect name\n");
        return NULL;
      }

      ts = ts->next;

      n->command.in = calloc(1, sizeof(struct redirect));
      n->command.in->type = FILENAME;
      n->command.in->value.filename = ts->token.value.str;

      ts = ts->next;

      if (!ts) break;
    }

    switch (ts->token.type) {
      // Move on to next command if an end of statement token
      // is encountered.
      case EOS: break;

      // Flag that the command should be run in the background
      // if the background token is encountered.
      case BG: n->command.background = true; break;

      case RDIR:
        if (!ts->next || ts->next->token.type != NAME) {
          fprintf(stderr, "bosh: expected redirect name\n");
          return NULL;
        }

        ts = ts->next;

        n->command.out = calloc(1, sizeof(struct redirect));
        n->command.out->type = FILENAME;
        n->command.out->value.filename = ts->token.value.str;

        // If the next token is present and is not an end of
        // statement, return the null pointer to indicate an
        // error during parsing.
        if (ts->next && ts->next->token.type != EOS) {
          fprintf(stderr, "bosh: expected end of statement\n");
          return NULL;
        } else break;

      case PIPE:
        n->command.out = calloc(1, sizeof(struct redirect));
        n->command.out->type = PROCESS;

        // Remember the output of this command and use it as the
        // input of the next command.
        in = n->command.out;

        // If the next token is not a name, i.e. a process that
        // can be piped to, return the null pointer to indicate
        // an error during parsing.
        if (!ts->next || ts->next->token.type != NAME) {
          fprintf(stderr, "bosh: expected command name\n");
          return NULL;
        } else break;

      // If an invalid token is found, i.e. one not recognized
      // by the parser, return the null pointer to indicate an
      // error during parsing.
      default:
        fprintf(stderr, "bosh: unexpected token\n");
        return NULL;
    }

    ts = ts->next;
  }

  return s;
}
