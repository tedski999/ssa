TARGET  := ssa
CC      := gcc
AR      := ar
BUILD   := release
OBJ_DIR := build/${BUILD}
LIB_DIR := lib/${BUILD}

override CFLAGS += -fpic -Iinclude -Wall -pedantic
override LFLAGS +=
ifeq (${BUILD}, release)
override CFLAGS += -O3
else ifeq (${BUILD}, debug)
override CFLAGS += -g -Og -DSSA_DEBUG
else
$(error Unrecognised build type '${BUILD}')
endif

# Build static and dynamic library build
.PHONY: build
build: ${LIB_DIR}/lib${TARGET}.so ${LIB_DIR}/${TARGET}.a
	@echo -e "\n# Library ${BUILD} build complete."

# Build debug library build
.PHONY: debug
debug:
	@make --no-print-directory BUILD="debug"

# Build everything
.PHONY: all
all: build debug \
	bin/tests/check \
	bin/examples/linkedlist \
	bin/examples/strings \
	bin/examples/random
	@echo -e "\n# Full build complete."

# Build and run unit tests
.PHONY: check
check: bin/tests/check
	@echo -e "\n# Running unit tests..."
	./bin/tests/check

# Remove build directories
.PHONY: clean
clean:
	@echo -e "\n# Removing build files..."
	rm -rf build lib bin

# Create dynamic library
${LIB_DIR}/lib${TARGET}.so: ${OBJ_DIR}/src/${TARGET}.o
	@echo -e "\n# Linking as dynamic library $@..."
	@mkdir -p $(@D)
	$(CC) -shared -o $@ $^

# Create static library
${LIB_DIR}/${TARGET}.a: ${OBJ_DIR}/src/${TARGET}.o
	@echo -e "\n# Linking as static library $@..."
	@mkdir -p $(@D)
	$(AR) rcs $@ $^

# Link binary file from object files
bin/%: ${OBJ_DIR}/%.o ${LIB_DIR}/lib${TARGET}.so
	@echo -e "\n# Linking $@..."
	@mkdir -p $(@D)
	$(CC) -o $@ $^

# Compile a source file into an object file
.PRECIOUS: ${OBJ_DIR}/%.o
${OBJ_DIR}/%.o: %.c Makefile
	@echo -e "\n# Compiling $< -> $@"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
