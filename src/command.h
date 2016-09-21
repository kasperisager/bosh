#ifndef COMMAND
#define COMMAND

#include <stdbool.h>
#include "redirect.h"

/**
 * A single executable command.
 */
struct command {
  /**
   * The program to execute.
   */
  char *program;

  /**
   * The arguments to pass to the program.
   */
  char **arguments;

  /**
   * The redirects to perform.
   */
  struct redirect_list *redirects;

  /**
   * Whether to execute in background.
   */
  bool background;
};

/**
 * A linked list of commands.
 */
struct command_list {
  /**
   * The current command.
   */
  struct command command;

  /**
   * A pointer to the next command.
   */
  struct command_list *next;
};

#endif
