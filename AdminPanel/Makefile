# Makefile for Game Rating Admin Panel
# Compatible with Windows (MinGW), Linux, and macOS


# Detect OS
UNAME_S := $(shell uname -s)


# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2


# JSON include path (добавлено)
JSON_INC = -I./include


# OS-specific include and lib paths
ifeq ($(UNAME_S),Linux)
    # Linux paths
    INC_DIR = -I/usr/include/mysql
    LIB_DIR = -L/usr/lib/x86_64-linux-gnu
    LIBS = -lmysqlclient
endif


ifeq ($(UNAME_S),Darwin)
    # macOS paths (Homebrew)
    INC_DIR = -I/usr/local/opt/mysql-client/include
    LIB_DIR = -L/usr/local/opt/mysql-client/lib
    LIBS = -lmysqlclient
endif


ifeq ($(OS),Windows_NT)
    # Windows (MinGW) paths - ADJUST PATHS IF NEEDED
    INC_DIR = -I"C:/Program Files/MySQL/MySQL Server 8.0/include" 
    LIB_DIR = -L"C:/Program Files/MySQL/MySQL Server 8.0/lib" 
    LIBS = -lmysql
    RM_CMD = del /Q
    EXE_EXT = .exe
else
    RM_CMD = rm -f
    EXE_EXT = 
endif


SOURCES = src/main.cpp \
          src/DatabaseConnection.cpp \
          src/User.cpp \
          src/Game.cpp \
          src/Review.cpp \
          src/Platform.cpp \
          src/AdminPanel.cpp


# Object files
OBJECTS = $(SOURCES:.cpp=.o)


# Output executable
TARGET = admin_panel$(EXE_EXT)


# Default target
all: $(TARGET)


# Build the executable
$(TARGET): $(OBJECTS)
	@echo [LINK] Linking object files to $(TARGET)...
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIB_DIR) $(LIBS)
	@echo [SUCCESS] Build complete! Run with: ./$(TARGET)


# Compile source files to object files (обновлено с JSON_INC)
src/%.o: src/%.cpp
	@echo [COMPILE] Compiling $<...
	$(CXX) $(CXXFLAGS) $(JSON_INC) $(INC_DIR) -c $< -o $@


# Clean build files
clean:
	@echo [CLEAN] Removing object files and executable...
	$(RM_CMD) $(OBJECTS)
	$(RM_CMD) $(TARGET)
	@echo [SUCCESS] Cleaned build files


# Run the program
run: $(TARGET)
	@echo [RUN] Starting $(TARGET)...
	./$(TARGET)


# Rebuild
rebuild: clean all


# Print configuration
info:
	@echo === Build Configuration ===
	@echo OS: $(UNAME_S)
	@echo Compiler: $(CXX)
	@echo Flags: $(CXXFLAGS)
	@echo JSON Include: $(JSON_INC)
	@echo Include: $(INC_DIR)
	@echo Libs: $(LIB_DIR) $(LIBS)
	@echo Target: $(TARGET)


.PHONY: all clean run rebuild info
