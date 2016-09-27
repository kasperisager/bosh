#include "execute.h"

/**
 * Keep track of the currently active PID.
 */
static volatile int pid = 0;

/**
 * Terminate the process currently executed.
 */
void terminate() {
  if (pid != 0) kill(pid, SIGTERM);
}

/**
 * Spawn a new process for a command with the given input and
 * output file descriptors.
 *
 * @param c The command to spawn a process for.
 * @param i The input file descriptor.
 * @param o The output file descriptor.
 * @return The spawned process ID.
 */
int spawn(struct command c, int i, int o) {
  int pid = fork();

  if (pid == 0) {
    // If input should not be redirected to stdin, close it and
    // replace it with the corresponding input file descriptor.
    if (i != STDIN_FILENO) {
      close(STDIN_FILENO);
      dup(i);
    }

    // If output should not be redirected to stdout, close it and
    // replace it with the corresponding output file descriptor.
    if (o != STDOUT_FILENO) {
      close(STDOUT_FILENO);
      dup(o);
    }

    // Execute the program with the corresponding arguments. This
    // call will replace the entire running process if successful.
    execvp(c.program, c.arguments);

    // If the above call returns then an error occurred. Check
    // the global error number contant to figure out where the
    // command went haywire.
    switch (errno) {
      case EACCES:
        fprintf(stderr, "bosh: %s: permission denied\n", c.program);
        break;

      case ENOENT:
        fprintf(stderr, "bosh: %s: command not found\n", c.program);
        break;
    }

    exit(1);
  }

  return pid;
}

/**
 * Execute a list of commands.
 *
 * @param commands The commands to execute.
 */
void execute(struct command_list *cs) {
  // Keep track of the input file descriptor (i), output file
  // descriptor (o), pipe redirection file descriptor (r), and
  // pipe file descriptor (p).
  int i, o, r, p[2];

  for (; cs; cs = cs->next) {
    // Built in command: exit [code]
    if (strcmp("exit", cs->command.program) == 0) {
      char *code = cs->command.arguments[1];

      // Ignore the SIGTERM signal in the current process
      // so it doesn't get killed before a clean exit can
      // be performed.
      signal(SIGTERM, SIG_IGN);

      // Send a SIGTERM to the current process group to
      // initiate a clean termination of background
      // processes that might have been started.
      kill(0, SIGTERM);

      // Since the current process ignored the SIGTERM,
      // it can now perform a clean exit.
      exit(code ? atoi(code) : 0);
    }

    // Built in command: cd [dir]
    if (strcmp("cd", cs->command.program) == 0) {
      char *dir = cs->command.arguments[1];

      // If a directory was specified, change into it. If
      // not, change to the home directory.
      chdir(dir ? dir : getenv("HOME"));

      continue;
    }

    // Create a new pipe to be used for the current command if
    // needed. The write end of the pipe can be used for the
    // current command and the read end passed on via the pipe
    // redirection file descriptor to the next command.
    pipe(p);

    // Check if the command must grab it input from somewhere
    // that isn't stdin.
    if (cs->command.in) {
      switch (cs->command.in->type) {
        // If the input should be redirected from a filename,
        // open up the associated file in read-only mode.
        case FILENAME:
          i = open(cs->command.in->value.filename,
            O_RDONLY
          );
          break;

        // If the input should be redirected from a process,
        // grab the stored read end of the previous pipe.
        case PROCESS:
          i = r;
          break;
      }
    // Otheriwse, just redirect the input from stdin.
    } else i = STDIN_FILENO;

    if (cs->command.out) {
      switch (cs->command.out->type) {
        // If the output should be redirected to a filename,
        // open up the associated file in write-only mode. If
        // the file already exists, truncate it. If it however
        // doesn't already exists, create it.
        case FILENAME:
          o = open(cs->command.out->value.filename,
            O_WRONLY | O_TRUNC | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
          );
          break;

        // If the output should be redirected to a process,
        // grab the write end of the current pipe and use it
        // as input and store the read end for the next process.
        case PROCESS:
          o = p[1];
          r = p[0];
          break;
      }
    // Otheriwse, just redirect the output to stdout.
    } else o = STDOUT_FILENO;

    // Spawn a new process for the command and pass along the
    // corresponding input and output file descriptors.
    pid = spawn(cs->command, i, o);

    // Close the write end of the pipe as only the read end is
    // needed for the next command if piped.
    close(p[1]);

    // If the command is a background job, set the no-hang
    // option for the waitpid call. This ensures that the call
    // doesn't block, but still reaps the child process when
    // finished.
    waitpid(pid, NULL, cs->command.background ? WNOHANG : 0);
  }
}
