#include "window_surface.h"
#include "tetrimino.h"

WindowSurface::WindowSurface() : Surface()
{
    this->pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    rend = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    cout << "Renderer created" << endl;
    this->set_surf(pWindow);
    cout << "winSurface created" << endl;

    pauseRect = new SDL_Rect();
    pauseRect->h = WIN_H / 3;
    pauseRect->w = WIN_W / 2;
    pauseRect->y = WIN_H / 2 - pauseRect->h / 2;
    pauseRect->x = WIN_W / 2 - pauseRect->w / 2;

    police = TTF_OpenFont("src/GUNSHIP2.TTF", 20);
    colorPolice = {74, 69, 68};
    positionMenuInfos = new SDL_Rect();
    textButtonSurface = new SDL_Surface();

    menuBackgroundSprite = IMG_Load("src/tetrisLogo.png");
    if (menuBackgroundSprite == nullptr)
    {
        cout << "IMG_Load: " << IMG_GetError() << "\n";
    }
}

SDL_Window* WindowSurface::ResizeWindow(SDL_Window* win)
{
    int win_w;
    int win_h;
    int win_x;
    int win_y;

    SDL_GetWindowSize(win, &win_w, &win_h);
    SDL_SetWindowSize(win, 2 * win_w, win_h);
    SDL_GetWindowPosition(win, &win_x, &win_y);
    SDL_SetWindowPosition(win, win_x / 4, win_y);

    SDL_DestroyRenderer(rend);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return win;
}


SDL_Window *WindowSurface::get_w()
{
    return this->pWindow;
}

SDL_Renderer* WindowSurface::get_rend()
{
    return this->rend;
}

void WindowSurface::backgroundRender(SDL_Surface *spriteBg)
{
    SDL_Rect srcBg = { 0, 128, 96, 128 };
    Bg = SDL_CreateTextureFromSurface(rend, spriteBg);
    if (Bg == nullptr)
        printf("error creation texture\n");
    SDL_RenderCopy(rend, Bg, &srcBg, NULL);
    SDL_SetRenderDrawColor(rend, 213, 213, 213, 255);
    SDL_RenderFillRect(rend, NULL);
}

void WindowSurface::render(SDL_Surface *spriteBg, Board * board1, Board * board2, bool isPaused, bool menuMode)
{
    if (menuMode)
    {
        drawMenuScreen();
        SDL_RenderPresent(rend);
        return;
    }
    backgroundRender(spriteBg);

    board1->draw_board(rend);
    board1->printInfosToScreen(rend);
    if (board2 != nullptr)
    {
        board2->draw_board(rend);
        board2->printInfosToScreen(rend);
    }

    if (isPaused)
        drawPauseScreen();

    SDL_RenderPresent(rend); 

    board1->freeScoreText();
    if (board2 != nullptr)
        board2->freeScoreText();
}

void WindowSurface::textMenuInfos(menuInfo infos)
{
    switch (infos)
    {
    case RESUME:
        snprintf(menuMsg, 100, "RESUME");
        break;
    case QUIT:
        snprintf(menuMsg, 100, "QUIT");
        break;
    case COPYRIGHT:
        snprintf(menuMsg, 100, "MADE By Hofmann Michael AND Berthault Dylan");
        break;
    case PLAY:
        snprintf(menuMsg, 100, "PLAY");
        break;
    case IA:
        snprintf(menuMsg, 100, "  IA  ");
        break;
    case EXIT:
        snprintf(menuMsg, 100, "EXIT");
        break;
    }
    menuMsg[strlen(menuMsg)] = '\0';
}

void WindowSurface::setPositionInfos(menuInfo infos)
{
    int win_w;
    int win_h;

    SDL_GetWindowSize(this->pWindow, &win_w, &win_h);

    pauseRect->h = win_h / 3;
    pauseRect->w = win_w / 2;
    pauseRect->y = win_h / 2 - pauseRect->h / 2;
    pauseRect->x = win_w / 2 - pauseRect->w / 2;

    if (infos != COPYRIGHT)
    {
        positionMenuInfos->w = pauseRect->w / 2;
        positionMenuInfos->h = pauseRect->h / 7;
        positionMenuInfos->x = pauseRect->x + (pauseRect->w / 2 - positionMenuInfos->w / 2);
        if (infos > COPYRIGHT)
            positionMenuInfos->y = pauseRect->h + int(infos) * (positionMenuInfos->h + pauseRect->h / 5);
        else
            positionMenuInfos->y = pauseRect->h + (int(infos) + 1) * (positionMenuInfos->h + pauseRect->h / 7);
        return;
    }
    positionMenuInfos->w = win_w / 2;
    positionMenuInfos->h = pauseRect->h / 7;
    positionMenuInfos->x = win_w / 2;
    positionMenuInfos->y = win_h - positionMenuInfos->h;
}

