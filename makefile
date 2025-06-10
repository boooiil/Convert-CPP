CXX = g++
CCFLAGS = -std=c++20

BASEDIR = src
OBJ_DIR = obj
DIST_DIR = dist
OUTPUT_NAME = convert
PLATFORM = LINUX

DEBUG_ARGS = -d -i
ARGS = -a 2

SRCS := $(shell find $(BASEDIR) -type f -name '*.cpp')
LINUX_OBJS := $(patsubst $(BASEDIR)/%.cpp,$(DIST_DIR)/linux/$(OBJ_DIR)/%.o,$(SRCS))
WINDOWS_OBJS := $(patsubst $(BASEDIR)/%.cpp,$(DIST_DIR)/windows/$(OBJ_DIR)/%.o,$(SRCS))
INC_DIRS := $(shell find $(BASEDIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Add additional include paths for MinGW
MINGW_INC_FLAGS := -Iexternal -Iexternal/nlohmann

ifeq ($(OS),Windows_NT)
    PLATFORM = WIN32
endif

.PHONY: all linux windows clean valgrind debug run

all: linux windows

linux: $(LINUX_OBJS)
	$(CXX) $(LINUX_OBJS) -o $(DIST_DIR)/$(OUTPUT_NAME)

windows: $(WINDOWS_OBJS)
	x86_64-w64-mingw32-g++ $(WINDOWS_OBJS) -o $(DIST_DIR)/$(OUTPUT_NAME).exe $(MINGW_INC_FLAGS)

$(DIST_DIR)/linux/$(OBJ_DIR)/%.o: $(BASEDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CCFLAGS) -D LINUX $(INC_FLAGS) -c $< -o $@

$(DIST_DIR)/windows/$(OBJ_DIR)/%.o: $(BASEDIR)/%.cpp
	@mkdir -p $(dir $@)
	x86_64-w64-mingw32-g++ $(CCFLAGS) -D WIN32 $(INC_FLAGS) $(MINGW_INC_FLAGS) -c $< -o $@ 

clean:
	rm -rf $(DIST_DIR)
	rm convert
	rm convert.exe

valgrind: linux
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(DIST_DIR)/$(OUTPUT_NAME) $(DEBUG_ARGS)

debug: linux
	./$(DIST_DIR)/$(OUTPUT_NAME) $(DEBUG_ARGS)

run: linux
	./$(DIST_DIR)/$(OUTPUT_NAME) $(ARGS)
