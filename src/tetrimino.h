#ifndef _TETRIMINO_
#define _TETRIMINO_
using namespace std;
#include <iostream>
#include <SDL.h>
#include <vector>

/***
 * size that determines the size of the squares composing the board and
 * the tetriminos
 * ***/
#define TETR_SIZE 20

/***
  * all the tetriminos types
  * ***/
typedef enum tetrimino_type
{
   BARRE,
   BLOC,
   T_TYPE,
   L_TYPE,
   J_TYPE,
   Z_TYPE,
   S_TYPE
} tetrimino_type;

/***
 * all the tetriminos colors
 * ***/
typedef enum color_type
{
   CYAN,
   YELLOW,
   PURPLE,
   ORANGE,
   BLUE,
   RED,
   GREEN
} color_type;

/***
 * all the tetriminos direction (NO_MOVE is the default direction,
 * FAR_DOWN is the hard drop of a piece)
 * ***/
typedef enum MOV_DIRECTION
{
   LEFT,
   RIGHT,
   DOWN,
   ROT_R,
   ROT_L,
   NO_MOVE,
   FAR_DOWN
} MOV_DIRECTION;

class Tetrimino
{
protected:
   /***
 * matrix2 : a matrix containing all the tetriminos and their 4 roations
 *              each tetrimino has a number (1 to 7) representing its color
 * tetr_wallkick : a matrix representing the possibilities a tetrimino has when 
 *                  it wants to rotate (following the Super Rotation System)
 * color : color for the tetriminos
 * type_t : type of the tetrimino (barre, bloc, etc...)
 * num_rot : number of the actual rotation state of the piece (0 to 3)
 * shape : shape of the tetriminos
 * finished : a bool stating if the piece is immobile (true) or still moving (false)
 * ***/

   static vector<vector<vector<vector<int>>>> matrix2;
   static vector<vector<vector<vector<int>>>> tetr_wallkick;
   SDL_Color *color;
   tetrimino_type type_t;
   int num_rot;
   SDL_Rect *shape;
   bool finished;

public:
   /***
 * current_tetr : the current tetrimino shape and rotation state
 * x, y : positions of the tetrimino
 * ***/
   vector<vector<int>> current_tetr;
   int x, y;

   /***
 * tetrimino constructor and destructor
 * ***/
   Tetrimino(int x_, int y_, tetrimino_type type_t_);
   ~Tetrimino();

   /***
 * rotate the tetrimino according to the direction_ (left or right rotation)
 * does not look if the rotation is possible or not, it is the function
 * TryRotate() in the board class that does it
 * ***/
   void rotate(MOV_DIRECTION direction_);

   /***
 * return the type of the tetrimino (barre, bloc, etc...)
 * ***/
   tetrimino_type get_type();

   /***
 * return the rotation number (state) of the current tetrimino
 * ***/
   int get_num_rot();

   /***
 * return the matrix tetr_wallkick seen before
 * ***/
   vector<vector<vector<vector<int>>>> get_wallkick();

   /***
 * change the coordinates of the current tetrimino
 * ***/
   void set_coord(int x_, int y_);

   /***
 * set the state of the piece as "finished" (called when the piece is landed)
 * ***/
   void set_finished();

   /***
 * return true if the state is "finished" (the piece is immobile) or false if not
 * ***/
   bool getStateFinished();
};

#endif