#ifndef _WINDOW_SURFACE_
#define _WINDOW_SURFACE_
using namespace std;
#include <SDL.h>
#include "surface.h"
#include "sprite.h"
#include "tetrimino.h"
#include "board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#define WIN_W 990
#define WIN_H 990

typedef enum menuInfo
{
    RESUME,
    QUIT,
    COPYRIGHT,
    PLAY,
    IA,
    EXIT,
} menuInfo;

class Board;
class Sprite;
class Surface;




class WindowSurface : public Surface
{
protected:
    SDL_Window *pWindow;
    SDL_Renderer *rend;
    SDL_Texture *Bg;
    SDL_Rect *pauseRect;

    /* display Menu infos on the screen */
    TTF_Font *police;
    SDL_Color colorPolice;
    SDL_Surface *textButtonSurface;
    SDL_Rect *positionMenuInfos;
    SDL_Texture *textButtonTexture;
    char menuMsg[100];

    SDL_Surface* menuBackgroundSprite;

public:
    WindowSurface();
    SDL_Window *get_w();
    void backgroundRender(SDL_Surface *spriteBg);
    void render(SDL_Surface *spriteBg, Board *board, bool isPaused, bool menuMode);
    void textMenuInfos(menuInfo infos);
    void setPositionInfos(menuInfo infos);
    void drawBackgroundPauseScreen();
    void drawButtonsPauseScreen();
    void drawPauseScreen();

    bool xInsideResumeButton(int x);

    bool yInsideResumeButton(int y, menuInfo infosM);
    bool isInsideResumeButtom(int x, int y, menuInfo infosM);

    void drawBackgroundMenuScreen();
    void drawButtonsMenuScreen();
    void drawMenuScreen();
};

#endif