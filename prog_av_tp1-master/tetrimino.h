#ifndef _TETRIMINO_
#define _TETRIMINO_
using namespace std;
#include <iostream>
#include <SDL.h>
#include <vector>

#define TETR_SIZE 30

typedef enum tetrimino_type
{
    BARRE,
    BLOC,
    T_TYPE,
    L_TYPE,
    L_REVERSE,
    Z_TYPE,
    S_TYPE
} tetrimino_type;

// Used in the Window_surface ==> more convenient
// but transform in SDL_Color afterward
typedef enum color_type
{
    RED,
    GREEN,
    BLUE
} color_type;

typedef enum MOV_DIRECTION
{
    LEFT,
    RIGHT,
    DOWN,
    UP,
    NO_MOVE
} MOV_DIRECTION;

class Tetrimino
{
protected:
    static vector<vector<vector<int>>> matrix2; // contain all the tetriminos
    SDL_Color *color;
    tetrimino_type type_t;
    color_type color_t;
    int size;
    SDL_Rect *shape;


public:
    vector<vector<int>> current_tetr;
    double x, y;
    Tetrimino(double x_, double y_, int size_, tetrimino_type type_t_, color_type color_t_);
    ~Tetrimino();
    void draw(SDL_Renderer *rend);
    void print_tetrimino();
    void transpose(void);
    void reverseCols(void);
    void rotate(void);


    void set_coord(double x_, double y_); // si -1 on touche pas à la coord
                                          /**
     * On utilise toujours les coordonnes x et y de la classe et pas ceux de shape, 
     * les coord de shapes sont actualisées à chaque draw
    **/

};

#endif