# Paths
TEST_DIR := test
OUT_DIR := build
TMP_DIR := tmp
SRC_DIR := src
HDR_DIR := headers

# Compiler
CC := gcc
CFLAGS := -Wall -Wextra -O2 -I$(HDR_DIR)

# Automatically find test sources
TESTS := $(wildcard $(TEST_DIR)/*.c)
EXECUTABLES := $(patsubst $(TEST_DIR)/%.c, $(OUT_DIR)/%, $(TESTS))

# Automatically find shared .c from headers
HEADERS := $(wildcard $(HDR_DIR)/*.h)
SHARED_SRC := $(patsubst $(HDR_DIR)/%.h, $(SRC_DIR)/%/%.c, $(HEADERS))
SHARED_OBJ := $(patsubst $(SRC_DIR)/%/%.c, $(TMP_DIR)/%.o, $(SHARED_SRC))

# Create temp folders
$(OUT_DIR):
	@echo Create output directory.
	mkdir -p $@

$(TMP_DIR):
	@echo Create temporal directory.
	mkdir -p $@

# Default target
all: $(EXECUTABLES) $(SHARED_OBJ)
	@echo All files are compiled.x
	@echo Clear temporal directory.
	rm -rf $(TMP_DIR)/*
	rm -R $(TMP_DIR)

# Compile shared object files
$(TMP_DIR)/%.o: $(SHARED_SRC) | $(TMP_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile each test with shared objects
$(OUT_DIR)/%: $(TEST_DIR)/%.c $(SHARED_OBJ) | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(SHARED_OBJ) $(SRC_DIR)/global.c -L $(SRC_DIR)/global.h

# Clean
clean:
	@echo Clear output directory.
	rm -rf $(OUT_DIR)/*
	rm -R $(OUT_DIR)