/* board.h */
#ifndef _BOARD_
#define _BOARD_
using namespace std;
#include "tetrimino.h"
#include "window_surface.h"
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL_mixer.h"

/**
 * FREE : grill case empty, FILLED : grill case filled 
 * **/

/***
 *score:
  -ligne : 40 points 
  -deux lignes  :100 points
  -trois lignes : 300 points
  -quatre lignes : 1200 points .
 * **/

enum
{
    FREE,
    FILLED
};

typedef enum optionInfo
{
    SCORE,
    LINES,
} optionInfo;

const int BOARD_HEIGHT = 20;
const int BOARD_WIDTH = 10;

const int ORIGIN_X = 11;
const int ORIGIN_Y = 6;

class Board
{
private:
    SDL_Rect *carre_grill;
    SDL_Color *color[8];
    int totalLines;

    /* display on the screen infos */
    TTF_Font *police;
    SDL_Color colorPolice;
    SDL_Surface *textSurface;
    SDL_Rect *positionInfos;

    SDL_Texture *textTexture;
    char infosMsg[100];

public:
    Tetrimino *currentPiece; // La pièce courante se trouvant sur l'aire de jeu
    MOV_DIRECTION direction;
    vector<vector<int>> screenWithBlock;  // the screen with the falling block
    vector<vector<int>> screenBackground; // the screen without the falling block
    //vector<vector<int>> screenIABackground; // screenBackground to try the possibilities for the ia
    int totalScore;
    Board();

    void setCurrentPiece(Tetrimino *tetr);
    Tetrimino *getCurrentPiece();
    void print_board();
    void draw_board(SDL_Renderer *rend);
    void print_piece_to_board();
    void deletePieceFromBackground();
    void print_piece_to_background();
    void refresh_screen();
    int DetectCollision();
    int LookDown(int idy, int idx);
    int LookUp(int idy, int idx);
    int LookRight(int idy, int idx);
    int LookLeft(int idy, int idx);
    int TryRotate();
    bool IsGameOver();
    void GoFarDown();
    void GoFarUp();
    int LineFull();
    void BringDownColumns(int i_row);
    int nbLineFull();
    void update_direction(MOV_DIRECTION direction_);
    void moveCurrentPiece();
    void moveBackCurrentPiece();
    tetrimino_type GetRandomShape();
    Tetrimino *GenerateRandomShape();

    int computeScore(int nbLines);
    void setScore(int valueToAdd);

    int get_score();
    void setPositionInfos(optionInfo infos);
    void textInfos(optionInfo infos);
    void printInfosToScreen(SDL_Renderer *rend);
    void freeScoreText();
};

#endif