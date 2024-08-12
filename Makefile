# Default to Linux
LINUX_CC = clang
LINUX_CFLAGS = -Wall -g -I./include -I./vendor/glad/include -I./vendor
LINUX_LDFLAGS = -lGL -lglfw -lm
LINUX_TARGET = build/linux/flan

# Windows cross-compilation settings
WIN_CC = ~/llvm-mingw/llvm/bin/x86_64-w64-mingw32-clang
WIN_CFLAGS = -Wall -g -I/home/nnmqq/winlibs/glfw/include/ -I./include -I./vendor/glad/include -I./vendor
WIN_LDFLAGS = -L/home/nnmqq/winlibs/glfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -lm
WIN_TARGET = build/windows/flan.exe

# Debug flags
ifeq ($(DEBUG),1)
    LINUX_CFLAGS += -O0
    WIN_CFLAGS += -O0
else
    LINUX_CFLAGS += -O3
    WIN_CFLAGS += -O3
endif

# Source files and object files
SRCS = $(wildcard src/*.c)
LINUX_OBJS = $(SRCS:src/%.c=build/linux/%.o) build/linux/glad.o
WIN_OBJS = $(SRCS:src/%.c=build/windows/%.o) build/windows/glad.o

# Default target
all: linux windows

# Linux target
linux: $(LINUX_TARGET)
	@echo "Compiled for Linux"

$(LINUX_TARGET): $(LINUX_OBJS)
	$(LINUX_CC) -o $@ $^ $(LINUX_LDFLAGS)

# Windows target
windows: $(WIN_TARGET)
	@echo "Compiled for Windows"

$(WIN_TARGET): $(WIN_OBJS)
	$(WIN_CC) -o $@ $^ $(WIN_LDFLAGS)

# Run commands
rl: $(LINUX_TARGET)
	@echo "Running Linux executable..."
	./$(LINUX_TARGET)

rw: $(WIN_TARGET)
	@echo "Running Windows executable..."
	$(WIN_TARGET)

# Create directories for object files
build/linux:
	mkdir -p build/linux

build/windows:
	mkdir -p build/windows

# Compile object files for Linux
$(LINUX_OBJS): | build/linux

build/linux/%.o: src/%.c
	$(LINUX_CC) $(LINUX_CFLAGS) -c $< -o $@

# Compile object files for Windows
$(WIN_OBJS): | build/windows

build/windows/%.o: src/%.c
	$(WIN_CC) $(WIN_CFLAGS) -c $< -o $@

# Compile glad.c for Linux
build/linux/glad.o: vendor/glad/src/glad.c
	$(LINUX_CC) $(LINUX_CFLAGS) -c $< -o $@

# Compile glad.c for Windows
build/windows/glad.o: vendor/glad/src/glad.c
	$(WIN_CC) $(WIN_CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(LINUX_OBJS) $(WIN_OBJS) $(LINUX_TARGET) $(WIN_TARGET)
	rm -rf build

.PHONY: clean linux windows all rl rw
