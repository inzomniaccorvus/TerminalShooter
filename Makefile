# Compiler
CC = gcc

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Output
TARGET = $(BUILD_DIR)/terminal_shooter.exe

# Source and objects
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.d, $(SRCS))

# Compiler flags
CFLAGS = -I$(INC_DIR) -Wall -Wextra -g

# Linker flags
LDFLAGS = -static -mconsole

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile and generate dependencies
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(TARGET)
