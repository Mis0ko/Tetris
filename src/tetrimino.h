#ifndef _TETRIMINO_
#define _TETRIMINO_
using namespace std;
#include <iostream>
#include <SDL.h>
#include <vector>

#define TETR_SIZE 20

typedef enum tetrimino_type
{
    BARRE,  // cyan
    BLOC,   //jaune
    T_TYPE, //violet
    L_TYPE,
    J_TYPE,
    Z_TYPE,
    S_TYPE
} tetrimino_type;

// Used in the Window_surface ==> more convenient
// but transform in SDL_Color afterward
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
    static vector<vector<vector<vector<int>>>> matrix2; // contain all the tetriminos
    static vector<vector<vector<vector<int>>>> tetr_wallkick;
    SDL_Color *color;
    tetrimino_type type_t;
    int num_rot;
    //color_type color_t;

    SDL_Rect *shape;
    bool finished; // when the piece is immobile ==> finished = True

public:
    int size;
    vector<vector<int>> current_tetr;
    int x, y;
    Tetrimino(int x_, int y_, int size_, tetrimino_type type_t_);
    ~Tetrimino();
    //void draw(SDL_Renderer *rend);
    void print_tetrimino();
    //void transpose(void);
    //void reverseCols(void);
    void rotate(MOV_DIRECTION direction_);
    tetrimino_type get_type();
    int get_num_rot();
    vector<vector<vector<vector<int>>>> get_wallkick();
    void set_coord(int x_, int y_); // si -1 on touche pas à la coord
                                          /**
     * On utilise toujours les coordonnes x et y de la classe et pas ceux de shape, 
     * les coord de shapes sont actualisées à chaque draw
    **/
    void set_finished();
    bool getStateFinished();
    void print_coord();
};

#endif