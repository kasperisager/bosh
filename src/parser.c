#include "parser.h"

/**
 * Parse command arguments given a token list.
 *
 * @param ts The token list
 * @return A pointer to the command arguments.
 */
char **parse_arguments(struct token_list **ts) {
  // Keep track of the number of arguments that have been parsed.
  // The initial value is set at 2 in order to make room for two
  // required slots: The name of the program and a terminating
  // null pointer.
  int n = 2;

  // Dynamically allocate the initial array of arguments.
  char **a = malloc(n * sizeof(char *));

  while (*ts && (*ts)->token.type == NAME) {
    // Reallocate the array with memory enough for one more
    // argument. This may or may not cause the existing
    // arguments to be copied to the new memory block depending
    // on whether or not there's continous memory available.
    a = realloc(a, (n + 1) * sizeof(char *));

    // Add the next argument to the array and grab the next
    // token in the list.
    a[n++ - 1] = (*ts)->token.value.str;
    *ts = (*ts)->next;
  }

  // Make the final item of the array the null pointer to
  // indicate that the array has come to an end.
  a[n] = NULL;

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

  while (ts) {
    // Each command must start with a name token in order for the
    // command to make sense. If this is not the case, return the
    // null pointer to indicate an error during parsing.
    if (ts->token.type != NAME) {
      return NULL;
    }

    // Dynamically allocate a new node in the command list and
    // initialize its next pointer to the null pointer.
    struct command_list *n = malloc(sizeof(struct command_list));
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

    ts = ts->next;

    // Parse the arguments to pass to the program and set the
    // first argument to the program that should be run. This is
    // a convention used by the exec family of system calls.
    n->command.arguments = parse_arguments(&ts);
    n->command.arguments[0] = n->command.program;

    // If there are no tokens left to parse, bail out.
    if (!ts) break;

    switch (ts->token.type) {
      // Move on to next command if an end of statement token
      // is encountered.
      case EOS: break;

      // Flag that the command should be run in the background
      // if the background token is encountered.
      case BG: n->command.background = true; break;

      case PIPE:
      case RDIR:
      case LDIR:
        break;

      // If an invalid token is found, i.e. one not recognized
      // by the parser, return the null pointer to indicate an
      // error during parsing.
      default: return NULL;
    }

    ts = ts->next;
  }

  return s;
}
