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
    PROCESS,
    FILENAME
  } type;

  /**
   * The value of the redirect.
   */
  union {
    char *filename;
  } value;
};

#endif
