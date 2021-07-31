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

#define WIN_W 600
#define WIN_H 600

/***
 * all the menu infos and buttons
 * ***/
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

    /***
 * all the SDL variables we need to create the window, the renderer, the background surface,
 * menu surface and to get background and menu sprites 
 * ***/
    SDL_Window *pWindow;
    SDL_Renderer *rend;
    SDL_Texture *Bg;
    SDL_Rect *pauseRect;

    TTF_Font *police;
    SDL_Color colorPolice;
    SDL_Surface *textButtonSurface;
    SDL_Rect *positionMenuInfos;
    SDL_Texture *textButtonTexture;
    char menuMsg[100];

    SDL_Surface* menuBackgroundSprite;

public:

    /***
 * creates the window
 * ***/
    WindowSurface();

    /***
 * returns the window
 * ***/
    SDL_Window *get_w();

    /***
 * returns the renderer
 * ***/
    SDL_Renderer* get_rend();

    /***
 * resizes the window (used for the player vs IA mode)
 * ***/
    SDL_Window* ResizeWindow(SDL_Window* win);

    /***
 * creates the background renderer
 * ***/
    void backgroundRender(SDL_Surface *spriteBg);

    /***
 * creates the whole renderer (calls different function like backgroundRender or
 * functions that draw the board)
 * ***/
    void render(SDL_Surface *spriteBg, Board* board1, Board* board2, bool isPaused, bool menuMode);

    /***
 * print the infos in parameter according to what it is
 * ***/
    void textMenuInfos(menuInfo infos);

    /***
 * set the infos position according to what the parameter infos is
 * ***/
    void setPositionInfos(menuInfo infos);

    /***
 * creates the background of the pause screen
 * ***/
    void drawBackgroundPauseScreen();

    /***
 * creates the buttons of the pause screen
 * ***/
    void drawButtonsPauseScreen();

    /***
 * display the pause screen
 * ***/
    void drawPauseScreen();

    /***
 * returns if whether or not the mouse cursor is inside the button 
 * between x and (x + width)
 * ***/
    bool xInsideResumeButton(int x);

    /***
 * returns if whether or not the mouse cursor is inside the button 
 * between y and (y + height) according to the infos (the y parameter depends
 * on the infos)
 * ***/
    bool yInsideResumeButton(int y, menuInfo infosM);

    /***
 * use the 2 functions above to return if the mouse click is in a button
 * ***/
    bool isInsideResumeButtom(int x, int y, menuInfo infosM);

    /***
 * creates the background of the menu screen
 * ***/
    void drawBackgroundMenuScreen();

    /***
 * creates the buttons of the pause screen
 * ***/
    void drawButtonsMenuScreen();

    /***
 * display the pause screen
 * ***/
    void drawMenuScreen();
};

#endif