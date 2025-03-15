# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -pthread
LDFLAGS = -pthread

# Directories and files
SRC_DIR = .
BUILD_DIR = build
LIB_NAME = liblogger.a
TEST_APP = test_logger

# Source files
LOGGER_SRC = $(SRC_DIR)/logger.c
MAIN_SRC = $(SRC_DIR)/main.c

# Object files
LOGGER_OBJ = $(BUILD_DIR)/logger.o
MAIN_OBJ = $(BUILD_DIR)/main.o

# Targets
all: $(BUILD_DIR) $(LIB_NAME) $(TEST_APP)

$(BUILD_DIR):
    mkdir -p $(BUILD_DIR)

$(LOGGER_OBJ): $(LOGGER_SRC)
    $(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC)
    $(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME): $(LOGGER_OBJ)
    ar rcs $(BUILD_DIR)/$@ $^

$(TEST_APP): $(MAIN_OBJ) $(BUILD_DIR)/$(LIB_NAME)
    $(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@ $(LDFLAGS)

clean:
    rm -rf $(BUILD_DIR)

run: $(TEST_APP)
    $(BUILD_DIR)/$(TEST_APP)

.PHONY: all clean run