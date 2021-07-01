CC = gcc

# Output directories
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Final executable
TARGET := $(BIN_DIR)/chiton

# Source files
SRC := $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Flags
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS := -g -Wall
LDFLAGS := -Llib
LDLIBS := -lm

# Targets
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)