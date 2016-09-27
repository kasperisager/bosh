# Bosh

> A simple Unix shell that probably won't blow up your system

Bosh is a minimal Unix shell that allows for basic command interpretation and execution. It supports process communication via pipe redirects (`|`) in addition to file redirects (`>` and `<`) and backgrounding (`&`).

Bosh is written in ~400 lines of clean C with almost as many lines of comments. It was implemented as part of the Operating Systems and C course at the [IT University of Copenhagen](http://www.itu.dk) and turned out to be a most interesting introduction to both the C language and process management in Unix-like systems.

## Installation

Bosh has no external dependencies so assuming that you have `make` and a C compiler available on your system, Bosh can be installed (or uninstalled) using the following command:

```console
$ make [un]install
```

This will compile Bosh and place its binary at `/usr/local/bin/bosh`.

## Architecture

The overall architecture of Bosh is very much inspired by Bash and is split into four major components:

-   __Prompting__  
    As one would expect of a shell, the first step in executing a command is prompting the user for input. Where Bash uses the rather bloated Readline library for handling user input, this step is in Bosh handled by the [Linenoise](https://github.com/antirez/linenoise) library that you'll find included in the `lib/` directory.

-   __Lexical analysis__  
    Once the user has inputted one or more commands, a small, hand-written lexer takes care of splitting the input up into tokens. Each token is described by a `struct` that stores the type and optional value of the token. The result of the lexical analysis is a linked list of these tokens.

-   __Parsing__  
    Once the user input has been split up into tokens, these tokens are then parsed to individual commands and their associated redirects. Each command is like the tokens described by a `struct` that stores the program, arguments, I/O redirects, and backgrounding flag of the command. The result of the parsing is a linked list of these commands.

-   __Execution__  
    The final step is execution of the parsed commands. For each command, the executor takes care of setting up the pipe redirects and file redirects after spawning a separate process that will be used for executing the program specified by the command.

## License

Copyright &copy; 2016 [Kasper Kronborg Isager](https://github.com/kasperisager). Licensed under the terms of the [MIT license](LICENSE.md).
