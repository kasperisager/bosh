#include "terminal.h"

/**
 * Display a shell prompt and return the user input.
 *
 * @return The string inputted in the prompt.
 */
char *prompt() {
  // Read the configured system maximums for the host name, and
  // login name, and path length to ensure that enough but not
  // too much memory is available.
  unsigned long host_max = sysconf(_SC_HOST_NAME_MAX);
  unsigned long login_max = sysconf(_SC_LOGIN_NAME_MAX);
  unsigned long path_max = pathconf(".", _PC_PATH_MAX);

  // Automatically allocate the required memory, ensuring that
  // it is freed once it goes out of scope.
  char host[host_max];
  char login[login_max];
  char path[path_max];

  gethostname(host, sizeof(host));
  getlogin_r(login, sizeof(login));
  getcwd(path, sizeof(path));

  // Allocate enough memory for the host name, login name, path
  // name, and the 4 additional characters printed.
  char shell[host_max + login_max + path_max + 4];

  // Format the shell prompt that will be displayed prior to
  // user input.
  sprintf(shell, "%s@%s:%s# ", login, host, path);

  char *input = readline(shell);

  // If a non-empty string was inputted, add it to the prompt
  // history so it can easily be accessed again by the user.
  if (*input) add_history(input);

  return input;
}
