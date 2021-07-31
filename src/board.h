/* board.h */
#ifndef _BOARD_
#define _BOARD_
using namespace std;
#include "tetrimino.h"
#include "window_surface.h"
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL_mixer.h"

/**
 * FREE : grill case empty
 *  FILLED : grill case filled 
 * **/
enum
{
    FREE,
    FILLED
};

/***
 * option Info mainly used
 * for the menuMode
 * ***/
typedef enum optionInfo
{
    SCORE,
    LINES,
    LEVEL,
    HOLD,
    NEXT
} optionInfo;

/***
 * Dimension of the board, represent the
 * number of lines and columns
 * ***/
const int BOARD_HEIGHT = 20;
const int BOARD_WIDTH = 10;

/***
 * y coordinate of the board top
 * ***/
const int ORIGIN_Y = 7;

class Board
{
private:

    /***
     * carre_grill : square of the general grill
     * color[8] : color of the 8 possibles blocks (7 tetrimino and the grill)
     * nbHold : number of times we used a hold for a piece (only one allowed per piece)
     * totalLines : number of lines done
     * Level : current level of the game
     * IsOut : bool stating if the piece is out of the board (for the Game Over)
     * ORIGIN_X : x coordinate of the position of the board
     * ***/
    SDL_Rect *carre_grill; 
    SDL_Color *color[8];   
    int nbHold;            
    int totalLines;       
    int Level;
    bool IsOut;
    int ORIGIN_X;

    /***
     *  display on the screen infos
     * ***/
    TTF_Font *police;
    SDL_Color colorPolice;
    SDL_Surface *textSurface;
    SDL_Rect *positionInfos;
    SDL_Texture *textTexture;
    char infosMsg[100]; 

public:

    /***
     * currentPiece : current piece which is falling in the screen
     * holdPiece : the piece which is hold with the C key
     * NextPieces[3] : the nexts 3 pieces
     * direction : direction of the current piece
     * screenWithBlock : the screen with the falling block
     * screenBackground : the screen without the falling block
     * screenHold : the screen of the hold piece
     * screenNextPieces : the screen of the 3 next pieces
     * totalScore : current score of the game
     * ***/
    Tetrimino *currentPiece;  
    Tetrimino *holdPiece;     
    Tetrimino *NextPieces[3]; 
    MOV_DIRECTION direction;
    vector<vector<int>> screenWithBlock;  
    vector<vector<int>> screenBackground; 
    vector<vector<int>> screenHold;      
    vector<vector<int>> screenNextPieces; 
    int totalScore;

    /***
     * board constructor, the parameter is used to settled the x coordinate of 
     * the beginning of the board (which differs whether it's solo or vsIA mode)
     * ***/
    Board(int origin_x);

    /***
     * set the piece tetr as the current piece 
     * ***/
    void setCurrentPiece(Tetrimino *tetr);

    /***
     * get the current piece 
     * ***/
    Tetrimino *getCurrentPiece();

    /***
     * print on the render the screenWithBlock (background + curent Piece)
     * start with a refresh of the board ie screenWithBlock is assign by screenBackground
     * ***/
    void draw_board(SDL_Renderer *rend);

    /***
     * assign to screenWithBlock the value of the currentPiece
     * ***/
    void print_piece_to_board();

    /***
     * delete the currentPiece from screenBackground, used for the IA Mode
     * ***/
    void deletePieceFromBackground();

    /***
     * assign the current piece to the variable screenBackground so the 
     * piece stay visible when we change the currentPiece
     * ***/
    void print_piece_to_background();

    /***
     * assign the holdPiece to the screenHold
     * ***/
    void print_piece_to_hold();

    /***
     * assign the 3 nexts pieces to screenNextPieces
     * ***/
    void print_piece_to_next();

