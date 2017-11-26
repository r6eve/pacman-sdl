TARGET  = pacman-sdl
SRCS    = SDL_kanji.c enemy.c food.c image_manager.c input.c main.c main_chara.c map.c wipe.c
OBJS    = $(SRCS:%.c=%.o)
CC      = gcc
CFLAGS  = -std=c99 -O2 -Wall -Wextra -pedantic -Wformat=2 -Wstrict-aliasing=2
LDFLAGS = `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lm

.SUFFIXES: .c .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	-rm -f $(OBJS) $(TARGET)

SDL_kanji.o: SDL_kanji.c SDL_kanji.h
enemy.o: enemy.c def_global.h enemy.h image_manager.h main.h main_chara.h \
 map.h
food.o: food.c def_global.h enemy.h food.h image_manager.h main_chara.h \
 map.h
image_manager.o: image_manager.c image_manager.h
input.o: input.c def_global.h input.h
main.o: main.c SDL_kanji.h def_global.h enemy.h food.h image_manager.h \
 input.h main_chara.h main.h map.h wipe.h
main_chara.o: main_chara.c def_global.h main_chara.h image_manager.h \
 input.h map.h
map.o: map.c def_global.h image_manager.h main.h map.h
wipe.o: wipe.c def_global.h wipe.h
