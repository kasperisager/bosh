bin/bosh: LIBS += linenoise
bin/bosh: \
	obj/bosh.o \
	obj/execute.o \
	obj/lexer.o \
	obj/parser.o \
	obj/terminal.o \
	obj/token.o \
	arc/liblinenoise.a

install: bin/bosh
	@mv bin/bosh /usr/local/bin/
	@echo "installed at /usr/local/bin/bosh"

uninstall:
	@rm -i /usr/local/bin/bosh
