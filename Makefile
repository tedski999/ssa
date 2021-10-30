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
all: bin/${TARGET}_dynamic bin/${TARGET}_static
	@echo "Full build complete."

# Remove build directories
.PHONY: clean
clean:
	@echo "Removing build directories..."
	@rm -rf obj lib bin

lib/lib${TARGET}.so: obj/${TARGET}.o
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(CC) -shared -o $@ $^

lib/${TARGET}.a: obj/${TARGET}.o
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(AR) rcs $@ $^

bin/${TARGET}_dynamic: obj/main.o lib/lib${TARGET}.so
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(CC) -o $@ $^

bin/${TARGET}_static: obj/main.o lib/${TARGET}.a
	@echo "Linking $@..."
	@mkdir -p $(@D)
	@$(CC) -o $@ $^

# Compile a source file into an object file
obj/%.o: src/%.c Makefile
	@echo "Compiling $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
