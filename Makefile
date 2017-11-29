TARGET  = pacman-sdl
SRCS    = SDL_kanji.cpp enemy.cpp food.cpp image_manager.cpp input.cpp main.cpp main_chara.cpp map.cpp wipe.cpp
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

SDL_kanji.o: SDL_kanji.cpp SDL_kanji.h
enemy.o: enemy.cpp def_global.h enemy.h image_manager.h main.h main_chara.h \
 map.h
food.o: food.cpp def_global.h enemy.h food.h image_manager.h main_chara.h \
 map.h
image_manager.o: image_manager.cpp image_manager.h
input.o: input.cpp def_global.h input.h
main.o: main.cpp SDL_kanji.h def_global.h enemy.h food.h image_manager.h \
 input.h main_chara.h main.h map.h wipe.h
main_chara.o: main_chara.cpp def_global.h main_chara.h image_manager.h \
 input.h map.h
map.o: map.cpp def_global.h image_manager.h main.h map.h
wipe.o: wipe.cpp def_global.h wipe.h
