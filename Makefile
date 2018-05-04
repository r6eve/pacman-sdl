TARGET = pacman-sdl
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
SRC_DIR = src
OBJ_DIR = build
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic -Wformat=2 -Wstrict-aliasing=2 -MMD
LDFLAGS = `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lm
DEPENDS = $(OBJS:.o=.d)

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	-rm -rf $(OBJ_DIR) $(TARGET)

-include $(DEPENDS)
