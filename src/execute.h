#ifndef EXECUTE
#define EXECUTE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include "command.h"

/**
 * Terminate the process currently executed.
 */
void terminate();

/**
 * Execute a list of commands.
 *
 * @param commands The commands to execute.
 */
void execute(struct command_list *commands);

#endif
