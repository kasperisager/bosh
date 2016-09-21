#ifndef REDIRECT
#define REDIRECT

/**
 * An I/O redirect between processes.
 */
struct redirect {
  /**
   * The type of the redirect.
   */
  enum {
    // Pipes
    STDIN = 0,
    STDOUT = 1,

    // Files
    FILEIN,
    FILEOUT
  } type;

  /**
   * The value of the redirect.
   */
  union {
    char *filename;
  } value;
};

#endif
