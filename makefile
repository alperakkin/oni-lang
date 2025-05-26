
SRC_DIR := src
LIB_DIR := $(SRC_DIR)/lib
INC_DIR := $(SRC_DIR)/include
OBJ_DIR := build
BIN_DIR := bin

CC := gcc
CFLAGS := -Wall -Wextra -O2 -I$(INC_DIR)

SRCS := $(shell find $(SRC_DIR) -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TARGET := $(BIN_DIR)/oni

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(TARGET): $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
