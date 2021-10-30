TARGET  = ssa
CC      = gcc
AR      = ar
CFLAGS  = -fpic -Iinc -DSSA_PRINT -Wall -pedantic -O3

# Build static and dynamic libraries
.PHONY: build
build: lib/lib${TARGET}.so lib/${TARGET}.a
	@echo "Library build complete."

# Build everything
.PHONY: all
all: bin/ssa_dynamic bin/ssa_static
	@echo "Full build complete."

# Remove build directories
.PHONY: clean
clean:
	@echo "Removing build directories..."
	@rm -rf obj lib bin

lib/libssa.so: obj/ssa.o
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(CC) -shared -o $@ $^

lib/ssa.a: obj/ssa.o
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

bin/ssa_dynamic: obj/main.o lib/libssa.so
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(CC) -o $@ $^

bin/ssa_static: obj/main.o lib/ssa.a
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(CC) -o $@ $^

# Compile a source file into an object file
obj/%.o: src/%.c Makefile
	@echo "Compiling $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
