TARGET = pacman-sdl
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
SRC_DIR = src
OBJ_DIR = build
CC = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -pedantic -Wformat=2 -Wstrict-aliasing=2 -MMD
LDFLAGS = `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lm
DEPENDS = $(OBJS:.o=.d)

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	-rm -f $(OBJS) $(TARGET)

-include $(DEPENDS)