void WindowSurface::drawBackgroundPauseScreen()
{
    SDL_SetRenderDrawColor(rend, 213, 213, 213, 255);
    SDL_RenderFillRect(rend, pauseRect);
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
    SDL_RenderDrawRect(rend, pauseRect);
}

void WindowSurface::drawButtonsPauseScreen()
{
    for (menuInfo infos = RESUME; infos < 2; infos = menuInfo(int(infos) + 1))
    {
        textMenuInfos(infos);
        textButtonSurface = TTF_RenderText_Solid(police, menuMsg, colorPolice);
        setPositionInfos(infos);
        textButtonTexture = SDL_CreateTextureFromSurface(rend, textButtonSurface);

        SDL_SetRenderDrawColor(rend, 213, 213, 213, 255); 
        SDL_RenderFillRect(rend, positionMenuInfos);
        SDL_SetRenderDrawColor(rend, 150, 150, 150, 255);
        SDL_RenderDrawRect(rend, positionMenuInfos);

        SDL_RenderCopy(rend, textButtonTexture, NULL, positionMenuInfos);
    }
}

void WindowSurface::drawPauseScreen()
{
    drawBackgroundPauseScreen();
    drawButtonsPauseScreen();
}

bool WindowSurface::xInsideResumeButton(int x)
{
    return x > positionMenuInfos->x && x < positionMenuInfos->x + positionMenuInfos->w ? true : false;
}

bool WindowSurface::yInsideResumeButton(int y, menuInfo infosM)
{
    if (int(infosM) > COPYRIGHT)
        return (y > (pauseRect->h + (int(infosM)) * (positionMenuInfos->h + pauseRect->h / 5)) && (y < (pauseRect->h + (int(infosM)) * (positionMenuInfos->h + pauseRect->h / 5) + positionMenuInfos->h))) ? true : false;
    return (y > (pauseRect->h + (int(infosM) + 1) * (positionMenuInfos->h + pauseRect->h / 7)) && (y < (pauseRect->h + (int(infosM) + 1) * (positionMenuInfos->h + pauseRect->h / 7) + positionMenuInfos->h))) ? true : false;
}

bool WindowSurface::isInsideResumeButtom(int x, int y, menuInfo infosM)
{
    return (xInsideResumeButton(x) && yInsideResumeButton(y, infosM));
}

void WindowSurface::drawBackgroundMenuScreen()
{
    SDL_Rect menuBg;
    menuBg.x = 0;
    menuBg.y = 0;
    menuBg.w = WIN_W;
    menuBg.h = WIN_H;

    SDL_SetRenderDrawColor(rend, 213, 213, 213, 255);
    SDL_RenderFillRect(rend, &menuBg);
    SDL_RenderDrawRect(rend, &menuBg);

    SDL_Rect srcMenuBg = {128, 57, 568, 198};
    Bg = SDL_CreateTextureFromSurface(rend, menuBackgroundSprite); 
    if (Bg == nullptr)
        printf("error creation texture\n");

    SDL_Rect dest = {WIN_W / 2 - srcMenuBg.w / 2, WIN_H / 3, 600, 200};
    SDL_RenderCopy(rend, Bg, &srcMenuBg, &dest);
}

void WindowSurface::drawButtonsMenuScreen()
{
    for (menuInfo infos = COPYRIGHT; infos < 6; infos = menuInfo(int(infos) + 1))
    {
        textMenuInfos(infos);
        textButtonSurface = TTF_RenderText_Solid(police, menuMsg, colorPolice);
        setPositionInfos(infos);
        textButtonTexture = SDL_CreateTextureFromSurface(rend, textButtonSurface);

        SDL_SetRenderDrawColor(rend, 150, 150, 150, 255); 
        SDL_RenderDrawRect(rend, positionMenuInfos);
        SDL_RenderCopy(rend, textButtonTexture, NULL, positionMenuInfos);
    }
}

void WindowSurface::drawMenuScreen()
{
    drawBackgroundMenuScreen();
    drawButtonsMenuScreen();
}