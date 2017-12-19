#ifndef SDL_KANJI_H
#define SDL_KANJI_H

/*
 * 変更 2010/9/4  Kanji_PutText(), Kanji_CreateSurface() を可変引数対応に.
 *                引数の順番を変更.
 * 変更 2010/9/5  Kanji_CodingSystem に Kanji_UTF8 を追加.
 * 変更 2011/9/10  RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, BLACK, WHITEを初期化する
 *                 void init_color(void) 関数を追加. エラーは起こらないので返り値なし.
 *                 使い方は, SDL_kanji.h をインクルードし, init_color()を呼ぶ.
 *                 後は, 文字の色を指定するところで上の変数を指定すれば良い.
 *
 */

#include <SDL/SDL.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef SDL_KANJI
#define GLOBAL_KANJI
#else
#define GLOBAL_KANJI extern
#endif

GLOBAL_KANJI SDL_Color RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, BLACK, WHITE;

void init_color(void);

typedef enum { KANJI_SJIS, KANJI_EUC, KANJI_JIS, KANJI_UTF8 } Kanji_CodingSystem;

typedef struct {
	int k_size;
	int a_size;
	Kanji_CodingSystem sys;
	/* メモリ効率わるいけど、まあ死にはしないわな。 */
	Uint32* moji[96*96+256];
} Kanji_Font;

Kanji_Font* Kanji_OpenFont(const char* file, int size);

int Kanji_AddFont(Kanji_Font* font, const char* file);

void Kanji_SetCodingSystem(Kanji_Font* font, Kanji_CodingSystem sys);

int Kanji_FontHeight(Kanji_Font* font);
/* text == 0 なら ASCII 一文字分の幅を返す */
int Kanji_FontWidth(Kanji_Font* font, const char* text);

SDL_Surface* Kanji_CreateSurface(Kanji_Font* font, SDL_Color fg, int bpp, const char* text, ...);
int Kanji_PutText(SDL_Surface* dst, int dx, int dy, Kanji_Font* font, SDL_Color fg, const char* text, ...);
SDL_Surface* Kanji_CreateSurfaceTate(Kanji_Font* font, const char* text, SDL_Color fg, int bpp);
int Kanji_PutTextTate(Kanji_Font* font, int dx, int dy, SDL_Surface* dst, const char* text, SDL_Color fg);

void Kanji_CloseFont(Kanji_Font* font);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif
