cmake_minimum_required(VERSION 3.0)

project(bosh C)

add_library(linenoise
  lib/linenoise.c
  lib/linenoise.h
)

add_executable(bosh
  src/bosh.c
  src/command.h
  src/execute.c
  src/execute.h
  src/lexer.c
  src/lexer.h
  src/parser.c
  src/parser.h
  src/redirect.h
  src/terminal.c
  src/terminal.h
  src/token.c
  src/token.h
)

target_link_libraries(bosh linenoise)

install(TARGETS bosh DESTINATION bin)
