TARGET  = pacman-sdl
SRCS    = $(wildcard *.cpp)
OBJS    = $(SRCS:%.cpp=%.o)
CC      = g++
CFLAGS  = -std=c++11 -O2 -Wall -Wextra -pedantic -Wformat=2 -Wstrict-aliasing=2
LDFLAGS = `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lm

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	-rm -f $(OBJS) $(TARGET)

SDL_kanji.o: SDL_kanji.cpp SDL_kanji.hpp
enemy.o: enemy.cpp def_global.hpp enemy.hpp image_manager.hpp main.hpp main_chara.hpp \
 map.hpp
food.o: food.cpp def_global.hpp enemy.hpp food.hpp image_manager.hpp main_chara.hpp \
 map.hpp
image_manager.o: image_manager.cpp image_manager.hpp
input.o: input.cpp def_global.hpp input.hpp
main.o: main.cpp SDL_kanji.hpp def_global.hpp enemy.hpp food.hpp image_manager.hpp \
 input.hpp main_chara.hpp main.hpp map.hpp wipe.hpp
main_chara.o: main_chara.cpp def_global.hpp main_chara.hpp image_manager.hpp \
 input.hpp map.hpp
map.o: map.cpp def_global.hpp image_manager.hpp main.hpp map.hpp
wipe.o: wipe.cpp def_global.hpp wipe.hpp
