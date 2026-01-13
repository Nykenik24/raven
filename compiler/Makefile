CC      := cc
CFLAGS  := -Wall -Wextra -std=c99 -Iinclude -MMD -MP
LDFLAGS :=

SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/compiler
TARGET_DEBUG := $(BUILD_DIR)/compiler_debug

SRC := $(shell find $(SRC_DIR) -type f -name '*.c')
DEP := $(OBJ:.o=.d)

OBJ_RELEASE := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/release/%.o,$(SRC))
OBJ_DEBUG   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/debug/%.o,$(SRC))

all: $(TARGET) $(TARGET_DEBUG)

$(TARGET): $(OBJ_RELEASE)
	$(CC) $^ -o $@

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $^ -o $@

$(BUILD_DIR)/release/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/debug/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DRAVEN_SHOW_DEBUG_OUTPUT -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)
	rm -rf compile_commands.json

run:
	$(TARGET)

.PHONY: all clean

-include $(DEP)


