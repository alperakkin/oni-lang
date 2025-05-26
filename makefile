# Derleyici ve bayraklar
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Dizinler
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = build


SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/oni


all: $(TARGET)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(TARGET): $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