    /***
     * find where to print the projection and set the value which
     * correspond to the projection (9) at the concerned cases
     * ***/
    void print_projection();
    /***
     * update the nexts 3 pieces if needed
     * when the current piece is set to finished
     * ***/
    void update_screenNextPieces();

    /***
     * Refresh the screenWithBlock with screenbackground
     * it is done before we check if the current piece
     * can do its next moove.
     * Thanks to this function we dont have to bother
     * to erase where the current piece was in the previous moove.
     * ***/
    void refresh_screen();

    /***
     * Check if we can do the moove in the direction
     * wanted
     * ***/
    int DetectCollision();

    /***
     * Check just under the current piece if there is another
     * piece or the border
     * ***/
    int LookDown(int idy, int idx);

    /***
     * Check just above the current piece if there is another
     * piece or the border
     * ***/
    int LookUp(int idy, int idx);

    /***
     * Check at the direct right the current piece if there is another
     * piece or the border
     * ***/
    int LookRight(int idy, int idx);

    /***
     * Check at the direct Left the current piece if there is another
     * piece or the border
     * ***/
    int LookLeft(int idy, int idx);

    /***
     * try to rotate the current piece and check if it doesnt go through the border 
     * of the board / another piece
    * ***/
    void TryRotate();

    //bool IsGameOver();

    /***
     * allow the piece to go off the board in a straight line with space key
     * ***/
    void GoFarDown();

    /***
     * allow the piece to go up the board in a straight line (for AI)
     * ***/
    void GoFarUp();

    /***
     * Find all the full lines, bring down the colums and return the nb 
     * of lines that were found
     * ***/
    int LineFull();

    /***
     * Bring Down Colums for the Row with index i_row 
     * (used when Line Full is used)
     * **/
    void BringDownColumns(int i_row);

    /***
     * count the nb of lines full but doesnt erase them/ bring them down.
     * this function is used for the AI
     * ***/
    int nbLineFull();

    /***
     * Update the level of the Tetris game when the nb of lines
     * is high enough
     * ***/
    void UpdateLevel();

    /***
     * return the level of the current game
     * ***/
    int get_level();

    /***
     * set the nb of times we pressed the hold button
     * ***/
    void set_nbHold(int new_nbHold);

    /***
     * get the nb of times we pressed the hold button
     * ***/
    int get_nbHold();

    /***
     * get the ISOut value, representing if the current piece is out of the board
     * ***/
    bool get_IsOut();

    /***
     * set the IsOut value, used by the IA_Player
     * ***/
    void set_IsOut(bool new_IsOut);

    /***
     * set the direction of the current round of the tetris
     * ***/
    void update_direction(MOV_DIRECTION direction_);

    /***
     * moove the current Piece depending on the current direction 
     * ***/
    void moveCurrentPiece();

    /***
     * when a hold is requested and its the first time, change current piece
     * and hold piece
     * ***/
    void changePiece();

    /***
     * generate a random type of tetrimino send to generateRandomShape function
     * ***/
    tetrimino_type GetRandomShape();

    /***
     * generate a random tetrimino
     * ***/
    Tetrimino *GenerateRandomShape();

    /***
     * Compute the score of the Current PIece 
     * (depending of the lines it fills and its y position)
     * 
     * This function is called after the piece is in place
     * and before the new one is generated
     * ***/
    int computeScore(int nbLines);

    /***
     * set the current score with the new one generate on the current time 
     * ***/
    void setScore(int valueToAdd);

    /***
     * get the current value of the score
     * ***/
    int get_score();

    /***
     * set the position of the differents infos on the menu mode
     * ***/
    void setPositionInfos(optionInfo infos);

    /***
     * print the msg that will be displayed in the menu mode into the infosMsg variable
     * ***/
    void textInfos(optionInfo infos);

    /***
     * display the options (score, lines, level) in the render 
     * ***/
    void printInfosToScreen(SDL_Renderer *rend);

    /***
     * free the structure used for the generation of the text part
     * ***/
    void freeScoreText();
};

#endif