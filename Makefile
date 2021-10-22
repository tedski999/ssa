TARGET  = ssa
CC      = gcc
CFLAGS  = -Wall -pedantic -O3
SRC_FILES := src/main.c src/ssa.c

# Default: Build and run binary
.PHONY: run
run: build
	./bin/${TARGET}

# Build binary
.PHONY: build
build: bin/$(TARGET)
	@echo "Build complete."

# Remove build directories
.PHONY: clean
clean:
	@echo "Removing build directories..."
	@rm -rf obj bin

# Compile a source file into an object file
obj/%.o: src/%.c Makefile
	@echo "Compiling $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

# Link object files into a binary file
bin/$(TARGET): $(SRC_FILES:src/%.c=obj/%.o)
	@echo "Linking $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $^ $(LFLAGS) -o $@
