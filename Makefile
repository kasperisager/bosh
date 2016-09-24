# Compile a dependency file for a given source file
dep/%.d: src/%.c
	@mkdir -p dep
	${CC} -MM -MF $@ -MT $(@:dep/%.d=obj/%.o) $<

# Compile an object file based on a dependency file
obj/%.o: dep/%.d
	@mkdir -p obj
	${CC} -o $@ -c $(@:obj/%.o=src/%.c)

# Create a binary with zero or more libraries linked
bin/%:
	@mkdir -p bin
	${CC} -o $@ $^ $(LIBS:%=-l%)

# Remove all generated files
clean:
	rm -rf {dep,obj,bin}

# Generate and include dependency files for sources
SRC = $(wildcard src/*.c)
DEP = $(patsubst src/%.c, dep/%.d, ${SRC})
-include $(DEP)
